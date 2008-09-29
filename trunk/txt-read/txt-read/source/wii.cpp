#include "wii.h"
#include <di/di.h>
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void WIILIGHT_Init();

void init(){
	DI_Init();
  	// Initialise the video system
	VIDEO_Init();
	
	// This function initialises the attached controllers
	WPAD_Init();
	// Initialise GC pads
	PAD_Init();
	
	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	// Initialise the console, required for printf
// 	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	CON_InitEx(rmode, 20,30,rmode->fbWidth-40,rmode->xfbHeight-60); // Better way not working yet
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	
	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);
	
	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();
	VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// Initalise all inserted memory storage devices
	fatInitDefault();
	// Initialise disc slot light
	WIILIGHT_Init();
	// Initialise keyboard
	KEYBOARD_Init();
}
