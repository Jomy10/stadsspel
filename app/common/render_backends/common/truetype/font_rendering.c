#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <renderer/renderer.h>
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb/stb_image_write.h>

GFont gLoadFontFileDescriptor(FILE* file) {
  long size;
  unsigned char* fontBuffer;

  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);

  fontBuffer = malloc(size);

  fread(fontBuffer, size, 1, file);

  stbtt_fontinfo* info = malloc(sizeof(stbtt_fontinfo));
  if (!stbtt_InitFont(info, fontBuffer, 0)) {
    free(info);
    free(fontBuffer);
    return NULL;
  }

  free(fontBuffer);

  return (void*) info;
}

GFont gLoadFont(char* fontFile) {
  FILE* f = fopen(fontFile, "rb");
  if (fontFile == NULL) {
    return NULL;
  }
  GFont* font = gLoadFontFileDescriptor(f);
  fclose(f);
  return font;
}

void gFreeFont(GFont font) {
  free(font);
}

void gDrawText(GSurface* surface, GFont _font, const char* text, __attribute__((nonnull)) const GFontOpts* opts) {
  stbtt_fontinfo* font = (stbtt_fontinfo*) _font;

  float scale = stbtt_ScaleForPixelHeight(font, opts->lineHeight);

  int x = 0;

  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
  ascent = roundf(ascent * scale);
  descent = round(descent * scale);

  // TODO: init pixels
  //void* bitmap = calloc(surface->w * surface->h, surface->bytesPerPixel);

  for (unsigned long i = 0; i < strlen(text); i++) {
    // How wide is this char
    int ax; // advance x
    int lsb;
    stbtt_GetCodepointHMetrics(font, text[i], &ax, &lsb);

    // Get bounding box
    int bx1, bx2, by1, by2;
    stbtt_GetCodepointBitmapBox(font, text[i], scale, scale, &bx1, &by1, &bx2, &by2);

    // compute y
    int y = ascent + by1;

    // render char (stride and offset is important here)
    int byteOffset = x + roundf(lsb * scale) + (y * surface->w);
    stbtt_MakeCodepointBitmap(
      font,
      surface->pixels + byteOffset,
      surface->bytesPerPixel * (bx2 - bx1),
      surface->bytesPerPixel * (by2 - by1),
      surface->w,
      scale, scale,
      text[i]);

    // Advance x
    x += roundf(ax * scale);

    // Add kerning
    int kern = stbtt_GetCodepointKernAdvance(font, text[i], text[i + 1]);
    x += roundf(kern * scale);
  }
}

// int main() {
//   long size;
//   unsigned char* fontBuffer;

//   FILE* fontFile = fopen("Roboto-italic.ttf", "rb");
//   if (fontFile == NULL) {
//     printf("Couldn't find file\n");
//     return -1;
//   }
//   fseek(fontFile, 0, SEEK_END);
//   size = ftell(fontFile);
//   fseek(fontFile, 0, SEEK_SET);

//   fontBuffer = malloc(size);

//   fread(fontBuffer, size, 1, fontFile);
//   fclose(fontFile);

//   stbtt_fontinfo info;
//   if (!stbtt_InitFont(&info, fontBuffer, 0)) {
//     printf("Failed\n");
//     exit(-1);
//   }

//   int w = 512, h = 128, lineHeight = 64;

//   unsigned char* bitmap = calloc(w * h, sizeof(unsigned char));

//   float scale = stbtt_ScaleForPixelHeight(&info, lineHeight);
//   char* txt = "The quick brown fox";

//   int x = 0;

//   int ascent, descent, lineGap;
//   stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

//   ascent = roundf(ascent * scale);
//   descent = round(descent * scale);

//   for (int i = 0; i < (int) strlen(txt); i++) {
//     // How wide is this char
//     int ax;
//     int lsb;
//     stbtt_GetCodepointHMetrics(&info, txt[i], &ax, &lsb);

//     // Get bounding box
//     int bx1, bx2, by1, by2;
//     stbtt_GetCodepointBitmapBox(&info, txt[i], scale, scale, &bx1, &by1, &bx2, &by2);

//     // compute y
//     int y = ascent + by1;

//     // render char (stride and offset is important here)
//     int byteOffset = x + roundf(lsb * scale) + (y * w);
//     stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, bx2 - bx1, by2 - by1, w, scale, scale, txt[i]);

//     // Advance x
//     x += roundf(ax * scale);

//     // Add kerning
//     int kern = stbtt_GetCodepointKernAdvance(&info, txt[i], txt[i + 1]);
//     x += roundf(kern * scale);
//   }

//   stbi_write_png("out.png", w, h, 1, bitmap, w);

//   free(fontBuffer);
//   free(bitmap);

//   return 0;
// }
