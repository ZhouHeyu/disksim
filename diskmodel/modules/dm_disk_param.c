
#define STACK_MAX 32
{
    int c;
    int needed = 0;
    int param_stack[STACK_MAX];
    int stack_ptr = 0;  // index of first free slot
    BITVECTOR(paramvec, DM_DISK_MAX_PARAM);
    bit_zero(paramvec, DM_DISK_MAX_PARAM);

    for(c = 0; c < b->params_len; c++) {
	int pnum;
	int i = 0; 
	double d = 0.0; 
	char *s = 0; 
	struct lp_block *blk = 0;
	struct lp_list *l = 0;
	
	if(!b->params[c]) continue;

TOP:
	pnum = lp_param_name(lp_mod_name("dm_disk"), b->params[c]->name);

        // don't initialize things more than once
	if(BIT_TEST(paramvec, pnum)) continue;


	if(stack_ptr > 0) {
	    for(c = 0; c < b->params_len; c++) {
	        if(lp_param_name(lp_mod_name("dm_disk"), b->params[c]->name) == needed)
		    goto FOUND;
            }
	    break;
	}
      FOUND:

	switch(PTYPE(b->params[c])) {
	    case I:  i   = IVAL(b->params[c]); break;
	    case D:  d   = DVAL(b->params[c]); break;
	    case S:  s   = SVAL(b->params[c]); break;
	    case LIST: l = LVAL(b->params[c]); break;
	  default: blk = BVAL(b->params[c]); break;
	}


	switch(pnum) {
case DM_DISK_BLOCK_COUNT:
{
if(!(i >= 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->dm_sectors = i;
}
} break;
case DM_DISK_NUMBER_OF_DATA_SURFACES:
{
if(!(i > 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->dm_surfaces = i;
}
} break;
case DM_DISK_NUMBER_OF_CYLINDERS:
{
if(!(i > 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->dm_cyls = i;
}
} break;
case DM_DISK_MECHANICAL_MODEL:
{
if(!(result->mech = ((dm_mech_loader_t)blk->loader)(blk, result))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
case DM_DISK_LAYOUT_MODEL:
{
if(!BIT_TEST(paramvec, DM_DISK_MECHANICAL_MODEL)) {   param_stack[stack_ptr++] = c;   needed = DM_DISK_MECHANICAL_MODEL; continue;  }if(!(result->layout = ((dm_layout_loader_t)blk->loader)(blk, result))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
    default: assert(0); break; 
    } /* end of switch */ 
    BIT_SET(paramvec, pnum);
    if(stack_ptr > 0) { c = param_stack[--stack_ptr]; goto TOP; }    } /* end of outer for loop */ 

  for(c = 0; c <= DM_DISK_MAX; c++) {
    if(dm_disk_params[c].req && !BIT_TEST(paramvec,c)) {
      fprintf(stderr, "*** error: in DM_DISK spec -- missing required parameter \"%s\"
", dm_disk_params[c].name);
      return 0;
    }
  }
} /* end of scope */ 

 