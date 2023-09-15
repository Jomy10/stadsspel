#include "os.h"
#include "types.h"

#if osUnix || __has_include(<uuid/uuid.h>)

  #include <uuid/uuid.h>

#elif osWindows

  #include <rpc/Rpc.h>
  #define uuid_t UUID
  #define uuid_generate UuidCreate
  int uuid_compare(uuid_t a, uuid_t b);

#else

  typedef unsigned char uuid_t[16];
  int uuid_compare(uuid_t a, uuid_t b);
  void uuid_generate(uuid_t uuid);

#endif

u64 uuid_hash(const void* uuid, u64 seed0, u64 seed1);

int uuid_compare2(const void* uuida, const void* uuidb, void* udata);
