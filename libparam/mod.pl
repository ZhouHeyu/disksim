#!/usr/bin/perl

# libparam (version 1.0)
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

$modctr = 0;
@paramlines = ();
$startenum = 0;
$linenum = 0;
$package = $ARGV[0];
$PACKAGE = $package; $PACKAGE =~ tr/a-z/A-Z/;
$filename = $ARGV[1];
open(FILE, "$filename") || die("couldn't open $filename!");




sub mkname {
    $_ = $_[0];
    # upcase
    tr/a-z/A-Z/;
    # replace whitespace and dashes with underscores
    tr/ \t-/_/;
    s/[\/\\\.]//g;
    # eat things in parens
    s/\(.*\)//g;
    # eat trailing whitespace
    s/[ \t]*$//;
    # eat leading whitespace
    s/^[ \t]*//;
    
    return "$_[1]" . "_" . "$_";
}

sub print_head {
# print out loop, switch statement, var setup, etc


print CODE "
#define STACK_MAX 32
{
    int c;
    int needed = 0;
    int param_stack[STACK_MAX];
    int stack_ptr = 0;  // index of first free slot
    BITVECTOR(paramvec, $_[1]","_MAX_PARAM);
    bit_zero(paramvec, $_[1]","_MAX_PARAM);

    for(c = 0; c < b->params_len; c++) {
	int pnum;
	int i = 0; 
	double d = 0.0; 
	char *s = 0; 
	struct lp_block *blk = 0;
	struct lp_list *l = 0;
	
	if(!b->params[c]) continue;

TOP:
	pnum = lp_param_name(lp_mod_name(\"$_[0]\"), b->params[c]->name);

        // don't initialize things more than once
	if(BIT_TEST(paramvec, pnum)) continue;


	if(stack_ptr > 0) {
	    for(c = 0; c < b->params_len; c++) {
	        if(lp_param_name(lp_mod_name(\"$_[0]\"), b->params[c]->name) == needed)
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
";



}

sub startDoc {
#    print DOC "\\begin{tabular}{|l|l|l|l|}\n";
}

sub endDoc {
##print DOC "}\\\\ \n"; # end the last multicolumn
##print DOC "\\cline{1-4}\n";
    endParamDoc();

#    print DOC "\\end{tabular}\\\\ \n";
}


sub startParamDoc {
    my $tmpmodname = $_[0]; $tmpmodname =~ s/_/\\_/g;
    my $tmpname = $_[1]; $tmpname =~ s/_/\\_/g; 
    my $type = $_[2];
    my $req = $_[3];
    
    @paramDocLines = ();

    $type =~ s/LIST/list/;
    $type =~ s/BLOCK/block/;
    $type =~ s/I/int/;
    $type =~ s/D/float/;
    $type =~ s/S/string/;

    $req =~ s/1/required/;
    $req =~ s/0/optional/;

    print DOC "\\noindent \n";
    print DOC "\\begin{tabular}{|p{1.5in}|p{3.5in}|p{0.5in}|p{0.5in}|}\n";

    print DOC "\\cline{1-4}\n";
    print DOC "\\texttt{$tmpmodname} & \\texttt{$tmpname} & $type & $req \\\\ \n";
    print DOC "\\cline{1-4}\n";
}

sub fillParamDoc {
    $tmp = $_[0];
    $tmp =~ s/_/\_/g;
    if(!($tmp =~ /^[ \t]*$/)) {
	push(@paramDocLines,$tmp);
    }
}

sub endParamDoc {
    if($#paramDocLines >= 0) {
	print DOC "\\multicolumn{4}{|p{6in}|}{\n";
	foreach $l (@paramDocLines) {
	    print DOC "$l\n";
	}
	print DOC "}\\\\ \n\\cline{1-4}\n\\multicolumn{4}{p{5in}}{}\\\\\n";
    }
    print DOC "\\end{tabular}\\\\ \n";
}




while(<FILE>) {
    $linenum++;
    chomp();
    # eat comments
    s/#.*//;

    # eat trailing whitespace
    s/[ \t]*$//;

    # eat leading whitespace
    s/^[ \t]*//;

    # mash whitespace together
    s/\t+/\t/g;
    s/ +/ /g;

    # eat empty lines
    if(/^[ \t]*$/) { next; }




    if(/MODULE[ \t]+(.*)/) {
	$_ = $1;
        $_ = join("_", ($package, $_));
	tr/-/_/;
	open(HEADER, ">$_"."_param.h");
	open(CODE, ">$_"."_param.c");
        open(DOC, ">$_"."_param.tex");
	$modname = $_;
	tr/a-z/A-Z/;
	s/ \t/_/g;
	$MODNAME = $_;


        print HEADER "#include <libparam/libparam.h>\n";
	print HEADER "\n#ifndef _" . $MODNAME ."_PARAM_H\n";
	print HEADER "#define _" .  $MODNAME ."_PARAM_H  \n\n";


	startDoc();

        print_head($modname, $MODNAME);
    }
    elsif(/PROTO (.*)/) {
	print HEADER "\n/* prototype for $modname param loader function */\n";
	print HEADER "   $1\n\n";
    }
    elsif(/PARAM (.*)/) {

	if(!$startenum) { print HEADER "typedef enum {\n"; $startenum = 1; }
	$closeEnum = 1;
	$elseopen = 0;
	# close off case block
	if($modctr) { print CODE "} break;\n"; }

	($name,$type,$req) = split(/\t+/, $1);
	$NAME = mkname($name, $MODNAME);

	print HEADER "   $NAME,\n";
	print CODE "case $NAME:\n{\n";
#	print CODE "printf(\"$NAME\\n\");" ;
	$modctr++;
	$closeTest = 0;

        # close off the previous multicolumn if there was one
        if($modctr > 1) { 
	    endParamDoc();
        }

	startParamDoc($modname, $name, $type, $req);
	
	push(@paramlines, "{\"$name\", $type, $req }");
    }
    elsif(/TEST (.*)/) {
#	print CODE "#line $linenum \"modules/$filename\"\n";
	print CODE "if(!($1)) { BADVALMSG(b->params[c]->name); return 0; }\n";

	$closeTest = 1;
    }
    elsif(/INIT(.*)/) {
	$stuff = $1;
	if($closeTest && ! $elseopen) {
	    print CODE "else {\n";
	}
#	print CODE "#line $linenum \"modules/$filename\"\n";
	print CODE "$stuff\n";
	if($closeTest && ! $elseopen) { print CODE "}\n"; }
	$elseopen = 1;
    }
    elsif(/DEPEND (.*)/) {
	$PARAMNAME = mkname($1, $MODNAME);
	print CODE "if(!BIT_TEST(paramvec, $PARAMNAME)) { ";
	print CODE "  param_stack[stack_ptr++] = c; ";
	print CODE "  needed = $PARAMNAME; continue;  }";
    }
    else {
	if(!/^[ \t]*$/) {
	    fillParamDoc($_);
	}
    }

}




if($modctr) { 
    # close off the last case
    print CODE "} break;\n"; 

    # nix the trailing comma -- ssh hack!
    seek(HEADER, (tell(HEADER) - 2), SEEK_SET);
}


if($closeEnum) {
    $typename = join("_", ($modname, "param_t"));
    print HEADER "\n} $typename;\n\n";
}

print HEADER "#define $MODNAME"."_MAX_PARAM\t\t$NAME\n";
# print HEADER "#include <libparam.h>\n";
$aname = join("_", ($modname, "params"));
print HEADER "\n\nstatic struct lp_varspec $aname [] = {\n";

foreach (@paramlines) {
    print HEADER "   $_,\n";
    $c++;
}
print HEADER "   {0,0,0}\n};\n";


$maxstr = join("_", ($MODNAME, "MAX"));

print CODE "    default: assert(0); break; \n";
print CODE "    } /* end of switch */ \n";
print CODE "    BIT_SET(paramvec, pnum);\n";
print CODE "    if(stack_ptr > 0) { c = param_stack[--stack_ptr]; goto TOP; }";
print CODE "    } /* end of outer for loop */ \n";
print CODE "
  for(c = 0; c <= $maxstr; c++) {
    if($aname"."[c].req && !BIT_TEST(paramvec,c)) {
      fprintf(stderr, \"*** error: in $MODNAME spec -- missing required parameter \\\"%s\\\"\n\", $aname"."[c].name);
      return 0;
    }
  }
";


print CODE "} /* end of scope */ \n\n ";



print HEADER "#define $maxstr $modctr\n";
$structname = join("_", ($modname, "mod"));

$loaderstr = join("_", ($modname, "loadparams"));
print HEADER "static struct lp_mod $structname = { \"$modname\", $aname, $maxstr, (lp_modloader_t)$loaderstr,  0,0 };\n";

# print HEADER "static struct lp_mod $structname = { 0, 0, 0, 0, 0, 0 };\n";

print HEADER "#endif // _" . $MODNAME . "_PARAM_H\n";

endDoc();

close(HEADER);
close(CODE);
close(DOC);




