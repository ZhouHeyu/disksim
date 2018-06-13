
#define STACK_MAX 32
{
    int c;
    int needed = 0;
    int param_stack[STACK_MAX];
    int stack_ptr = 0;  // index of first free slot
    BITVECTOR(paramvec, DM_LAYOUT_G1_MAX_PARAM);
    bit_zero(paramvec, DM_LAYOUT_G1_MAX_PARAM);

    for(c = 0; c < b->params_len; c++) {
	int pnum;
	int i = 0; 
	double d = 0.0; 
	char *s = 0; 
	struct lp_block *blk = 0;
	struct lp_list *l = 0;
	
	if(!b->params[c]) continue;

TOP:
	pnum = lp_param_name(lp_mod_name("dm_layout_g1"), b->params[c]->name);

        // don't initialize things more than once
	if(BIT_TEST(paramvec, pnum)) continue;


	if(stack_ptr > 0) {
	    for(c = 0; c < b->params_len; c++) {
	        if(lp_param_name(lp_mod_name("dm_layout_g1"), b->params[c]->name) == needed)
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
case DM_LAYOUT_G1_LBN_TO_PBN_MAPPING_SCHEME:
{
if(!(RANGE(i,0,LAYOUT_MAX))) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->mapping = i;
}
} break;
case DM_LAYOUT_G1_SPARING_SCHEME_USED:
{
if(!(RANGE(i,NO_SPARING,MAXSPARESCHEME))) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->sparescheme = i;
}
} break;
case DM_LAYOUT_G1_RANGESIZE_FOR_SPARING:
{
if(!(i > 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->rangesize = i;
}
} break;
case DM_LAYOUT_G1_SKEW_UNITS:
{
 if(!strcmp(s, "revolutions")) { skew_units = REVOLUTIONS; }
 else if(!strcmp(s, "sectors")) { skew_units = SECTORS; }
 else { ddbg_assert(0); }
} break;
case DM_LAYOUT_G1_ZONES:
{
if(!(disk_load_zones(l,result,skew_units))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
    default: assert(0); break; 
    } /* end of switch */ 
    BIT_SET(paramvec, pnum);
    if(stack_ptr > 0) { c = param_stack[--stack_ptr]; goto TOP; }    } /* end of outer for loop */ 

  for(c = 0; c <= DM_LAYOUT_G1_MAX; c++) {
    if(dm_layout_g1_params[c].req && !BIT_TEST(paramvec,c)) {
      fprintf(stderr, "*** error: in DM_LAYOUT_G1 spec -- missing required parameter \"%s\"
", dm_layout_g1_params[c].name);
      return 0;
    }
  }
} /* end of scope */ 

 