/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : standards
 * @created     : Monday Sep 30, 2019 22:05:19 MDT
 */

#ifndef STANDARDS_H

#define STANDARDS_H

/**
 * This is a header where we define
 * the standard library functions
 * in our own code. No dependencies
 * and better implimentation
 */

#include <stddef.h>

void*
memcpy_rmt(void* destination, const void* src, size_t len);

#endif /* end of include guard STANDARDS_H */
