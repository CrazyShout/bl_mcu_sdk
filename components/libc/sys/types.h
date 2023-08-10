/* unified sys/types.h:
   start with sef's sysvi386 version.
   merge go32 version -- a few ifdefs.
   h8300hms, h8300xray, and sysvnecv70 disagree on the following types:

   typedef int gid_t;
   typedef int uid_t;
   typedef int dev_t;
   typedef int ino_t;
   typedef int mode_t;
   typedef int caddr_t;

   however, these aren't "reasonable" values, the sysvi386 ones make far
   more sense, and should work sufficiently well (in particular, h8300
   doesn't have a stat, and the necv70 doesn't matter.) -- eichin
 */

#ifndef _SYS_TYPES_H

#include <_ansi.h>
#include <sys/cdefs.h>
#include <machine/_types.h>

/* BSD types permitted by POSIX and always exposed as in Glibc.  Only provided
   for backward compatibility with BSD code.  The uintN_t standard types should
   be preferred in new code. */
#if ___int8_t_defined
typedef __uint8_t	u_int8_t;
#endif
#if ___int16_t_defined
typedef __uint16_t	u_int16_t;
#endif
#if ___int32_t_defined
typedef __uint32_t	u_int32_t;
#endif
#if ___int64_t_defined
typedef __uint64_t	u_int64_t;
#endif
typedef __intptr_t register_t;
#define __BIT_TYPES_DEFINED__ 1

#ifndef __need_inttypes

#define _SYS_TYPES_H
#include <sys/_types.h>
#include <sys/_stdint.h>

#if __BSD_VISIBLE
#include <machine/endian.h>
// #include <sys/select.h>
#  define	physadr		physadr_t
#  define	quad		quad_t

#ifndef _IN_ADDR_T_DECLARED
typedef	__uint32_t	in_addr_t;	/* base type for internet address */
#define	_IN_ADDR_T_DECLARED
#endif

#ifndef _IN_PORT_T_DECLARED
typedef	__uint16_t	in_port_t;
#define	_IN_PORT_T_DECLARED
#endif

typedef	__uintptr_t	u_register_t;
#endif /* __BSD_VISIBLE */

#if __MISC_VISIBLE
#ifndef _BSDTYPES_DEFINED
/* also defined in mingw/gmon.h and in w32api/winsock[2].h */
#ifndef __u_char_defined
typedef	unsigned char	u_char;
#define __u_char_defined
#endif
#ifndef __u_short_defined
typedef	unsigned short	u_short;
#define __u_short_defined
#endif
#ifndef __u_int_defined
typedef	unsigned int	u_int;
#define __u_int_defined
#endif
#ifndef __u_long_defined
typedef	unsigned long	u_long;
#define __u_long_defined
#endif
#define _BSDTYPES_DEFINED
#endif
#endif /* __MISC_VISIBLE */

#if __MISC_VISIBLE
typedef	unsigned short	ushort;		/* System V compatibility */
typedef	unsigned int	uint;		/* System V compatibility */
typedef	unsigned long	ulong;		/* System V compatibility */
#endif

#ifndef _BLKCNT_T_DECLARED
typedef	__blkcnt_t	blkcnt_t;
#define	_BLKCNT_T_DECLARED
#endif

#ifndef _BLKSIZE_T_DECLARED
typedef	__blksize_t	blksize_t;
#define	_BLKSIZE_T_DECLARED
#endif

#if !defined(__clock_t_defined) && !defined(_CLOCK_T_DECLARED)
// typedef	_CLOCK_T_	clock_t;
#define	__clock_t_defined
#define	_CLOCK_T_DECLARED
#endif

#if !defined(__time_t_defined) && !defined(_TIME_T_DECLARED)
typedef	_TIME_T_	time_t;
#define	__time_t_defined
#define	_TIME_T_DECLARED
#endif

#ifndef __daddr_t_defined
typedef	long	daddr_t;
#define __daddr_t_defined
#endif
#ifndef __caddr_t_defined
typedef	char *	caddr_t;
#define __caddr_t_defined
#endif

#ifndef _FSBLKCNT_T_DECLARED		/* for statvfs() */
typedef	__fsblkcnt_t	fsblkcnt_t;
typedef	__fsfilcnt_t	fsfilcnt_t;
#define	_FSBLKCNT_T_DECLARED
#endif

#ifndef _ID_T_DECLARED
typedef	__id_t		id_t;		/* can hold a uid_t or pid_t */
#define	_ID_T_DECLARED
#endif

#ifndef _INO_T_DECLARED
typedef	__ino_t		ino_t;		/* inode number */
#define	_INO_T_DECLARED
#endif

#if defined(__i386__) && (defined(GO32) || defined(__MSDOS__))
typedef	char *		addr_t;
typedef unsigned long vm_offset_t;
typedef unsigned long vm_size_t;
#endif /* __i386__ && (GO32 || __MSDOS__) */

/*
 * All these should be machine specific - right now they are all broken.
 * However, for all of Cygnus' embedded targets, we want them to all be
 * the same.  Otherwise things like sizeof (struct stat) might depend on
 * how the file was compiled (e.g. -mint16 vs -mint32, etc.).
 */

#ifndef _OFF_T_DECLARED
typedef	__off_t		off_t;		/* file offset */
#define	_OFF_T_DECLARED
#endif
#ifndef _DEV_T_DECLARED
typedef	__dev_t		dev_t;		/* device number or struct cdev */
#define	_DEV_T_DECLARED
#endif
#ifndef _UID_T_DECLARED
typedef	__uid_t		uid_t;		/* user id */
#define	_UID_T_DECLARED
#endif
#ifndef _GID_T_DECLARED
typedef	__gid_t		gid_t;		/* group id */
#define	_GID_T_DECLARED
#endif

#ifndef _PID_T_DECLARED
typedef	__pid_t		pid_t;		/* process id */
#define	_PID_T_DECLARED
#endif

#ifndef _KEY_T_DECLARED
typedef	__key_t		key_t;		/* IPC key */
#define	_KEY_T_DECLARED
#endif

#ifndef _SSIZE_T_DECLARED
// typedef _ssize_t ssize_t;
#define	_SSIZE_T_DECLARED
#endif

#ifndef _MODE_T_DECLARED
// typedef	__mode_t	mode_t;		/* permissions */
#define	_MODE_T_DECLARED
#endif

#ifndef _NLINK_T_DECLARED
typedef	__nlink_t	nlink_t;	/* link count */
#define	_NLINK_T_DECLARED
#endif

#if !defined(__clockid_t_defined) && !defined(_CLOCKID_T_DECLARED)
// typedef	__clockid_t	clockid_t;
#define	__clockid_t_defined
#define	_CLOCKID_T_DECLARED
#endif

#if !defined(__timer_t_defined) && !defined(_TIMER_T_DECLARED)
// typedef	__timer_t	timer_t;
#define	__timer_t_defined
#define	_TIMER_T_DECLARED
#endif

#ifndef _USECONDS_T_DECLARED
typedef	__useconds_t	useconds_t;	/* microseconds (unsigned) */
#define	_USECONDS_T_DECLARED
#endif

#ifndef _SUSECONDS_T_DECLARED
typedef	__suseconds_t	suseconds_t;
#define	_SUSECONDS_T_DECLARED
#endif

typedef	__int64_t	sbintime_t;

#include <sys/features.h>
#ifndef CONFIG_POSIX
#include <sys/_pthreadtypes.h>
#endif
#include <machine/types.h>

//因为bl sdk 中libc组件会默认使用自己写的而非编译器提供的内建函数，如libc
//os组件中的posix子项提供了同样的sys/types.h会引起编译报错，重复定义
//迁移posix子项下的sys/types.h中的部分关键项至此

/* FreeRTOS types include */
#include "FreeRTOS_POSIX_types.h"
/**
 * @brief Used for system times in clock ticks or CLOCKS_PER_SEC.
 *
 * Enabled/disabled by posixconfigENABLE_CLOCK_T.
 */
#if !defined( posixconfigENABLE_CLOCK_T ) || ( posixconfigENABLE_CLOCK_T == 1 )
    typedef uint32_t                 clock_t;
#endif

/**
 * @brief Used for clock ID type in the clock and timer functions.
 *
 * Enabled/disabled by posixconfigENABLE_CLOCKID_T.
 */
#if !defined( posixconfigENABLE_CLOCKID_T ) || ( posixconfigENABLE_CLOCKID_T == 1 )
    typedef int                      clockid_t;
#endif

/**
 * @brief Used for some file attributes.
 *
 * Enabled/disabled by posixconfigENABLE_MODE_T.
 */
#if !defined( posixconfigENABLE_MODE_T ) || ( posixconfigENABLE_MODE_T == 1 )
    typedef int                      mode_t;
#endif

/**
 * @brief Used for process IDs and process group IDs.
 *
 * Enabled/disabled by posixconfigENABLE_PID_T.
 */
#if !defined( posixconfigENABLE_PID_T ) || ( posixconfigENABLE_PID_T == 1 )
    typedef int                      pid_t;
#endif

/**
 * @brief Used to identify a thread attribute object.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_ATTR_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_ATTR_T ) || ( posixconfigENABLE_PTHREAD_ATTR_T == 1 )
    typedef PthreadAttrType_t        pthread_attr_t;
#endif

/**
 * @brief Used to identify a barrier.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_BARRIER_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_BARRIER_T ) || ( posixconfigENABLE_PTHREAD_BARRIER_T == 1 )
    typedef PthreadBarrierType_t     pthread_barrier_t;
#endif

/**
 * @brief Used to define a barrier attributes object.
 */
typedef void                         * pthread_barrierattr_t;

/**
 * @brief Used for condition variables.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_COND_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_COND_T ) || ( posixconfigENABLE_PTHREAD_COND_T == 1 )
    typedef  PthreadCondType_t       pthread_cond_t;
#endif

/**
 * @brief Used to identify a condition attribute object.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_CONDATTR_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_CONDATTR_T ) || ( posixconfigENABLE_PTHREAD_CONDATTR_T == 1 )
    typedef void                     * pthread_condattr_t;
#endif

/**
 * @brief Used for mutexes.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_MUTEX_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_MUTEX_T ) || ( posixconfigENABLE_PTHREAD_MUTEX_T == 1 )
    typedef PthreadMutexType_t       pthread_mutex_t;
#endif

/**
 * @brief Used to identify a mutex attribute object.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_MUTEXATTR_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_MUTEXATTR_T ) || ( posixconfigENABLE_PTHREAD_MUTEXATTR_T == 1 )
    typedef PthreadMutexAttrType_t   pthread_mutexattr_t;
#endif

/**
 * @brief Used to identify a thread.
 *
 * Enabled/disabled by posixconfigENABLE_PTHREAD_T.
 */
#if !defined( posixconfigENABLE_PTHREAD_T ) || ( posixconfigENABLE_PTHREAD_T == 1 )
    typedef void                     * pthread_t;
#endif

/**
 * @brief Used for a count of bytes or an error indication.
 *
 * Enabled/disabled by posixconfigENABLE_SSIZE_T.
 */
#if !defined( posixconfigENABLE_SSIZE_T ) || ( posixconfigENABLE_SSIZE_T == 1 )
    typedef int                      ssize_t;
#endif

/**
 * @brief Used for time in seconds.
 *
 * Enabled/disabled by posixconfigENABLE_TIME_T.
 */
#if !defined( posixconfigENABLE_TIME_T ) || ( posixconfigENABLE_TIME_T == 1 )
    typedef int64_t                  time_t;
#endif

/**
 * @brief Used for timer ID returned by timer_create().
 *
 * Enabled/disabled by posixconfigENABLE_TIMER_T.
 */
#if !defined( posixconfigENABLE_TIMER_T ) || ( posixconfigENABLE_TIMER_T == 1 )
    typedef void                     * timer_t;
#endif

/**
 * @brief Used for time in microseconds.
 *
 * Enabled/disabled by posixconfigENABLE_USECONDS_T.
 */
#if !defined( posixconfigENABLE_USECONDS_T ) || ( posixconfigENABLE_USECONDS_T == 1 )
    typedef unsigned long            useconds_t;
#endif

/**
 * @brief Used for file sizes.
 *
 * Enabled/disabled by posixconfigENABLE_OFF_T.
 */
#if !defined( posixconfigENABLE_OFF_T ) || ( posixconfigENABLE_OFF_T == 1 )
    typedef long int                 off_t;
#endif


/* the codes below are writen by xuyunjiang at schoow university */

typedef struct
{
    pthread_cond_t cv;
    pthread_mutex_t resource_mutex;

    /**
     * Number of current readers holding this lock, negative number means waiting readers
     */
    int8_t active_readers;

    uint8_t active_writers;
    uint8_t waiting_writers;
} pthread_rwlock_t;

typedef union
{
  char __size[32];
  long int __align;
} pthread_rwlockattr_t;

//至此结束
#endif  /* !__need_inttypes */

#undef __need_inttypes

#endif	/* _SYS_TYPES_H */
