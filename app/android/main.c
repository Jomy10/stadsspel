// Modified from https://github.com/cnlohr/rawdrawandroid
// Modified by: Jonas Everaert (see git history for changes)
//Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD License you choose.
// NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "os_generic.h"
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android_native_app_glue.h>
#include <android/sensor.h>
#include <byteswap.h>
#include <errno.h>
#include <fcntl.h>
#include "CNFGAndroid.h"

//#define CNFA_IMPLEMENTATION
#define CNFG_IMPLEMENTATION
#define CNFG3D

//#include "cnfa/CNFA.h"
#include "CNFG.h"

#include <util/allocator.h>
#include <map_data/render_objects.h>

// #define WEBVIEW_NATIVE_ACTIVITY_IMPLEMENTATION
// #include "webview_native_activity.h"

void AndroidDisplayKeyboard(int pShow);

int lastbuttonx = 0;
int lastbuttony = 0;
int lastmotionx = 0;
int lastmotiony = 0;
int lastbid = 0;
int lastmask = 0;
int lastkey, lastkeydown;

static int keyboard_up;
uint8_t buttonstate[8];

void HandleKey( int keycode, int bDown )
{
	lastkey = keycode;
	lastkeydown = bDown;
	if( keycode == 10 && !bDown ) { keyboard_up = 0; AndroidDisplayKeyboard( keyboard_up );  }

	if( keycode == 4 ) { AndroidSendToBack( 1 ); } //Handle Physical Back Button.
}

void HandleButton( int x, int y, int button, int bDown )
{
	buttonstate[button] = bDown;
	lastbid = button;
	lastbuttonx = x;
	lastbuttony = y;

	printf("Press at %i, %i\n", x, y);

	// if( bDown ) { keyboard_up = !keyboard_up; AndroidDisplayKeyboard( keyboard_up ); }
}

void HandleMotion( int x, int y, int mask )
{
	lastmask = mask;
	lastmotionx = x;
	lastmotiony = y;
}

short screenx, screeny;

extern struct android_app * gapp;

void HandleDestroy()
{
	printf( "Destroying\n" );
	exit(10);
}

volatile int suspended;

void HandleSuspend()
{
	suspended = 1;
}

void HandleResume()
{
	suspended = 0;
}

void HandleThisWindowTermination()
{
	suspended = 1;
}

// The pixel buffer for olive
uint32_t* data = NULL;

int main( int argc, char ** argv )
{
  MapRenderObjects objs = makeRenderObjects();
  printf("render objects from zig: %p\n", objs.ptr);

	CNFGBGColor = 0xFF000000;
	CNFGSetupFullscreen( "Stadsspel", 0 );

	HandleWindowTermination = HandleThisWindowTermination;

	while(1)
	{
		CNFGHandleInput();

		if( suspended ) {
		  usleep(50000);
				continue;
		}

		CNFGClearFrame();
		// CNFGColor( 0xFFFFFFFF );

		// (Re-)allocate pixel buffer
		short prevx = screenx, prevy = screeny;
		CNFGGetDimensions( &screenx, &screeny );
		if (prevx != screenx || prevy != screeny) {
		  printf("New screen size: %i %i\n", screenx, screeny);
		  if (data != NULL) {
				free(data);
			}
			data = malloc(screenx * screeny * 4);
		}

		int x, y;
		for( y = 0; y < screeny; y++ )
  		for( x = 0; x < screenx; x++ )
  			data[x+y*screenx] = 0xFFFF00FF; // x | ((x*394543L+y*355+rand()*3)<<8);
		CNFGBlitImage( data, 0, 0, screenx, screeny );

		//On Android, CNFGSwapBuffers must be called, and CNFGUpdateScreenWithBitmap does not have an implied framebuffer swap.
		CNFGSwapBuffers();
	}

	return(0);
}
