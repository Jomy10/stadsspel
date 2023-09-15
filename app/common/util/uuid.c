#include <uuid.h>

#if !osUnix && !__has_include(<uuid/uuid.h>) && !osWindows

#include <stdlib.h>

void uuid_generate(uuid_t uuid) {
  const char v[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  for (int i = 0; i < 16; i++) {
    uuid[i] = v[rand()%16];
  }
}

int uuid_compare(uuid_t a, uuid_t b) {
  return memcmp((const unsigned char*)a, (const unsigned char*)b, 16);
}

#endif

#if osWindows

int uuid_compare(uuid_t a, uuid_t b) {
  RPC_STATUS status;
  return UuidCompare((UUID*)&a, (UUID*)&b, &status);
}

#endif


#include <hashmap.h>

u64 uuid_hash(const void *uuid, uint64_t seed0, uint64_t seed1) {
  return hashmap_sip(uuid, 16, seed0, seed1);
}

int uuid_compare2(const void *uuida, const void *uuidb, __attribute__((unused)) void *udata) {
  const uuid_t* a = (const uuid_t*) uuida;
  const uuid_t* b = (const uuid_t*) uuidb;

  return uuid_compare(*a, *b);
}
