#ifndef WEAK_UNDER_ALIAS_H
#define WEAK_UNDER_ALIAS_H

#define weak_under_alias(name)                                          \
  extern __typeof (__weak_##name) name __attribute__ ((weak, alias ("__weak_"#name)))

#endif /* WEAK_UNDER_ALIAS_H */
