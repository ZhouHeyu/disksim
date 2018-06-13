
#define STACK_MAX 32
{
    int c;
    int needed = 0;
    int param_stack[STACK_MAX];
    int stack_ptr = 0;  // index of first free slot
    BITVECTOR(paramvec, DM_LAYOUT_G1_ZONE_MAX_PARAM);
    bit_zero(paramvec, DM_LAYOUT_G1_ZONE_MAX_PARAM);

    for(c = 0; c < b->params_len; c++) {
	int pnum;
	int i = 0; 
	double d = 0.0; 
	char *s = 0; 
	struct lp_block *blk = 0;
	struct lp_list *l = 0;
	
	if(!b->params[c]) continue;

TOP:
	pnum = lp_param_name(lp_mod_name("dm_layout_g1_zone"), b->params[c]->name);

        // don't initialize things more than once
	if(BIT_TEST(paramvec, pnum)) continue;


	if(stack_ptr > 0) {
	    for(c = 0; c < b->params_len; c++) {
	        if(lp_param_name(lp_mod_name("dm_layout_g1_zone"), b->params[c]->name) == needed)
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
case DM_LAYOUT_G1_ZONE_FIRST_CYLINDER_NUMBER:
{
if(!(i >= 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->startcyl = i;
}
} break;
case DM_LAYOUT_G1_ZONE_LAST_CYLINDER_NUMBER:
{
if(!(i >= 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->endcyl = i;
}
} break;
case DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK:
{
if(!(i > 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->blkspertrack = i;
}
 result->sector_width = ((long long)1 << 32) / result->blkspertrack;
} break;
case DM_LAYOUT_G1_ZONE_OFFSET_OF_FIRST_BLOCK:
{
if(!BIT_TEST(paramvec, DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK)) {   param_stack[stack_ptr++] = c;   needed = DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK; continue;  } {
 skew_unit_t skewtmp = NONE;
 if(skew_units != NONE) skewtmp = skew_units;
 else if(layout_skew_units != NONE) skewtmp = layout_skew_units;
 switch(skewtmp) {
 case REVOLUTIONS:
 result->firstblkangle = dm_angle_dtoi(d);
 break;
 case NONE:
 case SECTORS:
 default:
 result->firstblkangle = (int)d * result->sector_width;
 break;
 }
 }
} break;
case DM_LAYOUT_G1_ZONE_SKEW_UNITS:
{
 if(!strcmp(s, "revolutions")) { skew_units = REVOLUTIONS; }
 else if(!strcmp(s, "sectors")) { skew_units = SECTORS; }
 else { ddbg_assert(0); }
} break;
case DM_LAYOUT_G1_ZONE_EMPTY_SPACE_AT_ZONE_FRONT:
{
 result->deadspace = i;
} break;
case DM_LAYOUT_G1_ZONE_SKEW_FOR_TRACK_SWITCH:
{
if(!BIT_TEST(paramvec, DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK)) {   param_stack[stack_ptr++] = c;   needed = DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK; continue;  } {
 skew_unit_t skewtmp = NONE;
 if(skew_units != NONE) skewtmp = skew_units;
 else if(layout_skew_units != NONE) skewtmp = layout_skew_units;
 switch(skewtmp) {
 case REVOLUTIONS:
 result->trackskew = dm_angle_dtoi(d);
 break;
 case NONE:
 case SECTORS:
 default:
 result->trackskew = (int)d * result->sector_width;
 break;
 }
 }
} break;
case DM_LAYOUT_G1_ZONE_SKEW_FOR_CYLINDER_SWITCH:
{
if(!BIT_TEST(paramvec, DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK)) {   param_stack[stack_ptr++] = c;   needed = DM_LAYOUT_G1_ZONE_BLOCKS_PER_TRACK; continue;  } {
 skew_unit_t skewtmp = NONE;
 if(skew_units != NONE) skewtmp = skew_units;
 else if(layout_skew_units != NONE) skewtmp = layout_skew_units;
 switch(skewtmp) {
 case REVOLUTIONS:
 result->cylskew = dm_angle_dtoi(d);
 break;
 case NONE:
 case SECTORS:
 default:
 result->cylskew = (int)d * result->sector_width;
 break;
 }
 }
} break;
case DM_LAYOUT_G1_ZONE_NUMBER_OF_SPARES:
{
 result->sparecnt = i;
} break;
case DM_LAYOUT_G1_ZONE_SLIPS:
{
if(!(!getslips(result, l))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
case DM_LAYOUT_G1_ZONE_DEFECTS:
{
if(!(!getdefects(result, l))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
    default: assert(0); break; 
    } /* end of switch */ 
    BIT_SET(paramvec, pnum);
    if(stack_ptr > 0) { c = param_stack[--stack_ptr]; goto TOP; }    } /* end of outer for loop */ 

  for(c = 0; c <= DM_LAYOUT_G1_ZONE_MAX; c++) {
    if(dm_layout_g1_zone_params[c].req && !BIT_TEST(paramvec,c)) {
      fprintf(stderr, "*** error: in DM_LAYOUT_G1_ZONE spec -- missing required parameter \"%s\"
", dm_layout_g1_zone_params[c].name);
      return 0;
    }
  }
} /* end of scope */ 

 