//#include <renderer/renderer_run.h>
#include <map_data/render_objects.h>
#include <map_data/parse.h>

int main() {
  FILE* f = fopen("map.o5m", "rb");
  char* buffer;
  long filelen;

  f = fopen("map.o5m", "rb");
  fseek(f, 0, SEEK_END);
  filelen = ftell(f);
  rewind(f);

  buffer = (char*) malloc(filelen * sizeof(char));
  fread(buffer, filelen, 1, f);
  fclose(f);

  FILE* f2 = fmemopen(buffer, filelen, "rb");

  MapRenderObjects objs = parseMapFileDescriptorToRenderObjects(f2);
  RO_determineBounds(&objs);
  printObjsSummary(&objs);
  fclose(f2);
  //run_main_renderer();
}
