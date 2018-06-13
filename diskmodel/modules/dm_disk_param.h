#include <libparam/libparam.h>

#ifndef _DM_DISK_PARAM_H
#define _DM_DISK_PARAM_H  


/* prototype for dm_disk param loader function */
   struct dm_disk_if *dm_disk_loadparams(struct lp_block *b, int *num);

typedef enum {
   DM_DISK_BLOCK_COUNT,
   DM_DISK_NUMBER_OF_DATA_SURFACES,
   DM_DISK_NUMBER_OF_CYLINDERS,
   DM_DISK_MECHANICAL_MODEL,
   DM_DISK_LAYOUT_MODEL
} dm_disk_param_t;

#define DM_DISK_MAX_PARAM		DM_DISK_LAYOUT_MODEL


static struct lp_varspec dm_disk_params [] = {
   {"Block count", I, 1 },
   {"Number of data surfaces", I, 1 },
   {"Number of cylinders", I, 1 },
   {"Mechanical Model", BLOCK, 1 },
   {"Layout Model", BLOCK, 1 },
   {0,0,0}
};
#define DM_DISK_MAX 5
static struct lp_mod dm_disk_mod = { "dm_disk", dm_disk_params, DM_DISK_MAX, (lp_modloader_t)dm_disk_loadparams,  0,0 };
#endif // _DM_DISK_PARAM_H
