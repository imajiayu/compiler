#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>

class GenMips
{
public:
	string RVALUE[RegNum];
	map<string, set<string>> avalue;
	vector<Reg>regs_info;
	vector<quate> quate;
	ofstream asm_ostream;
	map<string, int> local_var_offset_table;  
	map<string, int> global_var_addr_table; 
	int param_offset = PARAM_OFFSET_INIT; 
	int localvar_offset = LOCALVAR_OFFSET_INIT;
	int globalvar_addr = DATA_SEG_ADDR;
	string procedureName = ""; 
	set<string> label_need_output;


	
	GenMips(string in_filepath, string file_path);
	
	void parse();
	void parse_step(vector<quate>::iterator present_quate);
	
	int get_reg(string var, int except_index);
	
	int get_lru_reg_index(int except_index);
	
	void mark_reg_in_regs_info(int reg_index);
	
	int get_reg_index_by_name(string reg_name);
};

vector<string> registers = { "$zero",
	"$at",
	"$v0","$v1",
	"$a0","$a1","$a2","$a3",
	"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
	"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
	"$t8","$t9",
	"$k0","k1",
	"$gp",
	"$sp",
	"$fp",
	"$ra"
};


struct quate
{
	int index;
	string operator_type;
	string arg1;
	string arg2;
	string result;
};


struct Reg
{
	string reg_name;
	int reg_index;
	int unuse_time;
	bool operator < (const Reg& b);
};