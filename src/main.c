#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gccore.h>
#include <ogcsys.h>
#include <ogc/ipc.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include <arm.h>
#include <jazelle.h>

static void* xfb = NULL;
static GXRModeObj* rmode = NULL;

int main(int argc, char** argv) {
  VIDEO_Init();
  WPAD_Init(); 
  rmode = VIDEO_GetPreferredMode(NULL);
  xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
						      
  console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(xfb);
  VIDEO_SetBlack(false);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
  printf("\x1b[2;0H");
  printf("aramya's jazelle experiments\n");

  exec_jazelle();

  while(1) {
    VIDEO_WaitVSync();
    sleep(0x40);
    SYS_ResetSystem(SYS_RESTART,0,0);
  }
  return 0;
}
