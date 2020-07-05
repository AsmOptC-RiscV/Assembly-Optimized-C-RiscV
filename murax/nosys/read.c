/*
 * Stub version.
 */

#include <errno.h>
#undef errno
extern int errno;

int
_read (int   file,
        char *ptr,
        int   len)
{
  errno = ENOSYS;
  return -1;
}