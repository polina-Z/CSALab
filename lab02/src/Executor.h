
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        Word alu_value = Alu(instr);
        instr->_data = CalculateData(instr, ip, alu_value);
        instr->_addr = alu_value;
        instr->_nextIp = CalculateNextIp(instr, ip);
    }

private:
    Word Alu(InstructionPtr& instr)
    {
        Word A = instr->_src1Val;
        Word B = instr->_imm.value_or(instr->_src2Val);
        switch (instr->_aluFunc)
        {
            case AluFunc::Add:
            {
                return A + B;
            }
            case AluFunc::Sub:
            {
                return A - B;
            }
            case AluFunc::And:
            {
                return A & B;
            }
            case AluFunc::Or:
            {
                return A | B;
            }
            case AluFunc::Xor:
            {
                return A ^ B;
            }
            case AluFunc::Slt:
            {
                return (SignedWord)(A) < (SignedWord)(B);
            }
            case AluFunc::Sltu:
            {
                return A < B;
            }
            case AluFunc::Sll:
            {
                return A << (B % 32);
            }
            case AluFunc::Srl:
            {
                return A >> (B % 32);
            }
            case AluFunc::Sra:
            {
                return (SignedWord)(A) >> (B % 32);
            }
            default:
            {
                return 0xdeadbeaf;
            }
        }
    }

    Word CalculateData (InstructionPtr& instr, Word ip, Word alu_value)
    {
        switch (instr->_type)
        {
            case IType::Csrr:
            {
                return instr->_csrVal;
            }
            case IType::Csrw:
            {
                return instr->_src1Val;
            }
            case IType::St:
            {
                return instr->_src2Val;
            }
            case IType::J:
            case IType::Jr:
            {
                return ip + 4;
            }
            case IType::Auipc:
            {
                return ip + instr->_imm.value();
            }
            default:
            {
                return alu_value;
            }
        }
    }

  
    Word CalculateNextIp (InstructionPtr& instr, Word ip)
    {
        bool isJump = false;
        switch(instr->_brFunc)
        {
            case BrFunc::Eq:
            {
                isJump = instr->_src1Val == instr->_src2Val;
                break;
            }
            case BrFunc::Neq:
            {
                isJump = instr->_src1Val != instr->_src2Val;
                break;
            }
            case BrFunc::Lt:
            {
                isJump = (SignedWord)(instr->_src1Val) < (SignedWord)(instr->_src2Val);
                break;
            }
            case BrFunc::Ltu:
            {
                isJump = instr->_src1Val < instr->_src2Val;
                break;
            }
            case BrFunc::Ge:
            {
                isJump = (SignedWord)(instr->_src1Val) >= (SignedWord)(instr->_src2Val);
                break;
            }
            case BrFunc::Geu:
            {
                isJump = instr->_src1Val >= instr->_src2Val;
                break;
            }
            case BrFunc::AT:
            {
                isJump = true;
                break;
            }
            case BrFunc::NT:
            {
                isJump = false;
                break;
            }
        }

        Word jumpTo = ip + 4;
        if (isJump)
        {
            switch(instr->_type)
            {
                case IType::Br:
                case IType::J:
                {
                    jumpTo = ip + instr->_imm.value();
                    break;
                }
                case IType::Jr:
                {
                    jumpTo = instr->_src1Val + instr->_imm.value();
                    break;
                }
            }
        }
        return jumpTo;
    }

};

#endif // RISCV_SIM_EXECUTOR_H
