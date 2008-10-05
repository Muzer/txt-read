#include "wii.h"
#include <di/di.h>
#include <string.h>
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void WIILIGHT_Init();

int di_enabled = 0;

char ascii ( char s ) {
	if ( s < 0x20 ) return '.';
	if ( s > 0x7E ) return '.';
	return s;
}

char * display_tmd_info ( const tmd *t ) {
	u32 title_l = t->title_id & 0xFFFFFFFF;
	char title_ascii[5];
	u32 i;
	memcpy ( title_ascii, &title_l, 4 );
	for ( i = 0; i < 4; i++ ) title_ascii[i] = ascii ( title_ascii[i] );
	title_ascii[4] = '\0';
	if ( strstr ( title_ascii, "DVDX" ) != NULL ) {DI_Init();di_enabled = 1;}
	return (char *)"";
}


int init() {

	s32 ret;
	u32 count;
	ret = ES_GetNumTitles ( &count );
	static u64 title_list[256] ATTRIBUTE_ALIGN ( 32 );
	ret = ES_GetTitles ( title_list, count );
	u32 i;
	for ( i = 0; i < count; i++ ) {
		u32 tmd_size;
		ret = ES_GetStoredTMDSize ( title_list[i], &tmd_size );
		static u8 tmd_buf[MAX_SIGNED_TMD_SIZE] ATTRIBUTE_ALIGN ( 32 );
		signed_blob *s_tmd = ( signed_blob * ) tmd_buf;
		ret = ES_GetStoredTMD ( title_list[i], s_tmd, tmd_size );
		display_tmd_info ( (tmd *)SIGNATURE_PAYLOAD ( s_tmd ));
	}
	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();
	// Initialise GC pads
	PAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode ( NULL );

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1 ( SYS_AllocateFramebuffer ( rmode ) );

	// Initialise the console, required for printf
// 	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	CON_InitEx ( rmode, 20, 30, rmode->fbWidth - 40, rmode->xfbHeight - 60 ); // Better way not working yet
	// Set up the video registers with the chosen mode
	VIDEO_Configure ( rmode );

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer ( xfb );

	// Make the display visible
	VIDEO_SetBlack ( FALSE );

	// Flush the video register changes to the hardware
	VIDEO_Flush();
	VIDEO_ClearFrameBuffer ( rmode, xfb, COLOR_BLACK );

	// Wait for Video setup to complete
	VIDEO_WaitVSync();

	if ( rmode->viTVMode&VI_NON_INTERLACE ) VIDEO_WaitVSync();

	// Initalise all inserted memory storage devices
	fatInitDefault();

	// Initialise disc slot light
	WIILIGHT_Init();

	// Initialise keyboard
	KEYBOARD_Init();
	return di_enabled;
}
