
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



%x src

%{
#include <string.h>

#include "libparam.tab.h"
#define MAX_INPUT_FILES 32
  int top_file = 0;
  struct { 
    struct yy_buffer_state *b; 
    int lineno; 
    char *filename;
  } input_files[MAX_INPUT_FILES];
  int lp_lineno = 0;
  char *lp_filename;
%}

%option noyywrap
%option yylineno

source source
semi ;
lbrace \{
rbrace \}
lbrak \[
rbrak \]
comma ,
decint -?[0-9]+ 
hexint -?0x[0-9a-fA-F]+
float -?[0-9]+\.[0-9]+(e[+-][0-9]+)?
equal =
qu \?
colon :
string [^ \t\n\f;{},\[\]:\?=]+
comment #[^\n\f]*
newline [\r\n]
whitespace [ \t]+
dotdot \.\.
topology topology
instantiate instantiate
as as
%%

{whitespace} ; 
{newline} { lp_lineno++; };

{semi}   { return SEMI; };
{lbrace} { return LBRACE; };
{lbrak}  { return LBRAK; };
{rbrak}  { return RBRAK; };
{comma}  { return COMMA; };
{equal}  { return EQUAL; };
{lbrace} { return LBRACE; };
{rbrace} { return RBRACE; };
{comment} ;
{qu} { return QU; }
{colon} { return COLON; }
{dotdot} { return DOTDOT; }
{source} BEGIN(src);

{topology} { return TOPOLOGY; }
{instantiate} { return INSTANTIATE; }
{as} { return AS; }

{decint} { 
  libparamlval.i = atoi(yytext); return DECINT; 
};

{hexint} { 
  libparamlval.i = strtol(yytext, 0, 16); return HEXINT; 
};

{float} { 
  libparamlval.d = strtod(yytext, 0); 
  return FLOAT; 
};

{string} {
/*    printf("STRING: \"%s\"\n", yytext);  */
  libparamlval.s = strdup(yytext); return STRING; 
};

<src>{string} {

  input_files[top_file].b = YY_CURRENT_BUFFER;
  input_files[top_file].lineno = lp_lineno;
  input_files[top_file].filename = lp_filename;
  top_file++;

  lp_filename = strdup(yytext);

  yyin = fopen(lp_filename, "r");

  if(!yyin) {
    fprintf(stderr, "*** error: couldn't open %s\n", yytext);
    yyterminate();
  }
  else {
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    lp_lineno = 0;
  }
  BEGIN(INITIAL);
}

<<EOF>> {

  if(--top_file < 0) { yyterminate(); }
  else {
    libparam_delete_buffer(YY_CURRENT_BUFFER);
    libparam_switch_to_buffer(input_files[top_file].b);

    lp_lineno = input_files[top_file].lineno;

/*      free(lp_filename); */
    lp_filename = input_files[top_file].filename;
  }


}


%%








