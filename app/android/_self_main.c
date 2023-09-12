// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "os_generic.h"
// #include <GLES3/gl3.h>
// #include <android/asset_manager.h>
// #include <android/asset_manager_jni.h>
// #include <android_native_app_glue.h>
// #include <android/sensor.h>
// #include <errno.h>
// #include <fcntl.h>
// #include "CNFGAndroid.h"
// #include "rawdraw/CNFG.h"

// #define CNFG_IMPLEMENTATION
// #define CNFG3D

// #include "CNFG.h"

// // #define WEBVIEW_NATIVE_ACTIVITY_IMPLEMENTATION
// // #include "webview_native_activity.h"

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
#include <android/log.h>
#include <byteswap.h>
#include <errno.h>
#include <fcntl.h>
#include "CNFGAndroid.h"
#include "rawdraw/CNFG.h"

//#define CNFA_IMPLEMENTATION
#define CNFG_IMPLEMENTATION
#define CNFG3D

#define LOGI(tag, fmt, ...) __android_log_print(ANDROID_LOG_INFO, tag, fmt, __VA_ARGS__)

//#include "cnfa/CNFA.h"
#include "CNFG.h"

#define WEBVIEW_NATIVE_ACTIVITY_IMPLEMENTATION
#include "webview_native_activity.h"



// Keyboard input
void AndroidDisplayKeyboard(int pShow);

int lastkey, lastkeydown;
static int keyboard_up;

void HandleKey(int keycode, int bDown) {
  lastkey = keycode;
  lastkeydown = bDown;

  LOGI("stadspel-Key", "Key pressed: %i, down = %i\n", keycode, bDown);
  printf("Key was pressed %i\n", keycode);

  // if( keycode == 10 && !bDown ) { keyboard_up = 0; AndroidDisplayKeyboard( keyboard_up );  }

	if( keycode == 4 ) { AndroidSendToBack( 1 ); } //Handle Physical Back Button.
}

int lastbid = 0;
int lastbuttonx = 0;
int lastbuttony = 0;
uint8_t buttonstate[8];

void HandleButton(int x, int y, int button, int bDown) {
  buttonstate[button] = bDown;
  lastbid = button;
  lastbuttonx = x;
  lastbuttony = y;

  // Logging no work :()
  LOGI("stadssel-button", "Pressed at %i, %i. Button = %i (down = %i)\n", x, y, button, bDown);
  printf("button %i %i\n", x, y );

  // if (bDown) {
  //   keyboard_up = !keyboard_up;
  //   AndroidDisplayKeyboard(keyboard_up);
  // }
}

int lastmask = 0,
    lastmotionx = 0,
    lastmotiony = 0;
void HandleMotion(int x, int y, int mask) {
  lastmask = mask;
  lastmotionx = x;
  lastmotiony = y;
}

void drawAndroidApp() {

}

void HandleDestroy() {
  printf("Destroying\n");
  exit(10);
}

volatile int suspended;
void HandleSuspend() {
  suspended = 1;
}

void HandleResume() {
  suspended = 0;
}

void handleThisWindowTermination() {
  suspended = 1;
}

short screenx, screeny;

uint32_t imgdata[100 * 100 ];

uint32_t randomtexturedata[256*256];
int iframeno = 1;

int main(int argc, char** argv) {
  CNFGBGColor = 0x000040ff;
	CNFGSetupFullscreen( "Stadsspel", 0 );

  HandleWindowTermination = handleThisWindowTermination;

  // Main thread
  while (CNFGHandleInput()) {
    //CNFGHandleInput();

    if (suspended) {
      usleep(50000);
      continue;
    }

    CNFGBGColor = 0x000080ff; //Dark Blue Background

		CNFGClearFrame();
		CNFGGetDimensions( &screenx, &screeny );

		//Change color to white.
		CNFGColor( 0xffffffff );

		CNFGPenX = 1; CNFGPenY = 1;
		CNFGDrawText( "Hello, World", 2 );
		//Draw a white pixel at 3,0 30
		CNFGTackPixel( 30, 30 );

		//Draw a line from 50,50 to 100,50
		CNFGTackSegment( 50, 50, 100, 50 );

		//Dark Red Color Select
		CNFGColor( 0x800000ff );

		//Draw 50x50 box starting at 100,50
		CNFGTackRectangle( 100, 50, 150, 100 );

		//Draw a triangle
		RDPoint points[3] = { { 30, 36 }, { 20, 50 }, { 40, 50 } };
		CNFGTackPoly( points, 3 );

		// Blit random pixel data
		{
			static uint32_t data[64*64];
			int x, y;

			for( y = 0; y < 64; y++ ) for( x = 0; x < 64; x++ )
				data[x+y*64] = 0xff | (rand()<<8);

			CNFGBlitImage( data, 120, 190, 64, 64 );
		}

		//Display the image and wait for time to display next frame.
		CNFGSwapBuffers();

  //   CNFGBGColor = 0x000080ff; //Dark Blue Background
  //   CNFGClearFrame();
  //   // CNFGColor(0xFFFF00FF);
  //   CNFGGetDimensions(&screenx, &screeny);

  //   CNFGColor(0xFFFFFFFF);

  //   CNFGSetLineWidth(9);
  //   CNFGPenX = 0;
  //   CNFGPenY = 400;
  //   CNFGColor(0xFF00FFFF);
  //   CNFGDrawText("hello world", 15);

  //   {
  //  	  static uint32_t data[64*64];
		// 	int x, y;

		// 	for( y = 0; y < 64; y++ ) for( x = 0; x < 64; x++ )
		// 		data[x+y*64] = 0xff | (rand()<<8);

		// 	CNFGBlitImage( data, 120, 190, 64, 64 );
  //   }

  //   // for (int i = 0; i < 100 * 100; i++) {
  //   //   imgdata[i] = 0xFF;
  //   // }
  //   // CNFGBlitImage(imgdata, 0, 0, 100, 100);

  // //   int x, y;
		// // for( y = 0; y < 256; y++ )
  // // 		for( x = 0; x < 256; x++ )
  // // 			randomtexturedata[x+y*256] = x | ((x*394543L+y*355+iframeno*3)<<8);

		// // CNFGBlitImage( randomtexturedata, 100, 600, 256, 256 );

  //   //CNFGFlushRender();
  //   CNFGSwapBuffers();
  }

  printf("Goodbye!\n");

  return 0;
}
