#ifndef OS_STATUS_H
#define OS_STATUS_H

#define SUCCESS     0
#define EINVARG     1
#define ENOMEM      2


/* FS ERROR */
#define EBADPATH                100
#define EIO                     101
#define EFSNOTUS                102
#define EFNOTFOUND              103

/* FS ERROR */

/* TASK ERROR */
#define EISTAKEN                1000
#define ENOTASK                 1001
#define EMALLOC                 1002
/* TASK ERROR */


#define EUELF                   2000
#define EELFMAGIC               2001
#define ENOEXEC                 2002
#endif