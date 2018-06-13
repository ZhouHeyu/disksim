
#define STACK_MAX 32
{
    int c;
    int needed = 0;
    int param_stack[STACK_MAX];
    int stack_ptr = 0;  // index of first free slot
    BITVECTOR(paramvec, DM_MECH_G1_MAX_PARAM);
    bit_zero(paramvec, DM_MECH_G1_MAX_PARAM);

    for(c = 0; c < b->params_len; c++) {
	int pnum;
	int i = 0; 
	double d = 0.0; 
	char *s = 0; 
	struct lp_block *blk = 0;
	struct lp_list *l = 0;
	
	if(!b->params[c]) continue;

TOP:
	pnum = lp_param_name(lp_mod_name("dm_mech_g1"), b->params[c]->name);

        // don't initialize things more than once
	if(BIT_TEST(paramvec, pnum)) continue;


	if(stack_ptr > 0) {
	    for(c = 0; c < b->params_len; c++) {
	        if(lp_param_name(lp_mod_name("dm_mech_g1"), b->params[c]->name) == needed)
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
case DM_MECH_G1_ACCESS_TIME_TYPE:
{
 if(!strcmp(s, "constant")) { }
 else if(!strcmp(s, "averageRotation")) {
 result->acctime = AVGROTATE;
 }
 else if(!strcmp(s, "trackSwitchPlusRotation")) {
 result->acctime = SKEWED_FOR_TRACK_SWITCH;
 }
 else {
 fprintf(stderr, "*** error: Unknown access time type: %s\n", s);
 return 0;
 }
} break;
case DM_MECH_G1_CONSTANT_ACCESS_TIME:
{
if(!(d >= 0.0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->acctime = d;
}
} break;
case DM_MECH_G1_SEEK_TYPE:
{
 if(!strcmp(s, "constant")) {
 result->seektime = SEEK_CONST;
 }
 else if(!strcmp(s, "linear")) {
 result->seektime = SEEK_3PT_LINE;
 }
 else if(!strcmp(s, "curve")) {
 result->seektime = SEEK_3PT_CURVE;
 }
 else if(!strcmp(s, "hpl")) {
 result->seektime = SEEK_HPL;
 }
 else if(!strcmp(s, "hplplus10")) {
 result->seektime = SEEK_1ST10_PLUS_HPL;
 }
 else if(!strcmp(s, "extracted")) {
 result->seektime = SEEK_EXTRACTED;
 }
 else {
 fprintf(stderr, "*** error: Unknown seek type: %s\n", s);
 return 0;
 }
 result->seekfn = dm_mech_g1_seekfns[result->seektime];
} break;
case DM_MECH_G1_AVERAGE_SEEK_TIME:
{
if(!(d >= 0.0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->seekavg = d;
}
} break;
case DM_MECH_G1_CONSTANT_SEEK_TIME:
{
if(!(d >= 0.0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->seektime = d;
}
} break;
case DM_MECH_G1_SINGLE_CYLINDER_SEEK_TIME:
{
if(!(d >= 0.0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->seekone = d;
}
} break;
case DM_MECH_G1_FULL_STROBE_SEEK_TIME:
{
if(!(d >= 0.0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->seekfull = d;
}
} break;
case DM_MECH_G1_FULL_SEEK_CURVE:
{
if(!BIT_TEST(paramvec, DM_MECH_G1_SEEK_TYPE)) {   param_stack[stack_ptr++] = c;   needed = DM_MECH_G1_SEEK_TYPE; continue;  } result->seektime = SEEK_EXTRACTED;
 dm_mech_g1_read_extracted_seek_curve(s,
	 &result->xseekcnt,
	 &result->xseekdists,
	 &result->xseektimes);

} break;
case DM_MECH_G1_ADD_WRITE_SETTLING_DELAY:
{
 result->seekwritedelta = dm_time_dtoi(d);
} break;
case DM_MECH_G1_HEAD_SWITCH_TIME:
{
if(!(d >= 0.0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->headswitch = dm_time_dtoi(d);
}
} break;
case DM_MECH_G1_ROTATION_SPEED_:
{
if(!(i > 0)) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->rpm = i;
}
} break;
case DM_MECH_G1_PERCENT_ERROR_IN_RPMS:
{
if(!(RANGE(d, 0.0, 100.0))) { BADVALMSG(b->params[c]->name); return 0; }
else {
 result->rpmerr = d;
}
} break;
case DM_MECH_G1_FIRST_TEN_SEEK_TIMES:
{
if(!BIT_TEST(paramvec, DM_MECH_G1_SEEK_TYPE)) {   param_stack[stack_ptr++] = c;   needed = DM_MECH_G1_SEEK_TYPE; continue;  }if(!(!do_1st10_seeks(result,l))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
case DM_MECH_G1_HPL_SEEK_EQUATION_VALUES:
{
if(!BIT_TEST(paramvec, DM_MECH_G1_SEEK_TYPE)) {   param_stack[stack_ptr++] = c;   needed = DM_MECH_G1_SEEK_TYPE; continue;  }if(!(!do_hpl_seek(result,l))) { BADVALMSG(b->params[c]->name); return 0; }
} break;
    default: assert(0); break; 
    } /* end of switch */ 
    BIT_SET(paramvec, pnum);
    if(stack_ptr > 0) { c = param_stack[--stack_ptr]; goto TOP; }    } /* end of outer for loop */ 

  for(c = 0; c <= DM_MECH_G1_MAX; c++) {
    if(dm_mech_g1_params[c].req && !BIT_TEST(paramvec,c)) {
      fprintf(stderr, "*** error: in DM_MECH_G1 spec -- missing required parameter \"%s\"
", dm_mech_g1_params[c].name);
      return 0;
    }
  }
} /* end of scope */ 

 