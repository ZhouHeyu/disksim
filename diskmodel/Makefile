
# diskmodel (version 1.0)
# Authors: John Bucy, Greg Ganger
# Contributors: John Griffin, Jiri Schindler, Steve Schlosser
#
# Copyright (c) of Carnegie Mellon University, 2001, 2002, 2003.
#
# This software is being provided by the copyright holders under the
# following license. By obtaining, using and/or copying this
# software, you agree that you have read, understood, and will comply
# with the following terms and conditions:
#
# Permission to reproduce, use, and prepare derivative works of this
# software is granted provided the copyright and "No Warranty"
# statements are included with all reproductions and derivative works
# and associated documentation. This software may also be
# redistributed without charge provided that the copyright and "No
# Warranty" statements are included in all redistributions.
#
# NO WARRANTY. THIS SOFTWARE IS FURNISHED ON AN "AS IS" BASIS.
# CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER
# EXPRESSED OR IMPLIED AS TO THE MATTER INCLUDING, BUT NOT LIMITED
# TO: WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY
# OF RESULTS OR RESULTS OBTAINED FROM USE OF THIS SOFTWARE. CARNEGIE
# MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH
# RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT
# INFRINGEMENT.  COPYRIGHT HOLDERS WILL BEAR NO LIABILITY FOR ANY USE
# OF THIS SOFTWARE OR DOCUMENTATION.  


CC=gcc 

# must provide LIBPARAM_ and LIBDDBG_ vars
include .paths

# -D_DISKMODEL_FREEBSD for the freebsd kernel (malloc foo)
# -D_BSD_SOURCE for bsd-ish userspace to get uint32 and friends from the
# right place
CFLAGS = -g -I. $(LIBPARAM_CFLAGS) $(LIBDDBG_CFLAGS) -D_DM_SOURCE -Wall -Wno-unused  -MD # -D_DISKMODEL_FREEBSD

CC-DEP = $(CC)
CP = cp -p
all: modules libdiskmodel.a
	mkdir -p lib include/diskmodel/modules
	$(CP) libdiskmodel.a lib
	$(CP) dm.h dm_types.h dm_config.h marshall.h include/diskmodel
	$(CP) modules/*.h include/diskmodel/modules


.PHONY: modules
modules/modules.h modules: modules/*.modspec
	$(MAKE) -C modules

modules/modules.h modules/*.h modules/*.c: modules

-include *.d

DM_SRC = mech_g1_seektime.c mech_g1.c \
	 layout_g1.c sqrt.c marshall.c

# source for libparam loaders; separate for things like kernel
# which don't have libparam 
DM_USERSPACE_SRC = mech_g1_load.c layout_g1_load.c dm_load.c convert.c

DM_OBJ = $(DM_SRC:.c=.o) 

DM_USERSPACE_OBJ = $(DM_USERSPACE_SRC:.c=.o)

# for userspace
libdiskmodel.a:	$(DM_OBJ) $(DM_USERSPACE_OBJ) modules
	@echo "Linking libdiskmodel.a $(DM_USERSPACE_OBJ)"
	ar cru $@ $(DM_OBJ) $(DM_USERSPACE_OBJ)

# for kernel
#libdiskmodel.a: $(DM_OBJ) modules
#	ar cru $@ $(DM_OBJ)


g3_test: g3_load.o g3.o g3_test.o splaytree.o pbn.o
	$(CC) -o $@ $^

g3_repeat_test: g3_load.o g3.o g3_repeat_test.o splaytree.o pbn.o
	$(CC) -o $@ $^

perm_test: perm_test.o g3.o splaytree.o pbn.o
	$(CC) -o $@ $^

dm_testsuite.o: modules/modules.h

dm_testsuite:	dm_testsuite.o libdiskmodel.a
	$(CC) -o $@ $^ -L. -ldiskmodel $(LIBPARAM_LDFLAGS) $(LIBDDBG_LDFLAGS) -lm

eno-bug:	eno-bug.o libdiskmodel.a
	$(CC) -o $@ $^ -L. -ldiskmodel $(LIBPARAM_LDFLAGS) $(LIBDDBG_LDFLAGS) -lm

extract_zonefoo: 	extract_zonefoo.o
	$(CC) -o $@ $^  $(LIBPARAM_LDFLAGS) $(LIBDDBG_LDFLAGS)

clean:
	rm -f *.o libdiskmodel.a dm_testsuite
	rm -rf lib include 
	$(MAKE) -C modules clean

distclean: clean
	rm -f *.d

