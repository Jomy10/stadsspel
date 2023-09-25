#include <android/log.h>
#include <android_native_app_glue.h>
#include <jni.h>
extern "C" {
  #include <arena/varena.h>
}
#define APP_NAME "stadsspel"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))

void android_main(struct android_app* state) {
  LOGI("Hello world");
  VariableArena arena = makeVariableArena(1);
  while(true) {}
}

// #include <MiniFB.h>
// #include <MiniFB_enums.h>
// #include <MiniFB_cpp.h>
// #include <android/log.h>
// #include <android_native_app_glue.h>
// #include <atomic>
// #include <jni.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <assert.h>
// #include <app/bufferReallocator.h>

// #define OLIVEC_IMPLEMENTATION
// #include <olive.c>

// #define APP_NAME "stadsspel"

// #define LOGI(...) \
//   ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))
// #define LOGW(...) \
//   ((void)__android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__))
// #define LOGE(...) \
//   ((void)__android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__))

// struct AppState {
//   struct mfb_window* window;
//   unsigned int w;
//   unsigned int h;
//   uint32_t* buffer;
//   Olivec_Canvas canvas;
//   struct BufferReallocator br;

//   void(*onResize)(struct mfb_window* window, unsigned int width, unsigned int height);
//   void(*onFocusChanged)(struct mfb_window* window, bool isActive);
//   bool(*onClose)(struct mfb_window* window);
// };

// static struct AppState app;

// void resize_window(struct mfb_window* window, unsigned int width, unsigned int height) {
//   app.w = width;
//   app.h = height;
//   app.buffer = (uint32_t*) br_alloc(&app.br, app.w * app.h * sizeof(uint32_t));
//   app.canvas = olivec_canvas(app.buffer, app.w, app.h, app.w);
// }

// int main(int argc, char** argv) {
//   app = {};
//   app.w = 1;
//   app.h = 1;
//   app.window = mfb_open_ex(APP_NAME, app.w, app.h, WF_RESIZABLE);
//   if (!app.window) return 1;
//   app.br = br_init();
//   // Todo: callbacks

// }
