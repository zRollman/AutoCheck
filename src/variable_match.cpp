#include"variable_match.h"

string benchmark_name;
string loc_function;
int dyID, start_line_num;
string index_variable;
unordered_map<string, int> g_variable_index_map;
unordered_map<int, string> g_index_variable_map;
unordered_map<string, var_used_info> var_first_used;
vector<string> var_right;
var_info an_var_info;


void get_index(int start_line, vector<instr_info> instr_info_set, string& index) {
	int inst_sz = instr_info_set.size();
	int oprd_sz;
	int lineId;
	vector<int> opcode_set;
	unordered_map<string, string> index_set;
	for (int i = 0; i < inst_sz; i++) {
		instr_info inf = instr_info_set[i];
		vector<oprd_line> oprd_line_set = inf.oprd_line_set;
		string blockId = inf.blockId;
		lineId = inf.lineId;
		int opcodeId = inf.opcodeId;
		int dynInstId = inf.dynInstId;
		if (lineId == start_line) {
			if (opcodeId == 27 && !isNum(inf.oprd_line_set[0].regName))
				index_set.insert(make_pair(inf.oprd_line_set[1].regName, inf.oprd_line_set[0].regName));
			else if (opcodeId == 46) {
				//cout << inf.dynInstId << endl;
				auto it = index_set.find(inf.oprd_line_set[oprd_line_set.size() - 2].regName);
				if (it != index_set.end()) {
					index = it->second;
					index_variable = index;
					break;
				}
				else
					continue;
			}
		}
	}
}

void dyn_ist_tra(int start_line, vector<instr_info>& instr_info_set, unordered_map<string, string> in_loop_map, string target_funcname, unordered_map<string, var_loc>& var_location, string benchmark) {

	benchmark_name = benchmark;
	vector<dependecy_info> all_dependency_set;
	vector<string> code_execu_seq_set;
	vector<instr_info> IterProcess;

	start_line_num = start_line;
	loc_function = target_funcname;
	int n = 1;
	for (auto iter = in_loop_map.begin(); iter != in_loop_map.end(); iter++) {
		auto it = g_variable_index_map.find(iter->second);
		if (it == g_variable_index_map.end()) {
			g_variable_index_map.insert(make_pair(iter->second, n));
			g_index_variable_map.insert(make_pair(n, iter->second));
			n++;
		}
	}
	g_variable_index_map.insert(make_pair("other_variables", 0));
	g_index_variable_map.insert(make_pair(0, "other_variables"));

	int current_line = 0;
	int pre_line = 0;
	int inst_sz = instr_info_set.size();
	int i = 0;
	while (instr_info_set[i].lineId == -1) {
		//不分析main loop中的初始化变量
		i++;
	}
	current_line = instr_info_set[i].lineId;
	pre_line = instr_info_set[i].lineId;
	for (i; i < inst_sz; i++) {
		instr_info inf = instr_info_set[i];
		vector<oprd_line> oprd_line_set = inf.oprd_line_set;
		string blockId = inf.blockId;
		int lineId = inf.lineId;
		string funcname = inf.funcName;
		string basicBlockId = inf.basicBlockId;
		string staticInstId = inf.staticInstId;
		int opcodeId = inf.opcodeId;
		int dynInstId = inf.dynInstId;
		current_line = lineId;
		if (pre_line != current_line && current_line == start_line && funcname == target_funcname) {
			var_first_used.clear();
			var_right.clear();
			handle(IterProcess, in_loop_map, code_execu_seq_set, all_dependency_set);
			IterProcess.clear();
			IterProcess.push_back(instr_info_set[i]);
		}
		else {
			IterProcess.push_back(instr_info_set[i]);
		}
		pre_line = current_line;
	}

	get_graph(all_dependency_set, var_location);

}

void create_map(stack<ddd>& stk) {
	ddd an_ddd;
	for (auto it = g_variable_index_map.begin(); it != g_variable_index_map.end(); it++) {
		an_ddd.variable_map.insert(make_pair(it->first, it->first));
	}
	stk.push(an_ddd);
}

void handle(vector<instr_info> IterProcess, const unordered_map<string, string>& in_loop_map, vector<string>& code_execu_seq_set, vector<dependecy_info>& dependency_set) {
	stack<ddd> stk;
	int instr_sz = IterProcess.size();
	create_map(stk);
	dependecy_info an_dependency;
	int pre_line = 0;
	int callLine = 0;
	string pre_func_name = IterProcess[0].funcName;
	string curr_execu_seq;
	string bitVar;
	bool bit_cast_flag = false;
	for (int i = 0; i < instr_sz; i++) {
		instr_info inf = IterProcess[i];
		vector<oprd_line> oprd_line_set = inf.oprd_line_set;
		string blockId = inf.blockId;
		int lineId = inf.lineId;
		string funcName = inf.funcName;
		string basicBlockId = inf.basicBlockId;
		string staticInstId = inf.staticInstId;
		int opcodeId = inf.opcodeId;
		int dynInstId = inf.dynInstId;
		dyID = dynInstId;

		if (pre_line != lineId || pre_func_name != funcName)
			curr_execu_seq.append(to_string(lineId));

		pre_func_name = funcName;
		pre_line = lineId;

		bool creatFlag = false;
		if (opcodeId == 49) {
			int j = i + 1;
			if (j < IterProcess.size()) {
				string nextFuncName = IterProcess[j].funcName;
				if (oprd_line_set[0].regName == nextFuncName) {
					creatFlag = true;
					callLine = lineId;
				}

			}
		}
		switch (opcodeId)
		{
		case 1:ret_instruction(stk); break;
		case 8:calculation_instruction(oprd_line_set, stk); break;
		case 9:calculation_instruction(oprd_line_set, stk); break;
		case 10:calculation_instruction(oprd_line_set, stk); break;
		case 11:calculation_instruction(oprd_line_set, stk); break;
		case 12:calculation_instruction(oprd_line_set, stk); break;
		case 13:calculation_instruction(oprd_line_set, stk); break;
		case 26:alloca_instruction(oprd_line_set, stk); break;
		case 27:load_instruction(oprd_line_set, stk); break;
		case 28:if (bit_cast_flag)
			store_instruction(oprd_line_set, stk, an_dependency, lineId, bitVar, bit_cast_flag);
			   else
			store_instruction(oprd_line_set, stk, an_dependency, lineId);
			break;
		case 29:get_element_instruction(oprd_line_set, stk); break;
		case 44:BitCast_instruction(oprd_line_set, stk, bit_cast_flag, bitVar); break;
		case 104:intrinsic_instruction(oprd_line_set, stk, an_dependency, lineId, bit_cast_flag);
		case 49:call_instruction(oprd_line_set, stk, in_loop_map, creatFlag); break;
		default:
			break;
		}
	}

	auto it = find(code_execu_seq_set.begin(), code_execu_seq_set.end(), curr_execu_seq);
	if (it == code_execu_seq_set.end()) {
		code_execu_seq_set.push_back(curr_execu_seq);
		dependency_set.push_back(an_dependency);
	}
}

void ret_instruction(stack<ddd>& stk) {
	stk.pop();
}

void alloca_instruction(const vector<oprd_line> oprd_line_set, stack<ddd>& stk) {

	unordered_map<string, string>* variable_map = &stk.top().variable_map;
	auto it = variable_map->find(oprd_line_set[1].regName);
	if (it != variable_map->end()) {
		auto iter = g_variable_index_map.find(oprd_line_set[1].regName);
		if (iter != g_variable_index_map.end()) {
			string temp = "temp_";
			temp += iter->second;
			it->second = temp;
		}
		else
			it->second = oprd_line_set[1].regName;
	}
	else
		variable_map->insert(make_pair(oprd_line_set[1].regName, oprd_line_set[1].regName));
}

void load_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk) {

	unordered_map<string, string>* variable_map = &stk.top().variable_map;
	auto it = variable_map->find(oprd_line_set[0].regName);
	if (it != variable_map->end()) {

		auto iter = variable_map->find(oprd_line_set[1].regName);
		if (iter != variable_map->end())
			iter->second = it->second;
		else {
			variable_map->insert(make_pair(oprd_line_set[1].regName, it->second));
			if (!isNum(it->first)) {
				an_var_info.location = 0;
				an_var_info.regName = it->second;
				an_var_info.varType = "constant";
				an_var_info.regName = oprd_line_set[1].regName;
			}
		}
	}
}

void store_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, dependecy_info& an_dependency_info, int lineId) {
	if (lineId == -1) {

	}
	else {
		unordered_map<string, string>* variable_map = &stk.top().variable_map;
		unordered_map<string, vector<string>>* variable_dependency_map = &stk.top().variable_dependency_map;
		unordered_map<string, vector<string>>* alloc_variable_dependency_map = &stk.top().alloc_variable_dependency_map;
		vector<string> result_set;


		auto it = variable_map->find(oprd_line_set[0].regName);
		if (it != variable_map->end()) {
			auto iter = g_variable_index_map.find(it->second);
			if (iter != g_variable_index_map.end()) {
				if (oprd_line_set[1].regORnot) {

					auto varIt = variable_map->find(oprd_line_set[1].regName);
					if (varIt != variable_map->end()) {
						//存储的对象是一个变量

						auto varIter = g_variable_index_map.find(varIt->second);
						if (varIter != g_variable_index_map.end()) {

							vector<string> result;
							result.push_back(varIt->second);
							insert(iter->first, result, an_dependency_info, lineId);
						}
						else {
							auto allocIt = alloc_variable_dependency_map->find(varIt->second);
							if (allocIt != alloc_variable_dependency_map->end()) {
								insert(iter->first, allocIt->second, an_dependency_info, lineId);
							}
							else {
								result_set.push_back("other_variables");
								insert(iter->first, result_set, an_dependency_info, lineId);
							}


						}
					}
					else {
						//存储的对象是一个vector
						auto ddIter = variable_dependency_map->find(oprd_line_set[1].regName);
						if (ddIter != variable_dependency_map->end()) {
							insert(iter->first, ddIter->second, an_dependency_info, lineId);
							variable_dependency_map->erase(ddIter);
						}
						else {
							auto allocIt = alloc_variable_dependency_map->find(oprd_line_set[1].regName);
							if (allocIt != alloc_variable_dependency_map->end())
								insert(iter->first, allocIt->second, an_dependency_info, lineId);
						}
					}

				}
				else {
					//若某变量是第一次做存储操作且该变量在指令中直接以变量名的形式出现，则该变量是一个常量
					//cout << "初始化操作，位置：" << dyID << endl;
					result_set.push_back("other_variables");
					insert(iter->first, result_set, an_dependency_info, lineId);
				}

				if (!isNum(oprd_line_set[0].regName)) {
					auto usedIter = an_dependency_info.variable_used_info.find(iter->first);
					if (usedIter == an_dependency_info.variable_used_info.end()) {
						//第四个等级，被赋值的变量是一个常量。
						used_info an_info;
						an_info.line = lineId;
						an_info.times++;
						an_info.init_state = 4;
						an_dependency_info.variable_used_info.insert(make_pair(iter->first, an_info));
					}
				}

				auto usedIter = an_dependency_info.variable_used_info.find(iter->first);

				if (usedIter == an_dependency_info.variable_used_info.end()) {
					used_info an_info;
					an_info.line = lineId;
					an_info.times++;
					an_info.init_state = 2;
					an_dependency_info.variable_used_info.insert(make_pair(iter->first, an_info));
				}
				else {
					if (usedIter->second.factor_state == 1)
						usedIter->second.init_state = 1;
					else {
						if (usedIter->second.line == lineId) {
							if (usedIter->second.init_state != 4) {
								usedIter->second.init_state = 3;
								usedIter->second.times++;
							}

						}
					}
				}
			}
			else {
				auto ddIt = variable_dependency_map->find(oprd_line_set[1].regName);
				if (ddIt != variable_dependency_map->end()) {
					auto ddIter = alloc_variable_dependency_map->find(it->second);
					if (ddIter != alloc_variable_dependency_map->end()) {
						for (string v : ddIt->second) {
							auto resIt = find(ddIter->second.begin(), ddIter->second.end(), v);
							if (resIt == ddIter->second.end())
								ddIter->second.push_back(v);
						}
					}
					else {
						vector<string> result;
						for (string v : ddIt->second)
							result.push_back(v);
						alloc_variable_dependency_map->insert(make_pair(it->second, result));
					}
					variable_dependency_map->erase(ddIt);
				}
				else {
					auto it = variable_map->find(oprd_line_set[1].regName);
					if (it != variable_map->end()) {
						auto iter = g_variable_index_map.find(it->second);
						if (iter != g_variable_index_map.end()) {
							auto usedIter = an_dependency_info.variable_used_info.find(iter->first);

							if (usedIter == an_dependency_info.variable_used_info.end()) {
								used_info an_info;
								an_info.line = lineId;
								an_info.times++;
								an_info.factor_state = 1;
								an_dependency_info.variable_used_info.insert(make_pair(iter->first, an_info));
							}
							else {
								if (usedIter->second.init_state == 2)
									usedIter->second.factor_state = 2;
								else if (usedIter->second.init_state == 3) {
									usedIter->second.factor_state = 3;
								}
							}
						}
					}
				}
			}
		}
	}
}

void alloc_var_dependency_find(vector<string>& result_set, stack<ddd>& stk, string var, bool flag) {
	unordered_map<string, string>* variable_map = &stk.top().variable_map;
	unordered_map<string, vector<string>>* variable_dependency_map = &stk.top().variable_dependency_map;
	unordered_map<string, vector<string>>* alloc_variable_dependency_map = &stk.top().alloc_variable_dependency_map;

	auto allocIt = variable_map->find(var);
	string temp;
	if (allocIt != variable_map->end())
		temp = allocIt->second;
	else
		temp = var;
	auto allocIter = alloc_variable_dependency_map->find(temp);
	if (allocIter != alloc_variable_dependency_map->end()) {
		for (string v : allocIter->second) {
			auto gIt = g_variable_index_map.find(v);
			if (gIt != g_variable_index_map.end())
				result_set.push_back(v);
			else {
				if (flag)
					alloc_var_dependency_find(result_set, stk, v, false);
			}
		}
	}
}

void store_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, dependecy_info& an_dependency_info, int lineId, string bitVar, bool& bit_cast_flag) {
	bit_cast_flag = false;
	if (bitVar == oprd_line_set[1].regName) {
		unordered_map<string, string>* variable_map = &stk.top().variable_map;
		auto it = variable_map->find(oprd_line_set[0].regName);
		auto iter = variable_map->find(oprd_line_set[1].regName);
		if (it != variable_map->end() && iter != variable_map->end()) {
			it->second = iter->second;

			auto usedIter = an_dependency_info.variable_used_info.find(iter->second);
			if (usedIter == an_dependency_info.variable_used_info.end()) {
				used_info an_info;
				an_info.factor_state = 1;
				an_dependency_info.variable_used_info.insert(make_pair(iter->second, an_info));
			}
			else {
				if (usedIter->second.factor_state == 0 && usedIter->second.init_state == 2)
					usedIter->second.factor_state = 2;
				else if (usedIter->second.factor_state == 0 && usedIter->second.init_state == 3)
					usedIter->second.factor_state = 3;
			}
		}
	}
	else
		store_instruction(oprd_line_set, stk, an_dependency_info, lineId);
}

void insert(string res, const vector<string> factor, dependecy_info& an_dependency_info, int lineId) {

	int len = factor.size();
	dependency an_dependency;

	for (int i = 0; i < len; i++) {
		auto it = g_variable_index_map.find(factor[i]);
		if (it != g_variable_index_map.end()) {
			an_dependency.dependency_info_set.push_back(it->second);
			auto usedIter = an_dependency_info.variable_used_info.find(factor[i]);
			if (usedIter == an_dependency_info.variable_used_info.end()) {
				used_info an_info;
				an_info.factor_state = 1;
				//if (factor[i] == "it")
				//	cout << "find" << dyID << endl;
				an_dependency_info.variable_used_info.insert(make_pair(factor[i], an_info));
			}
			else {
				if (usedIter->second.factor_state == 0 && usedIter->second.init_state == 2)
					usedIter->second.factor_state = 2;
				else if (usedIter->second.factor_state == 0 && usedIter->second.init_state == 3)
					usedIter->second.factor_state = 3;
				else if (usedIter->second.factor_state == 0 && usedIter->second.init_state == 4)
					usedIter->second.factor_state = 4;
			}
		}
	}
	auto iter = g_variable_index_map.find(res);
	an_dependency.dependency_info_set.push_back(iter->second);
	an_dependency.line = lineId;
	if (an_dependency.dependency_info_set.size() == 1)
		an_dependency.dependency_info_set.insert(an_dependency.dependency_info_set.begin(), 0);
	an_dependency_info.dependency_set.push_back(an_dependency);
}

void get_element_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk) {

	unordered_map<string, string>* variable_map = &stk.top().variable_map;
	int len = oprd_line_set.size();
	auto it = variable_map->find(oprd_line_set[len - 2].regName);
	if (it != variable_map->end()) {
		auto iter = variable_map->find(oprd_line_set[len - 1].regName);
		if (!isNum(it->first)) {
			an_var_info.location = atoi(oprd_line_set[0].dynValue.c_str()) + 1;
			an_var_info.regName = it->second;
			an_var_info.varType = "array";
			an_var_info.regName = oprd_line_set[len - 1].regName;
		}
		else {
			if (oprd_line_set[len - 2].regName == an_var_info.regName) {
				an_var_info.location = (atoi(oprd_line_set[0].dynValue.c_str()) + 1) * an_var_info.location;
				an_var_info.regName = oprd_line_set[len - 1].regName;
			}
		}

		if (iter != variable_map->end())
			iter->second = it->second;
		else
			variable_map->insert(make_pair(oprd_line_set[len - 1].regName, it->second));
	}
}

void BitCast_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, bool& bit_cast_flag, string& bitVar) {

	unordered_map<string, string>* variable_map = &stk.top().variable_map;
	bit_cast_flag = true;
	bitVar = oprd_line_set[1].regName;
	auto it = variable_map->find(oprd_line_set[0].regName);
	auto iter = variable_map->find(oprd_line_set[1].regName);
	if (it != variable_map->end()) {
		if (iter != variable_map->end())
			iter->second = it->second;
		else
			variable_map->insert(make_pair(oprd_line_set[1].regName, it->second));
	}

}

void intrinsic_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, dependecy_info& an_dependency_info, int lineId, bool& bit_cast_flag) {


	oprd_line an_oprd;
	vector<oprd_line> new_oprd_set;
	bit_cast_flag = false;
	an_oprd.arguId = oprd_line_set[1].arguId;
	an_oprd.dynValue = oprd_line_set[1].dynValue;
	an_oprd.pre_block_id = oprd_line_set[1].pre_block_id;
	an_oprd.regName = oprd_line_set[1].regName;
	an_oprd.regORnot = oprd_line_set[1].regORnot;
	an_oprd.sizeOfArgu = oprd_line_set[1].sizeOfArgu;
	new_oprd_set.push_back(an_oprd);

	an_oprd.arguId = oprd_line_set[2].arguId;
	an_oprd.dynValue = oprd_line_set[2].dynValue;
	an_oprd.pre_block_id = oprd_line_set[2].pre_block_id;
	an_oprd.regName = oprd_line_set[2].regName;
	an_oprd.regORnot = oprd_line_set[2].regORnot;
	an_oprd.sizeOfArgu = oprd_line_set[2].sizeOfArgu;
	new_oprd_set.push_back(an_oprd);

	store_instruction(new_oprd_set, stk, an_dependency_info, lineId);
}

void calculation_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk) {

	unordered_map<string, string>* variable_map = &stk.top().variable_map;
	unordered_map<string, vector<string>>* variable_dependency_map = &stk.top().variable_dependency_map;
	vector<string> result_map;

	auto it = variable_map->find(oprd_line_set[0].regName);
	auto iter = variable_map->find(oprd_line_set[1].regName);
	if (it != variable_map->end() && iter != variable_map->end()) {
		result_map.push_back(it->second);
		result_map.push_back(iter->second);
	}
	else if (it != variable_map->end()) {
		auto resIter = variable_dependency_map->find(oprd_line_set[1].regName);
		if (resIter != variable_dependency_map->end()) {
			for (string v : resIter->second)
				result_map.push_back(v);
		}
		result_map.push_back(it->second);
	}
	else if (iter != variable_map->end()) {
		auto resIter = variable_dependency_map->find(oprd_line_set[0].regName);
		if (resIter != variable_dependency_map->end()) {
			for (string v : resIter->second)
				result_map.push_back(v);
		}
		result_map.push_back(iter->second);
	}
	else {
		auto resIt = variable_dependency_map->find(oprd_line_set[0].regName);
		auto resIter = variable_dependency_map->find(oprd_line_set[1].regName);
		if (resIt != variable_dependency_map->end()) {
			for (string v : resIt->second)
				result_map.push_back(v);
		}
		if (resIter != variable_dependency_map->end()) {
			for (string v : resIter->second)
				result_map.push_back(v);
		}
	}
	variable_dependency_map->insert(make_pair(oprd_line_set[2].regName, result_map));
}

void call_instruction(const vector<oprd_line>& oprd_line_set, stack<ddd>& stk, const unordered_map<string, string>& in_loop_map, bool creatFlag) {

	int len = oprd_line_set.size();
	if (creatFlag) {
		//if (dyID > 232763 && dyID < 301406)
		//	cout << dyID << endl;
		if (oprd_line_set[len - 1].arguId == "f") {
			unordered_map<string, string> variable_map = stk.top().variable_map;
			create_map(stk);
			unordered_map<string, string>* new_variable_map = &stk.top().variable_map;
			for (int i = 1; i <= len / 2; i++) {

				auto it = variable_map.find(oprd_line_set[i].regName);
				auto iter = new_variable_map->find(oprd_line_set[i + len / 2].regName);
				if (it != variable_map.end()) {
					if (iter != new_variable_map->end())
						iter->second = it->second;
					else
						new_variable_map->insert(make_pair(oprd_line_set[i + len / 2].regName, it->second));
				}
				else {
					auto inLoopIt = in_loop_map.find(oprd_line_set[i].dynValue);
					if (inLoopIt != in_loop_map.end())
						new_variable_map->insert(make_pair(oprd_line_set[i + len / 2].regName, inLoopIt->second));
					else
						new_variable_map->insert(make_pair(oprd_line_set[i + len / 2].regName, oprd_line_set[i + len / 2].regName));
				}
			}
		}
		else if (oprd_line_set[len - 1].arguId == "r") {
			unordered_map<string, string>* variable_map = &stk.top().variable_map;
			if (oprd_line_set.size() >= 3) {
				auto it = variable_map->find(oprd_line_set[len - 2].regName);
				auto iter = variable_map->find(oprd_line_set[len - 1].regName);
				if (it != variable_map->end()) {
					if (iter != variable_map->end())
						iter->second = it->second;
					else
						variable_map->insert(make_pair(oprd_line_set[len - 2].regName, it->second));
				}
			}
			create_map(stk);
		}
		else {
			create_map(stk);
		}
		creatFlag = false;
	}
	else {
		unordered_map<string, string>* variable_map = &stk.top().variable_map;
		if (oprd_line_set[len - 1].arguId == "r") {
			if (oprd_line_set.size() >= 3) {
				auto it = variable_map->find(oprd_line_set[len - 2].regName);
				auto iter = variable_map->find(oprd_line_set[len - 1].regName);
				if (it != variable_map->end()) {
					if (iter != variable_map->end())
						iter->second = it->second;
					else
						variable_map->insert(make_pair(oprd_line_set[len - 2].regName, it->second));
				}
			}
		}
	}

}


void get_graph(const vector<dependecy_info>& dependency_set, unordered_map<string, var_loc>& var_location) {

	size_t iterations_len = dependency_set.size();
	int line;
	int a, c;
	string v_c, v_a;
	string c_str, a_str;
	stringstream ss;
	string relation;
	vector<string> cpk_set;
	std::cout << "Begin writing the diagram to file..." << endl;
	write_graph("strict digraph {");
	for (size_t i = 0; i < iterations_len; i++)
	{
		dependecy_info an_iteration = dependency_set[i];
		size_t dependency_len = an_iteration.dependency_set.size();
		vector<string> ddg;
		unordered_map<string, variable_lable> lable_map;
		set<string> dependency_with_other;
		unordered_map<string, int> cpk_map;	    //0 - write; 1 - read; 2 W and R;

		string graph_lable = "cluster_";
		ss << graph_lable << to_string(i);
		ss >> graph_lable;
		ss.str("");
		ss.clear();
		string sub_graph = "subgraph " + graph_lable + "{";
		ddg.push_back(sub_graph);
		for (size_t j = 0; j < dependency_len; j++)
		{
			line = an_iteration.dependency_set[j].line;
			vector<int> an_dependency = an_iteration.dependency_set[j].dependency_info_set;
			size_t node_size = an_dependency.size();
			c = an_dependency[node_size - 1];
			v_c = g_index_variable_map.find(c)->second;


			//去掉0以及收集与其他变量之间的dependency
			vector<string> selected_dependency;
			int flag = 0;
			for (size_t n = 0; n < an_dependency.size() - 1; n++) {
				if (an_dependency[n] == 0) {
					if (an_dependency.size() == 2)
						dependency_with_other.insert(g_index_variable_map.find(an_dependency[1])->second);
				}
				else if (an_dependency[n] != an_dependency[node_size - 1])
					selected_dependency.push_back(g_index_variable_map.find(an_dependency[n])->second);
				else if (an_dependency[n] == an_dependency[node_size - 1] && flag == 0) {
					selected_dependency.push_back(g_index_variable_map.find(an_dependency[n])->second);
					flag++;
				}
			}

			//检测变量c是否需要C/R
			if (!cpk_map.count(v_c))
				cpk_map.insert(make_pair(v_c, 0));
			else {
				auto it = cpk_map.find(v_c);
				if (it->second == 1)
					it->second = 2;
			}

			//检测变量a是否需要C/R
			for (size_t k = 0; k < selected_dependency.size(); k++) {
				v_a = selected_dependency[k];
				if (!cpk_map.count(v_a))
					cpk_map.insert(make_pair(v_a, 1));
				else {
					auto it = cpk_map.find(v_a);
					if (it->second == 0)
						it->second = 2;
				}
			}

			vector<string> temp;

			//设置一个标志，检测是否存在变量c指向自己
			bool add_node_c = false;
			for (size_t k = 0; k < selected_dependency.size(); k++)
			{
				v_a = selected_dependency[k];
				if (v_a == v_c) {
					if (lable_map.count(v_a)) {
						auto it = lable_map.find(v_a);
						if (!it->second.point_to_self) {
							a_str = it->second.lable;
							it->second.read = true;
							it->second.point_to_self = true;
							add_node_c = true;
							ss << a_str << "->";
							temp.push_back(ss.str());
							ss.str("");
							ss.clear();
						}
					}
					else
					{
						variable_lable an_variable;
						ss << v_a << line << "self" << to_string(i);
						ss >> a_str;
						ss.str("");
						ss.clear();
						an_variable.lable = a_str;
						an_variable.read = true;
						lable_map.insert(make_pair(v_a, an_variable));
						lable_map.find(v_a)->second.point_to_self = true;
						add_node_c = true;
						ss << a_str << "->";
						temp.push_back(ss.str());
						ss.str("");
						ss.clear();
					}

				}
				else
				{
					if (lable_map.count(v_a)) {
						auto it = lable_map.find(v_a);
						a_str = it->second.lable;
						it->second.read = true;
					}
					else
					{
						variable_lable an_variable_lable;
						ss << v_a << line << "_" << to_string(i);
						ss >> a_str;
						ss.str("");
						ss.clear();
						an_variable_lable.lable = a_str;
						an_variable_lable.read = true;
						lable_map.insert(make_pair(v_a, an_variable_lable));
					}
					ss << a_str << "->";
					temp.push_back(ss.str());
					ss.str("");
					ss.clear();
				}
			}


			if (temp.size() > 0) {
				//寻找或创建变量c的标签
				if (add_node_c) {
					//写入第一个节点
					auto it = lable_map.find(v_c);
					ss << it->second.lable << "[label=" << v_c << "];";
					ddg.push_back(ss.str());
					ss.str("");
					ss.clear();

					//写入第二个节点
					ss << v_c << line << "_" << to_string(i);
					ss >> c_str;
					ss.str("");
					ss.clear();
					it->second.lable = c_str;
					it->second.if_write_label = true;
					it->second.read = false;
					ss << c_str << "[label=" << v_c << "];";
					ddg.push_back(ss.str());
					ss.str("");
					ss.clear();
				}
				else
				{
					if (lable_map.count(v_c)) {
						auto it = lable_map.find(v_c);
						if (it->second.read) {
							//变量被读取过，生成一个新的节点
							ss << v_c << line << "_" << to_string(i);
							ss >> c_str;
							ss.str("");
							ss.clear();
							it->second.lable = c_str;
							it->second.read = false;
							it->second.if_write_label = true;
							ss << c_str << "[label=" << v_c << "];";
							ddg.push_back(ss.str());
							ss.str("");
							ss.clear();
						}
						else
						{
							c_str = it->second.lable;
						}
					}
					else
					{
						variable_lable an_variable_lable;
						ss << v_c << line << "_" << to_string(i);
						ss >> c_str;
						ss.str("");
						ss.clear();
						an_variable_lable.lable = c_str;
						an_variable_lable.if_write_label = true;
						lable_map.insert(make_pair(v_c, an_variable_lable));
						ss << c_str << "[label=" << v_c << "];";
						ddg.push_back(ss.str());
						ss.str("");
						ss.clear();
					}
				}

				//写入变量a的label
				for (size_t i = 0; i < selected_dependency.size(); i++)
				{
					auto it = lable_map.find(selected_dependency[i]);
					if (!it->second.if_write_label) {
						ss << it->second.lable << "[label=" << selected_dependency[i] << "];";
						ddg.push_back(ss.str());
						ss.str("");
						ss.clear();
					}
				}

				//写入DDG
				for (size_t i = 0; i < temp.size(); i++)
				{
					ss << temp[i] << c_str << ";" << endl;
					ddg.push_back(ss.str());
					ss.str("");
					ss.clear();
				}
			}
		}

		int variable_nums = cpk_set.size();
		unordered_map<string, used_info> variable_used = dependency_set[i].variable_used_info;
		get_critical_variables(cpk_map, cpk_set, variable_used, dependency_with_other, var_location);
		if (cpk_set.size() > variable_nums) {
			//生成DDG
			for (size_t i = 0; i < ddg.size(); i++)
			{
				write_graph(ddg[i]);
			}
			write_graph("}");
		}
	}


	vector<string> locations;
	//get all variables for checkpoting
	write_graph("subgraph cluster_end {");
	for (int n = 0; n < cpk_set.size(); n++) {
		if (cpk_set[n] != index_variable) {
			string node;
			node = cpk_set[n] + "[color=purple]";
			write_graph(node);
			auto var_iter = var_location.find(cpk_set[n]);
			if (var_iter != var_location.end()) {
				ss << "Variable: " << cpk_set[n] << ", Function Name: " << var_iter->second.function << ", Line Num: " << var_iter->second.line_num;
				locations.push_back(ss.str());
				ss.str("");
				ss.clear();
			}
		}
	}
	ss << "Index: " << index_variable << ", Function Name: " << loc_function << ", Line Num: " << start_line_num;
	locations.push_back(ss.str());
	ss.str("");
	ss.clear();
	index_variable += "[color=yellow]";
	write_graph(index_variable);
	write_graph("}");
	write_graph("}");
	write_location(locations);
}





void get_critical_variables(unordered_map<string, int> cpk_map, vector<string>& cpk_set, unordered_map<string, used_info> variable_used, set<string> dependency_with_other, unordered_map<string, var_loc>& var_location) {
	for (auto it = cpk_map.begin(); it != cpk_map.end(); it++) {
		if (it->second == 2 || it->second == 0)
		{
			auto iter = find(cpk_set.begin(), cpk_set.end(), it->first);
			auto usedIt = variable_used.find(it->first);
			if (iter == cpk_set.end() && ((usedIt->second.factor_state == 1 && usedIt->second.init_state == 1) || (usedIt->second.factor_state == 2 && usedIt->second.init_state == 2)))
				cpk_set.push_back(it->first);
		}
	}


	for (auto it = dependency_with_other.begin(); it != dependency_with_other.end(); it++)
	{
		if (find(cpk_set.begin(), cpk_set.end(), *it) == cpk_set.end()) {
			auto usedIt = variable_used.find(*it);
			if (usedIt->second.factor_state != 3 && usedIt->second.init_state != 3 && usedIt->second.init_state != 4)
				cpk_set.push_back(*it);
		}
	}
}



bool first_write = true;
void write_graph(string str) {
	ofstream outfile;
	if (first_write) {
		outfile.open("graph.dot", ios::out | ios::trunc);
		first_write = false;
	}
	else
		outfile.open("graph.dot", ios::out | ios::app);
	outfile << str << endl;
	outfile.close();
}


void write_location(vector<string>& locations) {

	time_t now = time(0);
	tm* ltm = localtime(&now);

	stringstream ss;
	string file_name;
	int i = benchmark_name.size() - 1;
	while (benchmark_name[i] != '/') {
		i--;
	}
	file_name = benchmark_name.substr(i + 1);

	ss << "./result/" << file_name << "_" << 1900 + ltm->tm_year << "_" << 1 + ltm->tm_mon << "_" << ltm->tm_mday << "_" << ltm->tm_hour << "_" << ltm->tm_min << "_" << ltm->tm_sec;
	ss >> file_name;
	ss.str("");
	ss.clear();


	ofstream outfile;
	if (first_write) {
		outfile.open(file_name, ios::out | ios::trunc);
		first_write = false;
	}
	else
		outfile.open(file_name, ios::out | ios::app);
	for (int i = 0; i < locations.size(); ++i)
	{
		outfile << locations[i] << endl;
	}
	outfile.close();
}
