#define M_NARGS(...) M_NARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define M_NARGS_(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N

#define M_CONC(A, B) M_CONC_(A, B)
#define M_CONC_(A, B) A##B
// #define M_ID(...) __VA_ARGS__

#define M_FOR_EACH(ACTN, ...) M_CONC(M_FOR_EACH_, M_NARGS(__VA_ARGS__)) (ACTN, __VA_ARGS__)

#define M_FOR_EACH_0(ACTN, E) E
#define M_FOR_EACH_1(ACTN, E) ACTN(E)
#define M_FOR_EACH_2(ACTN, E, ...) ACTN(E) M_FOR_EACH_1(ACTN, __VA_ARGS__)
#define M_FOR_EACH_3(ACTN, E, ...) ACTN(E) M_FOR_EACH_2(ACTN, __VA_ARGS__)
#define M_FOR_EACH_4(ACTN, E, ...) ACTN(E) M_FOR_EACH_3(ACTN, __VA_ARGS__)
#define M_FOR_EACH_5(ACTN, E, ...) ACTN(E) M_FOR_EACH_4(ACTN, __VA_ARGS__)

#define __ENUM_VAL(name) T_##name ,

#define __UNION_FIELD(name) name as##name;

#define TaggedUnion(name, ...) \
  struct name { \
      enum { \
          M_FOR_EACH(__ENUM_VAL, __VA_ARGS__) \
      } type; \
      union { \
          M_FOR_EACH(__UNION_FIELD, __VA_ARGS__) \
      } value; \
  }

// MATCH Statement
#define M_CONC_N3(A, B, C) M_CONC_N3_(A, B, C)
#define M_CONC_N3_(A, B, C) A##B##C

#define M_FOR_EACH_N2_E(ACTN, EV, ...) M_CONC_N3(M_FOR_EACH_, M_NARGS(__VA_ARGS__), _N2_E) (ACTN, EV, __VA_ARGS__)

#define M_FOR_EACH_0_N2_E(ACTN, ExtraVar, E, B) ExtraVar, E, B
#define M_FOR_EACH_2_N2_E(ACTN, EV, E, B) ACTN(EV, E, B)
#define M_FOR_EACH_4_N2_E(ACTN, EV, E, B, ...) ACTN(EV, E, B) M_FOR_EACH_2_N2_E(ACTN, EV, __VA_ARGS__)
#define M_FOR_EACH_6_N2_E(ACTN, EV, E, B, ...) ACTN(EV, E, B) M_FOR_EACH_4_N2_E(ACTN, EV, __VA_ARGS__)
#define M_FOR_EACH_8_N2_E(ACTN, EV, E, B, ...) ACTN(EV, E, B) M_FOR_EACH_6_N2_E(ACTN, EV, __VA_ARGS__)
#define M_FOR_EACH_10_N2_E(ACTN, EV, E, B, ...) ACTN(EV, E, B) M_FOR_EACH_8_N2_E(ACTN, EV, __VA_ARGS__)

#define __MATCH_CASE(munion, TypeName, block) case T_##TypeName: { TypeName* _value = &munion.value.as##TypeName; block; };

#define match(munion, ...) \
  switch (munion.type) { \
      M_FOR_EACH_N2_E(__MATCH_CASE, munion, __VA_ARGS__) \
  }
