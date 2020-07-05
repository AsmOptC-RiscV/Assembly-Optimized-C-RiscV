/*
 * Stub version.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#undef errno
extern int errno;

int
_fstat (int          fildes,
        struct stat *st)
{
  errno = ENOSYS;
  return -1;
}
