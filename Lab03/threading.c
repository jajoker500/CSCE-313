#include <threading.h>

void t_init()
{
        for (uint8_t i = 0; i < NUM_CTX; ++i) { //loop through each context to initialize
                contexts[i].state = INVALID; // set each to invalid
        }

        current_context_idx = 0; // start at 0
        getcontext(&contexts[current_context_idx].context); // initialize its info
        contexts[current_context_idx].state = VALID; // start using it so set to valid
}

int32_t t_create(fptr foo, int32_t arg1, int32_t arg2)
{ 
        uint8_t i = current_context_idx; // start at current
        do {
                ++i; // increment i
                if(i >= NUM_CTX) i = 0; // if end of context array start over
                if(i == current_context_idx) return -1; // if you get back to the original context then none free
        }
        while(contexts[i].state != INVALID); // look for invalid thats available

        contexts[i].state = VALID; // set that one to valid as using it
        getcontext(&contexts[i].context); // initialize information

        contexts[i].context.uc_stack.ss_sp = (char *) malloc(STK_SZ); // move the stack pointer to make room
        contexts[i].context.uc_stack.ss_size = STK_SZ; // declare size on stack
        contexts[i].context.uc_stack.ss_flags = 0; // no flags
        contexts[i].context.uc_link = NULL; // no links
        makecontext(&contexts[i].context, (void (*)())foo, 2, arg1, arg2); // make the context
        return 0; // worked so return 0
}

int32_t t_yield()
{
        getcontext(&contexts[current_context_idx].context); // update status of context

        int32_t count = 0; // set valid count = 0
        if(contexts[current_context_idx].state == VALID) ++count; // if the current one is valid increment
        uint8_t i = current_context_idx; // start at the current
        int next_VALID = -1; // set next valid to error incase none valid found

        do {
                ++i; // increment i
                if(i >= NUM_CTX) i = 0; // if all contexts seen then start over
                if(i == current_context_idx) break; // if looped back no valid so exit loop
                if(contexts[i].state == VALID) { // if there is a valid context
                        if(next_VALID == -1) { // if the first valid context
                                next_VALID = i; // set next valid context = the current iteration
                        }
                        ++count; // increase valid count
                }
        }
        while(1); // loop until break

        if(next_VALID == -1) { // if no valid found
                return next_VALID; // return -1 (error)
        }

        uint8_t old_context_idx = current_context_idx; // update id
        current_context_idx = (uint8_t)next_VALID; // static cast from int to correct type and update new id
        swapcontext(&contexts[old_context_idx].context, &contexts[current_context_idx].context); // swap old for new

        return count; // return VALID count
}

void t_finish()
{
        contexts[current_context_idx].state = DONE; // set it to complete

        uint8_t i = current_context_idx; // go through the whole thing again looking for next valid
        do {
                ++i;
                if(i >= NUM_CTX) i = 0;
                if(i == current_context_idx) return; // looped back to self none
        }
        while(contexts[i].state != VALID);
        
        free(contexts[current_context_idx].context.uc_stack.ss_sp); // free memory
        contexts[current_context_idx].context.uc_stack.ss_sp = NULL; // set the pointer to null
        memset(&contexts[current_context_idx].context, 0, sizeof(ucontext_t)); // freeing memory

        current_context_idx = i; // update id

        setcontext(&contexts[current_context_idx].context); // set context to new context

}
