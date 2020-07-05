/*
 * Stub version.
 */

#include <errno.h>
#undef errno
extern int errno;

int
_getpid (void)
{
  errno = ENOSYS;
  return -1;
}
