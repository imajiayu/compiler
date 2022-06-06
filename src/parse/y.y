%{
    #include<stdio.h>
	#include "y.tab.h"
	int yydebug=1;
    int yyerror();
    int yyerror(char* msg);
	extern FILE* yyin;
    extern int yylex();
%}
%token END
%token VOID INTEGER INT ID TRUE FALSE
%token REL
%token IF ELSE WHILE RETURN
%token OR AND
%left OR
%left AND
%right '!'
%left '+' '-'
%left '*' '/'
%right UMINUS

%%

startSym:	
	program
	{
		
	}
	|startSym program
	{

	}
	|error
	{

	}

program:
	type ID '(' args ')' block 
	{
		printf("<函数>\n");
	}

args:	
	arg_dec ',' args
	|
	arg_dec
	{
		printf("<参数列表>\n");
	}
	|
	VOID
	|
	{

	}

arg_dec:
	type ID
	{
		printf("<参数>\n");
	}

block:
	'{' decls statementlist '}'
	{
		printf("<函数体>\n");
	}

decls:
	decls decl        
	{
		printf("<声明语句>\n");
	}
    |            
	{

	}
    

decl:
	type ID ';' 
	{  
		printf("<声明>\n");
	}
	|
	adecl
	{

	}
	|
	dadecl
	{

	}

adecl:
	type ID '[' INTEGER ']' ';'
	{
		printf("<一维数组声明>\n");
	}

dadecl:
	type ID '[' INTEGER ']' '[' INTEGER ']' ';'
	{
		printf("<二维数组声明>\n");
	}
    
type: 
	INT    
	{  

	}
	| 
	VOID            
	{  

	}
    

statementlist:
	statementlist M statement    
	{ 
		printf("<语句列表>\n");
	}
	| 
	statement            
	{ 
		
	}
        

statement:
	IF '(' boolean ')' M statement ELSE N M statement    
	{ 
		printf("<IF ELSE语句>\n");
	} 
	| 
	WHILE M '(' boolean ')' M statement    
	{ 
		printf("<WHILE语句>\n");
	}
	| 
	assignment ';'        
	{ 
		printf("<WHILE语句>\n");
	}
	|
	'{' statementlist '}'                
	{ 
		
	}
	| 
	RETURN expression ';'	
	{
		printf("<RETURN语句>\n");
	}
	|
	RETURN ';'
	{
		printf("<RETURN语句>\n");
	}

assignment: 
	ID '=' boolean          
	{ 
		printf("<赋值语句>\n");
	}
	|
	array '=' boolean
	{

	}
	|
	darray '=' boolean


boolean: 
	boolean OR M boolean    
	{ 
		printf("<或表达式>\n");
	}
    | 
	boolean AND M boolean    
	{ 
		printf("<与表达式>\n");
	}
    | 
	'!' boolean        
	{ 
		printf("<非表达式>\n");
	}
    | 
	'(' boolean ')'        
	{ 
		
	}
    | 
	expression REL expression        
	{ 
		
	}
    | 
	TRUE            
	{ 
		
	}
    | 
	FALSE                
	{ 
		
	}
    | 
	expression        
	{ 
		
	}

M:         
	{ 
		
	}

N:            
	{ 
		
	}

expression:
	'(' expression ')'
	{
		
	}
	|
	expression '+' expression        
	{ 
		
	}
    | 
	expression '-' expression        
	{ 
		
	}
    | 
	expression '*' expression            
	{ 
		
	}
    | 
	expression '/' expression        
	{ 
		
	}
    | 
	'-' expression %prec UMINUS            
	{ 
		
	}
    | 
	loc            
	{ 
		
	}
	| 
	INTEGER	
	{
 
	}
	|
	callprogram
	{

	}

callprogram:
	ID '(' expressions ')'
	{
		printf("<函数调用>\n");
	}

expressions:
	expressions ',' expression
	{
		
	}
	|
	expression
	{
		printf("<形参>\n");
	}
	|
	{

	}

array:
	ID '[' INTEGER ']'
	{
		printf("<一维数组取址>\n");
	}

darray:
	ID '[' INTEGER ']' '[' INTEGER ']'
	{
		printf("<二维数组取址>\n");
	}

loc: 
	ID            
	{ 
		
	}
	|
	array
	{

	}
	|
	darray
	{

	}

%%


int yyerror(char* msg)
{
    printf("\nERROR with message: %s\n", msg);
    return 0;
}


int main(int argc, char** argv)
{
    yyin=fopen(argv[1],"r");
    freopen(argv[2], "wt+", stdout);
    yyparse();
    fclose(stdout);
    return 0;
}
