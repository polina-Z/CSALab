#include "ll_cycle.h"

bool ll_has_cycle(node *head) {
    node *turtle = head;
    node *rabbit = head;
    
    while(rabbit != nullptr)
    {
        rabbit = rabbit->next;
        turtle = turtle->next;
        if(rabbit == nullptr)
        {
            return false;
        }
        rabbit = rabbit->next;
        if(rabbit == nullptr)
        {
            return false;
        }
        if(rabbit == turtle)
        {
            return true;
        }        
    }
    return false;
}
