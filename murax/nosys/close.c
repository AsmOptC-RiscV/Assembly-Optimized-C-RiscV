/*
 * Stub version.
 */

#include <errno.h>
#undef errno
extern int errno;

int
_close (int fildes)
{
  errno = ENOSYS;
  return -1;
}
