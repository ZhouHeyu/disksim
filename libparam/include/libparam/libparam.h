
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



#ifndef _LIBPARAM_LIBPARAM_H
#define _LIBPARAM_LIBPARAM_H
#include <stdio.h>
#include <assert.h>

struct lp_block;

#define MAX_PARAM_LEN 1024

struct lp_varspec {
  char *name;
  int type;   /* actually lp_type_t */
  int req;    /* is this variable required? */
};

/* types of config parameters 
 * builtin types have numbers less than 0; modules have numbers >= 0
 */
typedef enum { 
  BLOCK = -1,
  S     = -2,
  I     = -3,
  D     = -4,
  LIST  = -5,
  TOPOSPEC = -6
} lp_type_t;

extern char *lp_builtin_names[];

/* #define MAX_BUILTIN_TYPE TOPOSPEC */

typedef int*(*lp_modloader_t)(struct lp_block *, int);
// #include "modules/modules.h"




struct lp_value;
struct lp_list;

struct lp_list *lp_list_add(struct lp_list *l, 
			    struct lp_value *v);

struct lp_list {
  char *source_file;
  struct lp_value **values;
  /* allocated size of values */
  int values_len; 
};

struct lp_topospec { 
  char *source_file;
  char *type; 
  char *name; 
  struct lp_list *l; 
};

struct lp_value {
  char *source_file;
  union {
    char *s;
    int i;
    double d;
    struct lp_block *b;
    struct lp_list *l;
    struct { struct lp_topospec *l; int len; } t;
  } v;

  lp_type_t t;
};

struct lp_param {
  char *source_file;
  char *name; 
  struct lp_value *v;
};

/* super is NULL for builtin types, e.g. DEVICEBLOCK */
struct lp_subtype {
  char *super;
  char *sub;
  struct lp_block *spec; /* parse tree for sub */
};


struct lp_inst {
  char *source_file;
  struct lp_list *l;
  char *name;
};


// "tlt == Top Level Thing"
struct lp_tlt {
  char *source_file;
  enum {
    TLT_BLOCK,
    TLT_TOPO,
    TLT_INST
  } what;

  union {
    struct lp_inst *inst;
    struct lp_block *block;
    struct lp_topospec *topo;
  } it;
};


/* map subtype names to parent types */
struct lp_subtype **lp_typetbl;
int lp_typetbl_len;

struct lp_tlt **lp_tlts;
int lp_tlts_len;

struct lp_tlt *lp_new_tl_topo(struct lp_topospec *, char *);
struct lp_tlt *lp_new_tl_inst(struct lp_inst *, char *);
struct lp_tlt *lp_new_tl_block(struct lp_block *, char *);

void lp_add_tlt(struct lp_tlt *);

void lp_init_typetbl(void);

void lp_release_typetbl(void);

int lp_add_type(char *newt, char *parent);
char *lp_lookup_type(char *name, int *);
char *lp_lookup_base_type(char *name, int *n);

/* apply these macros to a struct lp_param *   */
#define PTYPE(x) ((x)->v->t)
#define IVAL(x) ((x)->v->v.i)
#define DVAL(x) ((x)->v->v.d)
#define SVAL(x) ((x)->v->v.s)
#define BVAL(x) ((x)->v->v.b)
#define LVAL(x) ((x)->v->v.l)

/* extract the type of a block.  apply this to a struct lp_block *  */
#define BTYPE(x) ((x)->type)

/* some handy macros for doing dumb stuff */

/* is x in [y,z] */
#define RANGE(x,y,z) (((x) >= (y)) && ((x) <= (z)))
#define BADVALMSG(N) fprintf(stderr, "*** error: Bad value for %s\n", N);


void load_block(struct lp_block *);
void load_topo(struct lp_topospec *t, int);

// client must provide one of these
typedef void(*lp_topoloader_t)(struct lp_topospec *t, int);
void lp_register_topoloader(lp_topoloader_t);


struct lp_block_val {
  struct lp_param **params;
  int params_len;
};

/* need mod_t */
struct lp_block {
  char *source_file;
  char *name;
  int type;
  lp_modloader_t loader;
  
  struct lp_param **params;

  int params_len;

};



void push_file(FILE *f);
int disksim_paramwrap(void);
#define MAX_FILE_DEPTH 32
extern struct { 
    struct yy_buffer_state *b; 
    int lineno; 
    char *filename;
} input_files[];

extern int top_file;
extern FILE *libparamin;
extern struct yy_buffer_state *yy_current_buffer;
extern char *lp_filename;
extern int lp_lineno;


int *
lp_override_inst(struct lp_block *b, 
	      char *cname, 
	      lp_modloader_t loader,
	      char **overrides,
	      int overrides_len);


typedef void(*lp_load_callback)(void *, int *);

// a module description
struct lp_mod {
  // unique; used in input files
  char *name; 

  // the variable specs for this module
  struct lp_varspec *modvars;
  int modvars_len;
  
  // loader function
  lp_modloader_t fn;

  // when we successfully instantiate an object of this type, 
  // we will call <callback>(ctx, ptr) where ptr is a pointer to the object
  // why not do this in the loader?  because the loader might come from
  // libdiskmodel and as a user of it, you want to do something with
  // each dm_disk without modifying the dm code itself.
  lp_load_callback callback;
  void *ctx;
};

extern struct lp_mod **lp_modules;

// register a module
// returns -1 on error

int lp_register_module(struct lp_mod *);


// extern FILE *outputfile;



// i.e. disksim command-line parameter overrides
extern char **lp_overrides;
extern int lp_overrides_len;


void unparse_param(struct lp_param *p, FILE *outfile);
void unparse_list(struct lp_list *l, FILE *outfile);
void unparse_block(struct lp_block *b, FILE *outfile);
void unparse_value(struct lp_value *v, FILE *outfile);
void unparse_topospec(struct lp_topospec *t, FILE *outfile);
void unparse_type(int, FILE *);
void lp_unparse_tlts(struct lp_tlt **, int, FILE *, char *);


// int disksim_loadparams(char *inputfile);

struct lp_block *lp_lookup_spec(char *name);


int lp_setup_subtype(struct lp_block *parent,
		     struct lp_block *child);

int lp_add_param(struct lp_param ***b, int *plen,
		 struct lp_param *p);



int lp_param_name(int, char *);
int lp_mod_name(char *);

// puts a pointer to an array of top level things into the 2nd argument,
// the length of that array in the 3rd arg
// 5th arg is an array of overrides (devspec, paramname, newval)
// 6th arg is number of overrides
int lp_loadfile(FILE *in, struct lp_tlt ***, int *, char *, char **, int);

// free the parse tree referenced by pt containing len tlts
void lp_destroy(struct lp_tlt **pt, int len);

#endif // _LIBPARAM_LIBPARAM_H





