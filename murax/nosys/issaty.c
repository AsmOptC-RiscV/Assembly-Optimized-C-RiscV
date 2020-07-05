/*
 * Stub version.
 */

#include <errno.h>
#undef errno
extern int errno;

int
_isatty (int file)
{
  errno = ENOSYS;
  return 0;
}