
/* libparam (version 1.0)
 * Authors: John Bucy, Greg Ganger
 * Contributors: John Griffin, Jiri Schindler, Steve Schlosser
 *
 * Copyright (c) of Carnegie Mellon University, 2001, 2002, 2003.
 *
 * This software is being provided by the copyright holders under the
 * following license. By obtaining, using and/or copying this
 * software, you agree that you have read, understood, and will comply
 * with the following terms and conditions:
 *
 * Permission to reproduce, use, and prepare derivative works of this
 * software is granted provided the copyright and "No Warranty"
 * statements are included with all reproductions and derivative works
 * and associated documentation. This software may also be
 * redistributed without charge provided that the copyright and "No
 * Warranty" statements are included in all redistributions.
 *
 * NO WARRANTY. THIS SOFTWARE IS FURNISHED ON AN "AS IS" BASIS.
 * CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER
 * EXPRESSED OR IMPLIED AS TO THE MATTER INCLUDING, BUT NOT LIMITED
 * TO: WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY
 * OF RESULTS OR RESULTS OBTAINED FROM USE OF THIS SOFTWARE. CARNEGIE
 * MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH
 * RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT
 * INFRINGEMENT.  COPYRIGHT HOLDERS WILL BEAR NO LIABILITY FOR ANY USE
 * OF THIS SOFTWARE OR DOCUMENTATION.  
 */




#ifndef _BITVECTOR_H
#define _BITVECTOR_H

/* y is the name, x is the number of bits */

#define BITVECTOR(y,x) char y[(x / 8) + 1]

/* test the yth bit of x */
#define BIT_TEST(x,y) (x[y / 8] & (1 << (y % 8)))

/* turn on the yth bit of x */
#define BIT_SET(x,y) x[y / 8] |= (1 << (y % 8))

/* turn off the yth bit of x */
#define BIT_RESET(x,y) x[y / 8] &= ~(1 << (y % 8))

static void bit_zero(char *v, int len) {
  memset(v, 0, (len / 8) + 1); 
}

static void bit_setall(char *v, int len) {
  memset(v, 0xff, (len / 8) + 1);
}

#endif // _BITVECTOR_H
