#ifndef _VAR_MATCH_
#define _VAR_MATCH_

#include<unordered_map>
#include<map>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <ctime>
#include<set>

#include<stack>
#include"get_info.h"

#include"dependecy_info.h"



struct var_used_info {
	int line;
	int size;
	int times = 0;
	string type;
};


struct var_info {
	string varName;
	string varType;
	string regName;
	int location = 0;
};
struct ddd {
	unordered_map<string, string> variable_map;
	unordered_map<string, vector<string>> variable_dependency_map;
	unordered_map<string, vector<string>> alloc_variable_dependency_map;
	unordered_map<string, var_used_info> var_first_used;
};

struct variable_lable
{
	string lable = "";
	bool if_write_label = false;
	bool read = false;
	bool point_to_self = false;
};



void get_index(int start_line, vector<instr_info> instr_info_set, string& index);
void dyn_ist_tra(int start_line, vector<instr_info>& instr_info_set, unordered_map<string, string> in_loop_map, string target_funcname, unordered_map<string, var_loc>& var_location, string benchmark);
void handle(vector<instr_info> IterProcess, const unordered_map<string, string>& in_loop_map, vector<string>& code_execu_seq_set, vector<dependecy_info>& dependency_set);
void create_map(stack<ddd>& stk);
void ret_instruction(stack<ddd>& stk);
void alloca_instruction(const vector<oprd_line> oprd_line_set, stack<ddd>& stk);
void alloc_var_dependency_find(vector<string>& result_set, stack<ddd>& stk, string var, bool flag);
void load_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk);
void store_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, dependecy_info& an_dependency_info, int lineId);
void store_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, dependecy_info& an_dependency_info, int lineId, string bitVar, bool& bit_cast_flag);
void get_element_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk);
void BitCast_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, bool& bit_cast_flag, string& bitVar);
void intrinsic_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, dependecy_info& an_dependency_info, int lineId, bool& bit_cast_flag);
void call_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, const unordered_map<string, string>& in_loop_map, bool creatFlag);
void calculation_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk);
void insert(string res, const vector<string> factor, dependecy_info& an_dependency_info, int lineId);
void get_graph(const vector<dependecy_info>& dependency_set, unordered_map<string, var_loc>& var_location);
void write_location(vector<string>& locations);
void write_graph(string str);
void get_critical_variables(unordered_map<string, int> cpk_map, vector<string>& cpk_set, unordered_map<string, used_info> variable_used, set<string> dependency_with_other, unordered_map<string, var_loc>& var_location);
#endif // !_VAR_MATCH_
