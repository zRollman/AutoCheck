#include<cstdlib>
#include<time.h>

#include"get_info.h"
#include"instr_info.h"
#include"variable_match.h"

int main(int argc, char* argv[]) {

	string path, function_name, is_enable_OpenMP;
	int call_loc, start_line_num, end_line_num;
	//int flag = atoi(argv[1]);
	is_enable_OpenMP = "off";
	//get_parameter(is_enable_OpenMP, path, function_name, call_loc, start_line_num, end_line_num, flag);
	/*path = "/home/lhm/test/tracefile/IS/dynamic_trace";
	function_name = "main";
	is_enable_OpenMP = "off";
	call_loc = 0;
	start_line_num = 732;
	end_line_num = 746;*/
	get_parameter(is_enable_OpenMP, path, function_name, call_loc, start_line_num, end_line_num);
	unordered_map<string, string> in_loop_map;
	unordered_map<string, var_loc> variable_location;
	vector<instr_info> instr_info_set;
	if (is_enable_OpenMP == "off") {
		if (call_loc == 0) {
			get_info(path, function_name, call_loc, start_line_num, end_line_num, in_loop_map, instr_info_set, variable_location);
		}
		else
		{
			get_c_plus(path, function_name, call_loc, start_line_num, end_line_num, in_loop_map, instr_info_set, variable_location);
		}
	}
	else
	{
		int nthreads = omp_get_num_procs();
		get_info_omp(nthreads, nthreads, path, start_line_num, end_line_num, function_name, call_loc, in_loop_map, instr_info_set, variable_location);
	}
	
	string index = "";
	get_index(start_line_num, instr_info_set, index);
	dyn_ist_tra(start_line_num, instr_info_set, in_loop_map, function_name, variable_location, path);
	return 0;
}
