%{ 
	#include"y.tab.h"
%}

line_comment	(\/\/.*\n) 
%x BLOCK_COMMENT 

delim        [ \t\n\r]
ws        {delim}+
letter        [A-Za-z]
digit        [0-9]
id        {letter}({letter}|{digit})*
integer        {digit}+

preprocess	^#.*\n	
end	^#(\n)*

%%
{end}	{
	return (END);
}

{preprocess} {

}

"/*" {
	BEGIN(BLOCK_COMMENT);
	yymore();
}
<BLOCK_COMMENT>.|\n	{
	yymore();
}

<BLOCK_COMMENT>"*/" {
	BEGIN(INITIAL);
}

{line_comment}	{
}

if    { return( IF ); }
else     { return( ELSE ); }
while    { return( WHILE ); }
return	{ return( RETURN ); }
true    { return( TRUE ); }
false    { return( FALSE ); }
int    { return( INT ); }
void	{ return( VOID ); }
"&&"    { return( AND ); }
"||"    { return( OR ); }
"!"    { return( '!'); }
"<"|"<="|">"|">="|"!="|"=="    { return( REL); }
"+"    { return( '+' ); }
"-"    { return( '-' ); }
"*"    { return( '*' ); }
"/"    { return( '/' ); }
"="    { return( '=' ); }
"{"    { return( '{' ); }
"}"    { return( '}' ); }
"("    { return( '(' ); }
")"    { return( ')' ); }
"["		{return('[');}
"]"		{return(']');}
";"    { return( ';' ); }
","		{ return( ',' ); }
{ws}    { }
{id}    {  return( ID ); }
{integer}	{ return INTEGER;}
%%

int yywrap() {
	return 1;
}