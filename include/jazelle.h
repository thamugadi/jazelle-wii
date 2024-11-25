
#include <stdint.h>
#include <stdbool.h>
// 512 KiB of stack

#define STACK_BASE 0x90000000 
#define LOCAL_VARS (STACK_BASE + 0x80000) 

void exec_jazelle();
