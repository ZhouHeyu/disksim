
/* libddbg (version 1.0)
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



/* libddbg (version 1.0)
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


#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


#include "libddbg.h"
#include <bitvector.h>

struct lt_class {
  char *name;
  BITVECTOR(instances, LT_MAX_INSTANCE);
};

static struct lt_class *lt_classes = 0;
static int lt_classes_len = 0;
static int lt_class_max = 0;
static FILE *lt_tracefile = 0;

// register a new class.  Returns an int class id to be used with
// subsequent calls
int lt_register(char *classname) {

  if(lt_class_max < lt_classes_len) {
    goto found;
  }

  lt_classes_len *= 2;
  lt_classes_len++;
  lt_classes = realloc(lt_classes, lt_classes_len * sizeof(struct lt_class));
  
 found:
  lt_classes[lt_class_max].name = strdup(classname);
  bit_zero(lt_classes[lt_class_max].instances, LT_MAX_INSTANCE);
  lt_class_max++;

  return lt_class_max - 1;
}

// start logging (class,instance) traces.  -1 is the wildcard which logs
// all instances from that class.
void lt_enable(int class, int instance) {
  ddbg_assert(class < lt_class_max);
  ddbg_assert((instance < LT_MAX_INSTANCE) || (instance == -1));
  if(instance != -1) {
    BIT_SET(lt_classes[class].instances, instance);
  }
  else {
    bit_setall(lt_classes[class].instances, LT_MAX_INSTANCE);
  }
}

// stop logging (class,instance) traces.  -1 is the wildcard which
// squelches all instances from that class.
void lt_disable(int class, int instance) {
  ddbg_assert(class < lt_class_max);
  ddbg_assert((instance < LT_MAX_INSTANCE) || (instance == -1));
  if(instance != -1) {
    BIT_RESET(lt_classes[class].instances, instance);
  }
  else {
    bit_zero(lt_classes[class].instances, LT_MAX_INSTANCE);
  }
}



void lt_trace(int class, int instance, char *fmt, ...)
{
  ddbg_assert(class < lt_class_max);
  ddbg_assert(instance < LT_MAX_INSTANCE);

#ifndef _LIBDDBG_FREEBSD
 
  if(BIT_TEST(lt_classes[class].instances, instance) && lt_tracefile) {
    va_list ap;

    fprintf(lt_tracefile, "(%s,%d): ", lt_classes[class].name, instance);
    
    va_start(ap, fmt);
    vfprintf(lt_tracefile, fmt, ap);

    fprintf(lt_tracefile, "\n");
  }
#endif // _LIBDDBG_FREEBSD
}




void lt_setfile(FILE *f) {
  lt_tracefile = f;
}
