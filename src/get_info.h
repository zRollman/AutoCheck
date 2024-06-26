#ifndef _GET_INFO_
#define _GET_INFO_

#include"instr_info.h"
#include<sstream>
#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#include<set>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<stack>
#include<omp.h>

void get_parameter(string& is_enable_openMP, string& path, string& function, int& call_loc, int& start_line_num, int& end_line_num);
void get_info(string path, string& function_name, int call_loc, int start_line_num, int end_line_num, unordered_map<string, string>& in_loop_map, vector<instr_info>& instr_info_set, unordered_map<string, var_loc>& var_loc);
void get_instruction(string line, vector<instr_info>& instr_info, int& n);
bool isNum(string str);

//before
void get_variables(vector<instr_info>& instr_info_set, unordered_map<string, string>& variable_map, int call_loc);
//inside
void get_variables(vector<instr_info>& instr_info_set, unordered_map<string, string>& variable_map, stack<set<string>>& local_variable, set<string>& global_variable_set, unordered_map<string, var_loc>& loc);
void opcode_operation(string dynValue, string variable, unordered_map<string, string>& variable_map);
void opcode_operation(string dynValue, string variable, unordered_map<string, string>& variable_map, stack<set<string>>& local_variable, unordered_map<string, var_loc>& var_loc, string function, int line_num);
void match(unordered_map<string, string> before_loop_map, unordered_map<string, string>& in_loop_map, set<string>& global_variable_set);
void traver_map(unordered_map<string, string> in_loop_map);

void get_c_plus(string path, string& function_name, int call_loc, int start_line_num, int end_line_num, unordered_map<string, string>& in_loop_map, vector<instr_info>& instr_info_set, unordered_map<string, var_loc>& var_loc);
void get_function_name(ifstream& in, vector<instr_info>& before_loop_instr_set, int& n, string& function_name, int call_loc);
void get_before_loop_and_index_instrction(ifstream& in, vector<instr_info>& before_loop_instr_set, vector<instr_info>& index_instr_set, int& k, string& function_name, int start_line_num, int& opencode);
void get_inside_loop_instr(ifstream& in, vector<instr_info>& instr_info_set, int& k, string& function_name, int end_line_num, int opencode);


//omp
void get_all_instr_omp(vector<instr_info>& instr_set, string path, int start_line_num, int stop_line_num);
void get_info_omp(int num_th, int max_file_id, string file_names, int start_line_num, int end_line_num, string function, int call_loc, unordered_map<string, string>& inside_loop_map, vector<instr_info>& inside_loop_instr_set, unordered_map<string, var_loc>& variable_loc);
void get_function_name_omp(vector<instr_info>& instr_set, string& function, int call_loc, int& call_dyn_id, int file_id, int& call_file_id);
void get_main_loop_loction_omp(vector<instr_info>& instr_set, string function, int call_dyn_id, map<int, int>& start_map, map<int, int>& stop_map, int start_line_num, int stop_line_num, int file_id);
void get_main_loop_loction_omp(vector<instr_info>& instr_set, string function, map<int, int>& start_map, map<int, int>& stop_map, int start_line_num, int stop_line_num, int file_id);


#endif // !_GET_INFO_
