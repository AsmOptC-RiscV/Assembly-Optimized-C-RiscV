/*
 * Stub version.
 */

#include <errno.h>
#undef errno
extern int errno;

int
_kill (int pid,
        int sig)
{
  errno = ENOSYS;
  return -1;
}
