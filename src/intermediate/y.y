%{
    #include "h.h"
    #define YYSTYPE node
    #include "y.tab.h"
	int yydebug=1;
    int yyerror();
    int yyerror(char* msg);
	extern FILE* yyin;
    extern int yylex();
    char fun_name[128];
	char temp[128];
	char temp1[128];
	char temp2[128];
    codelist* list;
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
	type ID_ '(' args ')' block 
	{
		
	}

ID_:
	ID
	{
		strcpy(fun_name,$1.lexeme);
		gen_custmise(list,$1.lexeme,"_","_","_");
		if(!strcmp($1.lexeme,"main"))
		{
			sprintf(tmp,"(j, _, _, %d)",list->linecnt-1);
			strcpy(list->code[0],tmp);
		}
	}

args:	
	arg_dec ',' args
	|
	arg_dec
	{

	}
	|
	VOID
	{

	}
	|
	{

	}

arg_dec:
	type ID
	{
		gen_custmise(list,"defpar","_","_",$2.lexeme);
	}

block:
	'{' decls statementlist '}'
	{

	}

decls:
	decls decl        
	{

	}
    |            
	{

	}
    

decl:
	type ID ';' 
	{  

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
		
	}

dadecl:
	type ID '[' INTEGER ']' '[' INTEGER ']' ';'
	{
		
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
		backpatch(list, $1.nextlist, $2.instr);
		$$.nextlist = $3.nextlist; 
	}
	| 
	statement            
	{ 
		$$.nextlist = $1.nextlist; 
	}
        

statement:
	IF '(' boolean ')' M statement ELSE N M statement    
	{ 
		backpatch(list, $3.truelist, $5.instr);
		backpatch(list, $3.falselist, $9.instr);
		$6.nextlist = merge($6.nextlist, $8.nextlist);
		$$.nextlist = merge($6.nextlist, $10.nextlist); 
	} 
	| 
	IF '(' boolean ')' M statement        
	{ 
		backpatch(list, $3.truelist, $5.instr);
		$$.nextlist = merge($3.falselist, $6.nextlist); 
	}
	| 
	WHILE M '(' boolean ')' M statement    
	{ 
		backpatch(list, $7.nextlist, $2.instr);
		backpatch(list, $4.truelist, $6.instr);
		$$.nextlist = $4.falselist;
		gen_goto(list, $2.instr); 
	}
	| 
	assignment ';'        
	{ 
		$$.nextlist = NULL; 
	}
	|
	'{' statementlist '}'                
	{ 
		$$.nextlist = $2.nextlist; 
	}
	| 
	RETURN expression ';'	
	{
		gen_custmise(list,"return",$2.addr,"_",fun_name);
	}
	|
	RETURN ';'
	{
		gen_custmise(list,"return","_","_",fun_name);
	}

assignment: 
	ID '=' boolean          
	{ 
		copyaddr(&$1, $1.lexeme); 
		gen_assignment(list, $3, $1); 
	}
	|
	array '=' boolean
	{
		gen_custmise(list,"[]=",$3.addr,$1.addr,$1.lexeme);
	}
	|
	darray '=' boolean
	{
		gen_custmise(list,"[]=",$3.addr,$1.addr,$1.lexeme);
	}

boolean: 
	boolean OR M boolean    
	{ 
		backpatch(list, $1.falselist, $3.instr);
        $$.truelist = merge($1.truelist, $4.truelist);
        $$.falselist = $4.falselist; 
	}
    | 
	boolean AND M boolean    
	{ 
		backpatch(list, $1.truelist, $3.instr);
        $$.truelist = $4.truelist;
        $$.falselist = merge($1.falselist, $4.falselist); 
	}
    | 
	'!' boolean        
	{ 
		$$.truelist = $1.falselist;
        $$.falselist = $1.truelist; 
	}
    | 
	'(' boolean ')'        
	{ 
		$$.truelist = $1.truelist; 
        $$.falselist = $1.falselist; 
	}
    | 
	expression REL expression        
	{ 
		$$.truelist = new_instrlist(nextinstr(list));
        $$.falselist = new_instrlist(nextinstr(list)+1);
        gen_if(list, $1, $2.oper, $3);
        gen_goto_blank(list); 
	}
    | 
	TRUE            
	{ 
		copyaddr(&$$, "TRUE");
        gen_goto_blank(list); 
	}
    | 
	FALSE                
	{ 
		copyaddr(&$$, "FALSE");
        gen_goto_blank(list); 
	}
    | 
	expression        
	{ 
		copyaddr_fromnode(&$$, $1); 
	}

M:         
	{ 
		$$.instr = nextinstr(list); 
	}

N:            
	{ 
		$$.nextlist = new_instrlist(nextinstr(list));
        gen_goto_blank(list); 
	}

expression: 
	'(' expression ')'
	{

	}
	|
	expression '+' expression        
	{ 
		new_temp(&$$, get_temp_index(list)); 
		gen_3addr(list, $$, $1, "+", $3); 
	}
    | 
	expression '-' expression        
	{ 
		new_temp(&$$, get_temp_index(list)); 
		gen_3addr(list, $$, $1, "-", $3); 
	}
    | 
	expression '*' expression            
	{ 
		new_temp(&$$, get_temp_index(list)); 
		gen_3addr(list, $$, $1, "*", $3); 
	}
    | 
	expression '/' expression        
	{ 
		new_temp(&$$, get_temp_index(list)); 
		gen_3addr(list, $$, $1, "/", $3); 
	}
    | 
	'-' expression %prec UMINUS            
	{ 
		new_temp(&$$, get_temp_index(list)); 
		gen_2addr(list, $$, "-", $2); 
	}
    | 
	loc            
	{ 
		copyaddr_fromnode(&$$, $1); 
	}
	| 
	INTEGER	
	{
		copyaddr(&$$, $1.lexeme); 
	}
	|
	callprogram
	{
		copyaddr(&$$, $1.lexeme); 
	}

callprogram:
	ID '(' expressions ')'
	{
		new_temp(&$$, get_temp_index(list));
		gen_custmise(list,"call",$1.lexeme,"_",$$.addr);
	}

expressions:
	expressions ',' expression
	{
		gen_custmise(list,"param",$3.addr,"_","_");
	}
	|
	expression
	{
		gen_custmise(list,"param",$1.addr,"_","_");
	}
	|
	{

	}

loc: 
	ID            
	{ 
		copyaddr(&$$, $1.lexeme); 
	}
	|
	array
	{
		sprintf(temp, "t%d", get_temp_index(list));
		gen_custmise(list,"=[]",$1.lexeme,$1.addr,temp);
		copyaddr(&$$,temp);
	}
	|
	darray
	{
		sprintf(temp, "t%d", get_temp_index(list));
		gen_custmise(list,"=[]",$1.lexeme,$1.addr,temp);
		copyaddr(&$$,temp);
	}

array:
	ID '[' INTEGER ']'
	{
		new_temp(&$$, get_temp_index(list)); 
		gen_custmise(list, "*", $3.lexeme,"4", $$.addr);
		strcpy($$.lexeme,$1.lexeme);
	}

darray:
	ID '[' INTEGER ']' '[' INTEGER ']'
	{
		sprintf(temp, "t%d", get_temp_index(list));
		gen_custmise(list, "*", $3.lexeme,"8", temp);
		sprintf(temp1, "t%d", get_temp_index(list));
		gen_custmise(list, "*", $6.lexeme,"4", temp1);
		new_temp(&$$, get_temp_index(list)); 
		gen_custmise(list, "+", temp,temp1, $$.addr);
		strcpy($$.lexeme,$1.lexeme);
	}

%%


int yyerror(char* msg)
{
    printf("\nERROR with message: %s\n", msg);
    return 0;
}


int main(int argc, char** argv)
{
    list = newcodelist();
    yyin=fopen(argv[1],"r");
    freopen(argv[2], "wt+", stdout);
	gen_goto(list,-1);
    yyparse();
    print(list);
    fclose(stdout);
    return 0;
}
