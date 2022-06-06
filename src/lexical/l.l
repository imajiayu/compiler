%{
	#include<stdio.h>
	#include<stdlib.h>

	struct variable{
		char name[128];
		int id;
	};

	struct variable var_list[128];
	int var_num=0;

	int wordCounter =0;
	int charCounter =0;
	int line = 1;
	int identifier_id=0;
	void addLine(int);
	void addChar(int);
	void addWord(int);
	void intobit(int num);
%}
line_comment	(\/\/.*\n) 
%x BLOCK_COMMENT 
letter	[A-Za-z_] 
digit	[0-9]
identifier	{letter}({letter}|{digit})* 
reserveword	"int"|"void"|"if"|"else"|"while"|"return"
preprocess	^#.*\n	
integer	{digit}+	
operater	"*"|"("|")"|"-"|"+"|"="|"=="|"{"|"}"|";"|"<"|">"|","|"/"|"!="|"<="|">="|"["|"]"
wrong	{integer}{letter}+
end	^#(\n)*

%%
{end}	{
	printf("Line=%3d : <$STOP,->\n",line);
	addChar(yyleng);
	addWord(1);
	return 0;
}

{preprocess} {
	printf("Line=%3d,This is a preprocess %s",line,yytext);
	addChar(yyleng);
	addLine(1);
}

{operater}	{
	addChar(yyleng);
	addWord(1);
	if(!strcmp(yytext,"+"))
		printf("Line=%3d : +\n",line);
	else if(!strcmp(yytext,"-"))
		printf("Line=%3d : -\n",line);
	else if(!strcmp(yytext,"*"))
		printf("Line=%3d : *\n",line);
	else if(!strcmp(yytext,"\\"))
		printf("Line=%3d : \\\n",line);
	else if(!strcmp(yytext,"="))
		printf("Line=%3d : =\n",line);
	else if(!strcmp(yytext,"=="))
		printf("Line=%3d : ==\n",line);
	else if(!strcmp(yytext,">"))
		printf("Line=%3d : >\n",line);
	else if(!strcmp(yytext,">="))
		printf("Line=%3d : >=\n",line);
	else if(!strcmp(yytext,"<"))
		printf("Line=%3d : <\n",line);
	else if(!strcmp(yytext,"<="))
		printf("Line=%3d : <=\n",line);
	else if(!strcmp(yytext,","))
		printf("Line=%3d : ,\n",line);
	else if(!strcmp(yytext,";"))
		printf("Line=%3d : ;\n",line);
	else if(!strcmp(yytext,"("))
		printf("Line=%3d : (\n",line);
	else if(!strcmp(yytext,")"))
		printf("Line=%3d : )\n",line);
	else if(!strcmp(yytext,"{"))
		printf("Line=%3d : {\n",line);
	else if(!strcmp(yytext,"}"))
		printf("Line=%3d : }\n",line);
	else if(!strcmp(yytext,"["))
		printf("Line=%3d : [\n",line);
	else if(!strcmp(yytext,"]"))
		printf("Line=%3d : ]\n",line);
}

{reserveword} {
	if(!strcmp(yytext,"int"))
		printf("Line=%3d : <int,->\n",line);
	else if(!strcmp(yytext,"void"))
		printf("Line=%3d : <void,->\n",line);
	else if(!strcmp(yytext,"if"))
		printf("Line=%3d : <$IF,->\n",line);
	else if(!strcmp(yytext,"else"))
		printf("Line=%3d : <$ELSE,->\n",line);
	else if(!strcmp(yytext,"while"))
		printf("Line=%3d : <$WHILE,->\n",line);
	else if(!strcmp(yytext,"return"))
		printf("Line=%3d : <$RETURN,->\n",line);
	addChar(yyleng);
	addWord(1);
}

{identifier} {
	int i;
	for(i=0;i<var_num;i++)
	{
		if(!strcmp(yytext,var_list[i].name))
		{
			printf("Line=%3d : <$ID,%d>\n",line,var_list[i].id);
			break;
		}
	}
	if(i==var_num)
	{
		strcpy(var_list[var_num].name,yytext);
		var_list[var_num].id=var_num;
		var_num++;
		printf("Line=%3d : <$ID,%d>\n",line,var_num);
	}

	addChar(yyleng);
	addWord(1);
}

{wrong}	{
	addChar(yyleng);
	printf("Line=%3d : error: unknown word\n",line);
}

"/*" {
	printf("Line=%3d : This is a block comment begin\n",line);
	BEGIN(BLOCK_COMMENT);
	yymore();
}
<BLOCK_COMMENT>.|\n	{
	yymore();
	if(yytext[0] == '\n'){
		addLine(1);
	}
}

<BLOCK_COMMENT>"*/" {
	addChar(yyleng);
	printf("%s\n",yytext);
	printf("Line=%3d : This is a block comment end\n",line);
	BEGIN(INITIAL);
}


{line_comment}	{
	printf("Line=%3d : This is a line comment : %s",line,yytext);
	addChar(yyleng);
	addLine(1);
}

{integer} {
	addChar(yyleng);
	printf("Line=%3d : <$INT,",line);
	intobit(atoi(yytext));
	printf(">\n");
}

.	{
	addChar(1);
}

\n	{
	addChar(1);
	addLine(1);
}

%%
int main(int argc,char** argv)
{
	yyin=fopen(argv[1],"r");
	freopen(argv[2],"w",stdout);
	yylex();
	freopen(argv[3],"w",stdout);
	printf("%d\n",line);
	printf("%d\n",charCounter);
	printf("%d",wordCounter);
	return 0;
}
int yywrap() {
	return 1;
}

void addLine(int cnt) {
	line += cnt;
}

void addChar(int leng) {
	charCounter += leng;
}
void addWord(int cnt) {
	wordCounter += cnt;
}

void intobit(int num){
	int tnum=num;
	int i;
	for(i=31;i>=0;i--)
	{
		int tmpa=tnum;
		tmpa=tnum&(1<<i);
		((i+1)%8==0&&i!=31)?printf(" %d",tmpa>>i):printf("%d",tmpa>>i);
	}
}