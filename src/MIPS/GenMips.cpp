#include "GenMips.h"

bool Reg::operator < (const Reg& b) {
	return unuse_time < b.unuse_time;
}


GenMips::GenMips(string in_filepath, string file_path)
{
	fstream file_in;
	file_in.open(in_filepath, ios::in);
	string tmp = "";
	char tem_ch;
	quate t;
	while (file_in.peek() != EOF)
	{
		file_in.get();
		file_in.get();
		file_in.get();
		file_in.get();
		while ((tem_ch = file_in.get()) != ':')
		{
			tmp += tem_ch;
		}
		t.index = atoi(tmp.c_str());
		tmp = "";
		file_in.get();
		file_in.get();
		file_in.get();
		while ((tem_ch = file_in.get()) != ',')
		{
			tmp += tem_ch;
		}
		if (tmp == "=[]" || tmp == "[]=")
			tmp = "=";
		t.operator_type = tmp;
		tmp = "";
		file_in.get();
		while ((tem_ch = file_in.get()) != ',')
		{
			tmp += tem_ch;
		}
		if (tmp == "_")
			tmp = "-";
		t.arg1 = tmp;
		tmp = "";
		file_in.get();
		while ((tem_ch = file_in.get()) != ',')
		{
			tmp += tem_ch;
		}
		if (tmp == "_")
			tmp = "-";
		t.arg2 = tmp;
		tmp = "";
		file_in.get();
		while ((tem_ch = file_in.get()) != ')')
		{
			tmp += tem_ch;
		}
		t.result = tmp;
		tmp = "";
		file_in.get();
		quate.push_back(t);
		continue;

	}

	
	file_out.open(file_path, ios::out);

	for (auto& it : quate) {
		if (it.operator_type == "j" || it.operator_type == "j=" || it.operator_type == "j<" || it.operator_type == "j>" || it.operator_type == "j<=" || it.operator_type == "j>=") {
			label_need_output.insert(it.result);
		}
	}

	
	for (int i = 0; i < RegNum; i++)
		regs_info.push_back({ registers[i],i,0 });

	
	RVALUE[0] = "0";
	for (int i = 1; i < RegNum; i++)
		RVALUE[i] = "null";
}

void GenMips::parse()
{
	
	file_out << "addi $sp,$sp," << to_string(STACK_SEG_ADDR) << endl;
	file_out << "addi $fp,$fp," << to_string(STACK_SEG_ADDR - 4) << endl;

	
	for (auto present_quate = quate.begin(); present_quate != quate.end(); present_quate++) {
		
		for (auto it = regs_info.begin(); it != regs_info.end(); it++)
			if (it->unuse_time < REG_MAX_UNUSETIME)
				it->unuse_time++;
		
		
		parse_step(present_quate);
	}
}

void GenMips::parse_step(vector<quate>::iterator present_quate)
{
	
	if (label_need_output.find(to_string(present_quate->index)) != label_need_output.end()) {
		file_out << "Label_" << to_string(present_quate->index) << " :" << endl;
	}

	
	if (present_quate->operator_type == "call") {
		
		for (auto& it : local_var_offset_table) {
			if (avalue.at(it.first).empty())
				continue;
			
			int unuse_time = REG_MAX_UNUSETIME;
			string write_reg;
			for (auto& r : avalue.at(it.first)) {
				if (regs_info[get_reg_index_by_name(r)].unuse_time < unuse_time) {
					unuse_time = regs_info[get_reg_index_by_name(r)].unuse_time;
					write_reg = r;
				}
			}
			file_out << "sw " << write_reg << "," << it.second << "($fp)" << endl;
		}

		
		file_out << "jal " << present_quate->arg1 << endl;
		
		
		int reg_index = get_reg(present_quate->result, -1);

		
		for (auto& it : local_var_offset_table) {
			if (avalue.at(it.first).empty())
				continue;
			
			for (auto& r : avalue.at(it.first)) {
				file_out << "lw " << r << "," << it.second << "($fp)" << endl;
			}
		}

		file_out << "move " << registers[reg_index] << ",$v1" << endl;
		reg_mark(reg_index);
	}
	
	else if (present_quate->operator_type == "return") {
		
		if (present_quate->arg1 != "-")
			file_out << "move $v1," << *(avalue.at(present_quate->arg1).begin()) << endl;

		
		for (auto it = local_var_offset_table.begin(); it != local_var_offset_table.end(); it++) {
			if (avalue.find(it->first) != avalue.end()) {
				for (auto reg = avalue.at(it->first).begin(); reg != avalue.at(it->first).end(); reg++)
					for (int i = 0; i < RegNum; i++)
						if (*reg == registers[i])
							RVALUE[i] = "null";
				avalue.erase(it->first);
			}
		}

		
		file_out << "move $sp,$fp" << endl;
		file_out << "addi $sp,$sp," << to_string(param_offset) << endl;
		
		file_out << "lw $ra,4($fp)" << endl;
		
		file_out << "lw $fp,0($fp)" << endl;

		
		if (procedureName != "main") {
			file_out << "jr $ra" << endl;
		}
		
		param_offset = PARAM_OFFSET_INIT;
		localvar_offset = LOCALVAR_OFFSET_INIT;
		local_var_offset_table.clear();
		procedureName = "";
	}
	
	else if (present_quate->operator_type == "j") {
		file_out << "j ";
		
		if (!isdigit(present_quate->result[0])) {
			for (auto it = quate.begin(); it != quate.end(); it++)
				if (present_quate->result == to_string(it->index))
					file_out << it->operator_type << endl;
		}
		
		else {
			file_out << "Label_" << present_quate->result << endl;
		}
	}
	
	else if (present_quate->operator_type == "+" || present_quate->operator_type == "*" || present_quate->operator_type == "-" || present_quate->operator_type == "/") {
		
		int reg_index_A = get_reg(present_quate->result, -1);
		int reg_index_B = -1;
		int reg_index_C = -1;

		
		if (avalue.find(present_quate->arg1) == avalue.end() || avalue.at(present_quate->arg1).empty()) {
			
			if (local_var_offset_table.find(present_quate->arg1) != local_var_offset_table.end()) {
				int offset = local_var_offset_table.at(present_quate->arg1);
				file_out << "lw " << registers[reg_index_A] << "," << to_string(offset) << "($fp)" << endl;
			}
			
			else if (global_var_addr_table.find(present_quate->arg1) != global_var_addr_table.end()) {
				int addr = global_var_addr_table.at(present_quate->arg1);
				file_out << "lw " << registers[reg_index_A] << "," << to_string(addr) << "($zero)" << endl;
			}
			reg_index_B = reg_index_A;
		}
		else {
			reg_index_B = get_reg_index_by_name(*(avalue.at(present_quate->arg1).begin()));
		}
		reg_mark(reg_index_B);

		
		if (!isdigit(present_quate->arg2[0])) {
			
			if (avalue.at(present_quate->arg2).empty()) {
				
				reg_index_C = get_reg(present_quate->arg2, reg_index_A);
			}
			else {
				reg_index_C = get_reg_index_by_name(*(avalue.at(present_quate->arg2).begin()));
			}
			reg_mark(reg_index_C);

			if (present_quate->operator_type == "+")
				file_out << "add " << registers[reg_index_A] << "," << registers[reg_index_B] << "," << registers[reg_index_C] << endl;
			else if (present_quate->operator_type == "*")
				file_out << "mul " << registers[reg_index_A] << "," << registers[reg_index_B] << "," << registers[reg_index_C] << endl;
			else if (present_quate->operator_type == "-")
				file_out << "sub " << registers[reg_index_A] << "," << registers[reg_index_B] << "," << registers[reg_index_C] << endl;
			else if (present_quate->operator_type == "/") {
				file_out << "div " << registers[reg_index_B] << "," << registers[reg_index_C] << endl;
				file_out << "mov " << registers[reg_index_A] << ",$lo" << endl;
			}
		}
		
		else {
			if (present_quate->operator_type == "+")
				file_out << "addi " << registers[reg_index_A] << "," << registers[reg_index_B] << "," << present_quate->arg2 << endl;
			else if (present_quate->operator_type == "*") {
				
				file_out << "addi $t8,$zero," << present_quate->arg2 << endl;

				file_out << "mul " << registers[reg_index_A] << "," << registers[reg_index_B] << ",$t8" << endl;
			}
			else if (present_quate->operator_type == "-") {
				file_out << "subi " << registers[reg_index_A] << "," << registers[reg_index_B] << "," << present_quate->arg2 << endl;
			}
			else if (present_quate->operator_type == "/") {
				
				file_out << "addi $t8,$zero," << present_quate->arg2 << endl;
				file_out << "div " << registers[reg_index_B] << ",$t8" << endl;
				file_out << "mov " << registers[reg_index_A] << ",$lo" << endl;
			}
		}
	}
	
	else if (present_quate->operator_type == "=") {
		int reg_index = get_reg(present_quate->result, -1);

		
		if (isdigit(present_quate->arg1[0])) {
			file_out << "addi " << registers[reg_index] << ",$zero," << present_quate->arg1 << endl;
			reg_mark(reg_index);
		}
		
		else {
			
			if (avalue.find(present_quate->arg1) == avalue.end())
				return;
			if (!avalue.at(present_quate->arg1).empty()) {
				
				int reg_index_B = get_reg_index_by_name(*(avalue.at(present_quate->arg1).begin()));
				file_out << "move " << registers[reg_index] << "," << registers[reg_index_B] << endl;
				reg_mark(reg_index);
				reg_mark(reg_index_B);
			}
			
			else {
				
				
				if (local_var_offset_table.find(present_quate->arg1) != local_var_offset_table.end()) {
					int offset = local_var_offset_table.at(present_quate->arg1);
					file_out << "lw " << registers[reg_index] << "," << to_string(offset) << "($fp)" << endl;
				}
				else if (global_var_addr_table.find(present_quate->arg1) != global_var_addr_table.end()) {
					int addr = global_var_addr_table.at(present_quate->arg1);
					file_out << "lw " << registers[reg_index] << "," << to_string(addr) << "($zero)" << endl;
				}
				reg_mark(reg_index);
			}
		}
	}
	
	else if (present_quate->operator_type == "param") {
		int reg_index;
		
		
		
		if (avalue.find(present_quate->arg1) == avalue.end() || avalue.at(present_quate->arg1).empty()) {
			reg_index = 1;
			
			if (local_var_offset_table.find(present_quate->arg1) != local_var_offset_table.end()) {
				int offset = local_var_offset_table.at(present_quate->arg1);
				reg_index = get_reg(present_quate->arg1, -1);
				file_out << "lw " << registers[reg_index] << "," << to_string(offset) << "($fp)" << endl;
				file_out << "subi $sp,$sp,4" << endl;
				file_out << "sw " << registers[reg_index] << ",0($sp)" << endl;
			}
			
			else if (global_var_addr_table.find(present_quate->arg1) != global_var_addr_table.end()) {
				int addr = global_var_addr_table.at(present_quate->arg1);
				reg_index = get_reg(present_quate->arg1, -1);
				file_out << "lw " << registers[reg_index] << "," << to_string(addr) << "($zero)" << endl;
				file_out << "subi $sp,$sp,4" << endl;
				file_out << "sw " << registers[reg_index] << ",0($sp)" << endl;
			}
		}
		
		else {
			reg_index = get_reg_index_by_name(*(avalue.at(present_quate->arg1).begin()));
			file_out << "subi $sp,$sp,4" << endl;
			file_out << "sw " << registers[reg_index] << ",0($sp)" << endl;
		}
		reg_mark(reg_index);
	}
	
	else if (present_quate->operator_type == "defpar") {
		
		local_var_offset_table.insert({ present_quate->result, {param_offset} });
		param_offset += 4;
		avalue.insert(pair<string, set<string>>(present_quate->result, {}));
	}
	
	else if (present_quate->arg1 == "-" && present_quate->arg2 == "-" && present_quate->result == "-") {
		
		procedureName = present_quate->operator_type;
		local_var_offset_table.clear();
		param_offset = PARAM_OFFSET_INIT;
		localvar_offset = LOCALVAR_OFFSET_INIT;

		
		file_out << present_quate->operator_type << " :" << endl;
		
		file_out << "subi $sp,$sp,4" << endl;
		
		file_out << "sw $ra,0($sp)" << endl;
		file_out << "subi $sp,$sp,4" << endl;
		
		file_out << "sw $fp,0($sp)" << endl;
		
		file_out << "move $fp,$sp" << endl;
	}
	
	else if (present_quate->operator_type == "j>") {
		
		int reg_index_A = -1;
		int reg_index_B = -1;
		
		if (isdigit(present_quate->arg1[0])) {
			file_out << "subi $t8,$zero," << present_quate->arg1 << endl;
			reg_index_A = get_reg_index_by_name("$t8");
		}
		
		else {
			reg_index_A = get_reg(present_quate->arg1, -1);
		}
		
		if (isdigit(present_quate->arg2[0])) {
			file_out << "subi $t9,$zero," << present_quate->arg2 << endl;
			reg_index_B = get_reg_index_by_name("$t9");
		}
		
		else {
			reg_index_B = get_reg(present_quate->arg2, -1);
		}

		
		
		
		file_out << "bgt " << registers[reg_index_A] << "," << registers[reg_index_B] << ",Label_" << present_quate->result << endl;
	}
	
	else if (present_quate->operator_type == "j<") {
		
		int reg_index_A = -1;
		int reg_index_B = -1;
		
		if (isdigit(present_quate->arg1[0])) {
			file_out << "subi $t8,$zero," << present_quate->arg1 << endl;
			reg_index_A = get_reg_index_by_name("$t8");
		}
		
		else {
			reg_index_A = get_reg(present_quate->arg1, -1);
		}
		
		if (isdigit(present_quate->arg2[0])) {
			file_out << "subi $t9,$zero," << present_quate->arg2 << endl;
			reg_index_B = get_reg_index_by_name("$t9");
		}
		
		else {
			reg_index_B = get_reg(present_quate->arg2, -1);
		}

		
		
		
		file_out << "blt " << registers[reg_index_A] << "," << registers[reg_index_B] << ",Label_" << present_quate->result << endl;
	}
	
	else if (present_quate->operator_type == "j>=") {
		
		int reg_index_A = -1;
		int reg_index_B = -1;
		
		if (isdigit(present_quate->arg1[0])) {
			file_out << "addi $t8,$zero," << present_quate->arg1 << endl;
			reg_index_A = get_reg_index_by_name("$t8");
		}
		
		else {
			reg_index_A = get_reg(present_quate->arg1, -1);
		}
		
		if (isdigit(present_quate->arg2[0])) {
			file_out << "addi $t9,$zero," << present_quate->arg2 << endl;
			reg_index_B = get_reg_index_by_name("$t9");
		}
		
		else {
			reg_index_B = get_reg(present_quate->arg2, -1);
		}

		
		
		
		file_out << "bge " << registers[reg_index_A] << "," << registers[reg_index_B] << ",Label_" << present_quate->result << endl;

	}
	
	else if (present_quate->operator_type == "j<=") {
		
		int reg_index_A = -1;
		int reg_index_B = -1;
		
		if (isdigit(present_quate->arg1[0])) {
			file_out << "addi $t8,$zero," << present_quate->arg1 << endl;
			reg_index_A = get_reg_index_by_name("$t8");
		}
		
		else {
			reg_index_A = get_reg(present_quate->arg1, -1);
		}
		
		if (isdigit(present_quate->arg2[0])) {
			file_out << "addi $t9,$zero," << present_quate->arg2 << endl;
			reg_index_B = get_reg_index_by_name("$t9");
		}
		
		else {
			reg_index_B = get_reg(present_quate->arg2, -1);
		}

		
		
		
		file_out << "ble " << registers[reg_index_A] << "," << registers[reg_index_B] << ",Label_" << present_quate->result << endl;

	}
	
	else if (present_quate->operator_type == "j=") {
		
		int reg_index_A = -1;
		int reg_index_B = -1;
		
		if (isdigit(present_quate->arg1[0])) {
			file_out << "subi $t8,$zero," << present_quate->arg1 << endl;
			reg_index_A = get_reg_index_by_name("$t8");
		}
		
		else {
			reg_index_A = get_reg(present_quate->arg1, -1);
		}
		
		if (isdigit(present_quate->arg2[0])) {
			file_out << "subi $t9,$zero," << present_quate->arg2 << endl;
			reg_index_B = get_reg_index_by_name("$t9");
		}
		
		else {
			reg_index_B = get_reg(present_quate->arg2, -1);
		}

		
		file_out << "beq " << registers[reg_index_A] << "," << registers[reg_index_B] << ",Label_" << present_quate->result << endl;
	}
}


int GenMips::get_reg(string var, int except_index)
{
	
	int reg_index = -1;

	
	
	if (local_var_offset_table.find(var) == local_var_offset_table.end() && global_var_addr_table.find(var) == global_var_addr_table.end()) {
		
		
		if (procedureName == "") {
			global_var_addr_table.insert({ var, globalvar_addr });
			globalvar_addr += 4;
		}
		
		else {
			local_var_offset_table.insert(pair<string, int>(var, localvar_offset));
			localvar_offset -= 4;
			file_out << "subi $sp,$sp,4" << endl;
		}
		
		avalue.insert({ var, {} });
	}

	
	
	if (avalue.at(var).empty()) {
		
		
		
		for (int i = VAR_REG_START; i <= VAR_REG_END; i++) {
			if (RVALUE[i] == "null") {
				reg_index = i;
				
				avalue.at(var).insert(registers[reg_index]);
				RVALUE[reg_index] = var;
				break;
			}

		}
		
		if (reg_index == -1) {
			
			int to_seize_reg_index = get_lru_reg_index(except_index);
			
			string to_seize_var_name = RVALUE[to_seize_reg_index];
			
			if (avalue.at(to_seize_var_name).size() >= 2) {
				
				
				
				
			}
			
			else {
				
				
				if (local_var_offset_table.find(to_seize_var_name) != local_var_offset_table.end()) {
					int offset = local_var_offset_table.at(to_seize_var_name);
					file_out << "sw " << registers[to_seize_reg_index] << "," << to_string(offset) << "($fp)" << endl;
				}
				
				else if (global_var_addr_table.find(to_seize_var_name) != global_var_addr_table.end()) {
					int addr = global_var_addr_table.at(to_seize_var_name);
					file_out << "sw " << registers[to_seize_reg_index] << "," << to_string(addr) << "($zero)" << endl;
				}
			}
			
			avalue.at(to_seize_var_name).erase(registers[to_seize_reg_index]);
			reg_index = to_seize_reg_index;
			RVALUE[reg_index] = var;
			avalue.at(var).insert(registers[to_seize_reg_index]);
		}
		
		if (local_var_offset_table.find(var) != local_var_offset_table.end()) {
			int offset = local_var_offset_table.at(var);
			file_out << "lw " << registers[reg_index] << "," << to_string(offset) << "($fp)" << endl;
		}
		
		else if (global_var_addr_table.find(var) != global_var_addr_table.end()) {
			int addr = global_var_addr_table.at(var);
			file_out << "lw " << registers[reg_index] << "," << to_string(addr) << "($zero)" << endl;
		}
	}
	
	else {
		string reg_name = *(avalue.at(var).begin());
		for (int i = 0; i < RegNum; i++) {
			if (registers[i] == reg_name) {
				reg_index = i;
				break;
			}
		}
	}
	reg_mark(reg_index);
	return reg_index;
}


int GenMips::get_lru_reg_index(int except_index) {
	int max_index = 0;
	int max_unuse = 0;
	for (int i = 0; i < regs_info.size(); i++) {
		
		if (regs_info[i].unuse_time > max_unuse && regs_info[i].reg_index >= VAR_REG_START && regs_info[i].reg_index <= VAR_REG_END && (except_index == -1 || except_index != regs_info[i].reg_index)) {
			max_index = i;
			max_unuse = regs_info[i].unuse_time;
		}
	}
	return regs_info[max_index].reg_index;
}

void GenMips::reg_mark(int reg_index) {
	for (auto it = regs_info.begin(); it != regs_info.end(); it++) {
		if (it->reg_index == reg_index) {
			it->unuse_time = 0;
			return;
		}
	}
}

int GenMips::get_reg_index_by_name(string reg_name)
{
	for (auto it = registers.begin(); it != registers.end(); it++) {
		if (*it == reg_name)
			return it - registers.begin();
	}
	return -1;
}


int main(int argc, char** argv) {
	GenMips GenMips(argv[1], argv[2]);
	GenMips.parse();
	return 0;
}

