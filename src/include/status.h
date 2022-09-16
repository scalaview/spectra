#ifndef OS_STATUS_H
#define OS_STATUS_H


#define EINVARG     1
#define ENOMEM      2
#define EBADPATH    3
#define EIO         4


#define ISERR(value) ((int)value < 0)

#endif