// #include <android/log.h>
// #include <android_native_app_glue.h>
// #include <jni.h>
// extern "C" {
//   #include <arena/varena.h>
// }
// #define APP_NAME "stadsspel"

// #define LOGI(...) \
//   ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))

// void android_main(struct android_app* state) {
//   LOGI("Hello world");
//   VariableArena arena = makeVariableArena(1);
//   while(true) {}
// }

#include <MiniFB.h>
#include <MiniFB_enums.h>
#include <MiniFB_cpp.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <atomic>
#include <jni.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
extern "C" {
  #include <app/bufferReallocator.h>
}

#define OLIVEC_IMPLEMENTATION
#include <olive.c>

#define APP_NAME "stadsspel"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__))

struct AppState {
  struct mfb_window* window;
  unsigned int w;
  unsigned int h;
  uint32_t* buffer;
  Olivec_Canvas canvas;
  struct BufferReallocator br;

  void(*onResize)(struct mfb_window* window, unsigned int width, unsigned int height);
  void(*onFocusChanged)(struct mfb_window* window, bool isActive);
  bool(*onClose)(struct mfb_window* window);
};

static struct AppState app;

void resize_window(struct mfb_window* window, unsigned int width, unsigned int height) {
  LOGI("Resizing window to %ui %ui", width, height);
  app.w = width;
  app.h = height;
  app.buffer = (uint32_t*) br_alloc(&app.br, app.w * app.h * sizeof(uint32_t));
  app.canvas = olivec_canvas(app.buffer, app.w, app.h, app.w);
}

int main(int argc, char** argv) {
  app = {};
  app.w = 1;
  app.h = 1;
  app.window = mfb_open_ex(APP_NAME, app.w, app.h, WF_RESIZABLE);
  if (!app.window) return 1;
  app.br = br_init();
  // Todo: callbacks

  mfb_update_state state;
  do {
    unsigned int newwidth = mfb_get_window_width(app.window);
    unsigned int newheight= mfb_get_window_height(app.window);
    if (newwidth != app.w || newheight != app.h)
      resize_window(app.window, newwidth, newheight);

    olivec_fill(app.canvas, 0xFFb00bFF);

    state = mfb_update_ex(app.window, app.buffer, app.w, app.h);
    if (state < 0) {
      app.window = nullptr;
      LOGI("Window destroyed");
      break;
    }
  } while(mfb_wait_sync(app.window));
}
