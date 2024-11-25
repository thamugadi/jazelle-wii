#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gccore.h>
#include <ogcsys.h>
#include <ogc/ipc.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include <jazelle.h>
#include <arm.h>

void exec_jazelle() {
  int i;
  for (int i = 0; i < sizeof(arm_code)/4; i++) {
    *(u32*)(0x92000000+i*4) = arm_code[i];
  }
  memset((void*)STACK_BASE, 0, (LOCAL_VARS-STACK_BASE)+0x100); 
 
  DCFlushRange((void*)STACK_BASE, (LOCAL_VARS-STACK_BASE)+0x100);
  IOS_Write(-1, (void*)STACK_BASE,  (LOCAL_VARS-STACK_BASE)+0x100);

  u32* arm_code_mem2 = (u32*)0x92000000;

  DCFlushRange((u32*)arm_code, sizeof(arm_code));
  DCFlushRange(arm_code_mem2, sizeof(arm_code));

  u32* search = arm_code_mem2;
  while(*search != 0xabcdefaa) {
    search++;
  }
  search++;
  u32 len = *search + 1;
  search++;
  u8* bytecode = (u8*)search;

  printf("JVM bytecode to be executed: ");
  for (i = 0; i < len; i++) {
    printf("%02x ", bytecode[i]);
  }
  printf("\n");

  printf("Performing Palapeli's /dev/sha exploit\n");
  #ifdef THUMB
  run_arm((u32*)arm_code_mem2, sizeof(arm_code), true);
  #else
  run_arm((u32*)arm_code_mem2, sizeof(arm_code), false);
  #endif
  printf("Starlet is running in Jazelle mode to execute the specified JVM bytecode\n");

  while (*(u32*)0x93000000 != 0xeeeeeeee); // flag used in exit_jazelle
  
  DCFlushRange((void*)STACK_BASE, (LOCAL_VARS-STACK_BASE)+0x100);
  IOS_Write(-1, (void*)STACK_BASE,  (LOCAL_VARS-STACK_BASE)+0x100);

  u32* stack = (u32*)(0x80000000 + (*(u32*)0x93000004));
  printf("Finished execution. Stack: ");  
  for (u32* si = stack-1; si >= (u32*)STACK_BASE; si--) {
    printf("%08x ", *si);
  }
  printf("\n");
  for (i = 0; i < 8; i++) {
    printf("Local variable %08x: %08x\n", (u32)(LOCAL_VARS+i*4), *(u32*)(LOCAL_VARS+i*4));
  }
}
