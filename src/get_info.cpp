#include "get_info.h"

void get_parameter(string& is_enable_openMP, string& path, string& function, int& call_loc, int& start_line_num, int& end_line_num) {

	char* ENABLE_OPENMP, * TRACE_FILE_HOME, * FUNCTION_NAME, * CALL_LOCATION, * START_LINE, * END_LINE, * SUB_FILE_HOME;
	ENABLE_OPENMP = getenv("ENABLE_OPENMP");
	FUNCTION_NAME = getenv("FUNCTION_NAME");
	CALL_LOCATION = getenv("CALL_LOCATION");
	START_LINE = getenv("START_LINE");
	END_LINE = getenv("END_LINE");

	is_enable_openMP = ENABLE_OPENMP;
	function = FUNCTION_NAME;
	call_loc = atoi(CALL_LOCATION);
	start_line_num = atoi(START_LINE);
	end_line_num = atoi(END_LINE);

	if (is_enable_openMP == "on") {
		SUB_FILE_HOME = getenv("SUB_FILE_HOME");
		path = SUB_FILE_HOME;
	}
	else {
		TRACE_FILE_HOME = getenv("TRACE_FILE_HOME");
		path = TRACE_FILE_HOME;
	}
}

void get_info(string path, string& function_name, int call_loc, int start_line_num, int end_line_num, unordered_map<string, string>& in_loop_map, vector<instr_info>& instr_info_set, unordered_map<string, var_loc>& var_location) {

	ifstream in;
	string outfile = path;
	in.open(outfile.c_str(), ifstream::in);

	if (!in) {
		cerr << "ERROR IN OPENNING THE TRACEFILE!" << endl;
		return;
	}

	string line;
	int line_num;
	string current_function;
	int opencode = 0;
	int dynInstId;
	int n = -1;
	int k = -1;
	bool before_loop = true;
	bool inside_loop = false;
	vector<instr_info> before_loop_instr_set;
	vector<instr_info> index_instr_set;
	unordered_map<string, string> before_loop_map;
	set<string> global_variable_set;
	stack<set<string>> local_variable;
	printf("Begin reading the instruction trace...\n");
	//good()表示文件流是否正常，eof表示文件流是否到结束了

	get_before_loop_and_index_instrction(in, before_loop_instr_set, index_instr_set, n, function_name, start_line_num, opencode);
	for (size_t i = 0; i < index_instr_set.size(); i++)
	{
		instr_info_set.push_back(index_instr_set[i]);
		k++;
	}
	get_inside_loop_instr(in, instr_info_set, k, function_name, end_line_num, opencode);

	get_variables(before_loop_instr_set, before_loop_map, call_loc);
	get_variables(instr_info_set, in_loop_map, local_variable, global_variable_set, var_location);
	match(before_loop_map, in_loop_map, global_variable_set);
}

void get_instruction(string line, vector<instr_info>& instr_info_set, int& n) {
	//得到loop中的指令

	size_t NumOfComma1 = 0;
	size_t NumOfComma2 = line.find(",", NumOfComma1);
	string first_flag = line.substr(NumOfComma1, NumOfComma2).c_str();
	if (first_flag == "0") {
		n++;
		instr_info an_instr_info;
		instr_info_set.push_back(an_instr_info);

		instr_info_set[n].blockId = line.substr(NumOfComma1, NumOfComma2).c_str();

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		//line-number
		instr_info_set[n].lineId = atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str());

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		//function-name
		instr_info_set[n].funcName = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		//basic-block-id
		instr_info_set[n].basicBlockId = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		//inst-id
		instr_info_set[n].staticInstId = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		//opcode
		instr_info_set[n].opcodeId = atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str());

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		//dynamic-inst-id
		instr_info_set[n].dynInstId = atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str());
	}
	else {
		oprd_line an_oprd_line;

		//argument ID
		an_oprd_line.arguId = line.substr(NumOfComma1, NumOfComma2).c_str();

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		an_oprd_line.sizeOfArgu = atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str());

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		an_oprd_line.dynValue = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		an_oprd_line.regORnot = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()) == 1);

		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		an_oprd_line.regName = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

		// for '-1' trace blocks, there is a predecesser block id here; for other blocks, here is empty
		NumOfComma1 = NumOfComma2;
		NumOfComma2 = line.find(",", NumOfComma1 + 1);
		an_oprd_line.pre_block_id = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

		instr_info_set[n].oprd_line_set.push_back(an_oprd_line);
	}
}

bool isNum(string str)
//识别是否是数字
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

void get_variables(vector<instr_info>& instr_info_set, unordered_map<string, string>& variable_map, int call_loc) {
	int opcodeId;
	int set_size = instr_info_set.size();
	for (size_t i = 0; i < set_size; i++)
	{
		instr_info inf = instr_info_set[i];
		opcodeId = inf.opcodeId;
		int loc;
		size_t j;
		switch (opcodeId)
		{
		case 27: opcode_operation(inf.oprd_line_set[0].dynValue, inf.oprd_line_set[0].regName, variable_map); break;
		case 28:
			if (inf.lineId != -1)
				opcode_operation(inf.oprd_line_set[0].dynValue, inf.oprd_line_set[0].regName, variable_map);
			break;
		case 29:
			loc = inf.oprd_line_set.size() - 2;
			opcode_operation(inf.oprd_line_set[loc].dynValue, inf.oprd_line_set[loc].regName, variable_map);
			break;
		case 49:
			if (inf.lineId == call_loc) {
				for (j = 1; j < inf.oprd_line_set.size(); j++)
				{
					opcode_operation(inf.oprd_line_set[j].dynValue, inf.oprd_line_set[j].regName, variable_map);
				}
				j = 1;
			}

			break;
		default:
			break;
		}
	}
}

void get_variables(vector<instr_info>& instr_info_set, unordered_map<string, string>& variable_map, stack<set<string>>& local_variable, set<string>& global_variable_set, unordered_map<string, var_loc>& var_location) {
	int opcodeId;
	int set_size = instr_info_set.size();

	for (size_t i = 0; i < set_size; i++) {
		instr_info inf = instr_info_set[i];
		opcodeId = inf.opcodeId;

		if (opcodeId == 49) {
			set<string> an_set;
			local_variable.push(an_set);
		}
		else if (opcodeId == 1) {
			if (!local_variable.empty())
				local_variable.pop();
		}
		int loc;
		size_t j;
		switch (opcodeId)
		{

		case 26:
			if (!isNum(inf.oprd_line_set[1].regName)) {
				local_variable.top().insert(inf.oprd_line_set[1].regName);
			}
		case 27: opcode_operation(inf.oprd_line_set[0].dynValue, inf.oprd_line_set[0].regName, variable_map, local_variable, var_location, inf.funcName, inf.lineId); break;
		case 28:
			if (inf.lineId != -1)
				opcode_operation(inf.oprd_line_set[0].dynValue, inf.oprd_line_set[0].regName, variable_map, local_variable, var_location, inf.funcName, inf.lineId);
			else
				local_variable.top().insert(inf.oprd_line_set[1].regName);
			break;
		case 29:
			loc = inf.oprd_line_set.size() - 2;
			opcode_operation(inf.oprd_line_set[loc].dynValue, inf.oprd_line_set[loc].regName, variable_map, local_variable, var_location, inf.funcName, inf.lineId);
			break;
		case 49:
			for (j = 1; j < inf.oprd_line_set.size(); j++)
			{
				if (inf.oprd_line_set[j].regName == " ")
					global_variable_set.insert(inf.oprd_line_set[j].dynValue);
				else
					opcode_operation(inf.oprd_line_set[j].dynValue, inf.oprd_line_set[j].regName, variable_map, local_variable, var_location, inf.funcName, inf.lineId);
			}
			j = 1;
			break;
		default:
			break;
		}
	}
}

void opcode_operation(string dynValue, string variable, unordered_map<string, string>& variable_map) {
	if (!isNum(variable))
	{
		auto it = variable_map.find(dynValue);
		if (it == variable_map.end())
			variable_map.insert(make_pair(dynValue, variable));
		else
			it->second = variable;
	}
}

void opcode_operation(string dynValue, string variable, unordered_map<string, string>& variable_map, stack<set<string>>& local_variable, unordered_map<string, var_loc>& var_location, string function, int line_num) {
	if (local_variable.empty()) {
		if (!isNum(variable))
		{
			auto it = variable_map.find(dynValue);
			if (it == variable_map.end())
				variable_map.insert(make_pair(dynValue, variable));
			else
				it->second = variable;

			auto iter = var_location.find(variable);
			if (iter == var_location.end()) {
				var_loc an_var;
				an_var.function = function;
				an_var.line_num = line_num;
				var_location.insert(make_pair(variable, an_var));
			}
			else
			{
				iter->second.function = function;
				iter->second.line_num = line_num;
			}
		}
	}
	else {
		if (local_variable.top().find(variable) == local_variable.top().end())
		{
			if (!isNum(variable))
			{
				auto it = variable_map.find(dynValue);
				if (it == variable_map.end())
					variable_map.insert(make_pair(dynValue, variable));
				else
					it->second = variable;

				auto iter = var_location.find(variable);
				if (iter == var_location.end()) {
					var_loc an_var;
					an_var.function = function;
					an_var.line_num = line_num;
					var_location.insert(make_pair(variable, an_var));
				}
				else
				{
					iter->second.function = function;
					iter->second.line_num = line_num;
				}
			}
		}
	}
}


void match(unordered_map<string, string> before_loop_map, unordered_map<string, string>& in_loop_map, set<string>& global_variable_set) {

	unordered_map<string, string>::iterator iter;
	iter = in_loop_map.begin();
	while (iter != in_loop_map.end()) {
		if (iter->second == "" || iter->second == " ") {
			iter = in_loop_map.erase(iter);
		}
		else {
			auto it = before_loop_map.find(iter->first);
			if (it != before_loop_map.end()) {
				if (iter->second.compare(it->second) == 0) {
					iter++;
				}
				else {
					iter = in_loop_map.erase(iter);
				}
			}
			else {
				iter = in_loop_map.erase(iter);
			}
		}
	}
	auto it = global_variable_set.begin();
	while (it != global_variable_set.end())
	{
		if (before_loop_map.find(*it) != before_loop_map.end() && before_loop_map.find(*it)->second != " " && in_loop_map.find(*it) == in_loop_map.end())
			in_loop_map.insert(make_pair(*it, before_loop_map.find(*it)->second));
		it++;
	}
}


void traver_map(unordered_map<string, string> in_loop_map) {
	unordered_map<string, string>::iterator iter;
	for (iter = in_loop_map.begin(); iter != in_loop_map.end(); iter++) {
		string s = iter->first;
		cout << s << "\t" << iter->second << endl;
	}
}



void get_c_plus(string path, string& function_name, int call_loc, int start_line_num, int end_line_num, unordered_map<string, string>& in_loop_map, vector<instr_info>& instr_info_set, unordered_map<string, var_loc>& var_location) {

	ifstream in;
	string outfile = path;
	in.open(outfile.c_str(), ifstream::in);

	if (!in) {
		cerr << "ERROR IN OPENNING THE TRACEFILE!" << endl;
		return;
	}

	string line;
	int line_num;
	string current_function;
	int opencode = 0;
	int dynInstId;
	int n = -1;
	int k = -1;
	bool before_loop = true;
	bool inside_loop = false;
	vector<instr_info> before_loop_instr_set;
	vector<instr_info> index_instr_set;
	unordered_map<string, string> before_loop_map;
	set<string> global_variable_set;
	stack<set<string>> local_variable;
	printf("Begin reading the instruction trace...\n");
	get_function_name(in, before_loop_instr_set, n, function_name, call_loc);
	get_before_loop_and_index_instrction(in, before_loop_instr_set, index_instr_set, n, function_name, start_line_num, opencode);
	for (size_t i = 0; i < index_instr_set.size(); i++)
	{
		instr_info_set.push_back(index_instr_set[i]);
		k++;
	}
	get_inside_loop_instr(in, instr_info_set, k, function_name, end_line_num, opencode);
	get_variables(before_loop_instr_set, before_loop_map, call_loc);
	get_variables(instr_info_set, in_loop_map, local_variable, global_variable_set, var_location);
	match(before_loop_map, in_loop_map, global_variable_set);
}


void get_function_name(ifstream& in, vector<instr_info>& before_loop_instr_set, int& n, string& function_name, int call_loc) {

	//bool flag = true;
	string line;
	int line_num;
	string current_function;
	int opencode = 0;
	int dynInstId;
	while (in.good()) {
		// read some info from the trace file
		getline(in, line);
		size_t yes_no, NumOfComma1, NumOfComma2;
		yes_no = line.find(",", 0);
		vector<string> temp;
		int temp_size = 0;
		if (yes_no == string::npos)
			//未找到字符串 
			continue;
		else {
			NumOfComma1 = 0;
			NumOfComma2 = line.find(",", NumOfComma1);
			string blockId_abnom = line.substr(NumOfComma1, NumOfComma2).c_str();
			if (blockId_abnom == "0") {
				//查找后面的函数名，定义一个flag
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				line_num = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				current_function = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				opencode = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				dynInstId = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));

				if (opencode == 49 && line_num == call_loc) {
					temp.push_back(line);
					//get_instruction(line, before_loop_instr_set, n);
					getline(in, line);
					temp.push_back(line);
					//get_instruction(line, before_loop_instr_set, n);
					NumOfComma1 = 0;
					NumOfComma2 = line.find(",", NumOfComma1);

					NumOfComma1 = NumOfComma2;
					NumOfComma2 = line.find(",", NumOfComma1 + 1);

					NumOfComma1 = NumOfComma2;
					NumOfComma2 = line.find(",", NumOfComma1 + 1);

					NumOfComma1 = NumOfComma2;
					NumOfComma2 = line.find(",", NumOfComma1 + 1);

					NumOfComma1 = NumOfComma2;
					NumOfComma2 = line.find(",", NumOfComma1 + 1);
					string reg_name = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();

					string::size_type idx;
					idx = reg_name.find(function_name);//在reg_name中查找function_name.
					if (idx != string::npos) {
						get_instruction(temp[0], before_loop_instr_set, n);
						get_instruction(line, before_loop_instr_set, n);
						function_name = reg_name;
						while (yes_no != string::npos)
						{
							getline(in, line);
							yes_no = line.find(",", 0);
							if (yes_no != string::npos)
								get_instruction(line, before_loop_instr_set, n);
						}
						break;
					}
				}

				switch (opencode)
				{
				case 1:get_instruction(line, before_loop_instr_set, n); break;
				case 26:get_instruction(line, before_loop_instr_set, n); break;
				case 27:get_instruction(line, before_loop_instr_set, n); break;
				case 28:get_instruction(line, before_loop_instr_set, n); break;
				case 29:get_instruction(line, before_loop_instr_set, n); break;
				case 49:
					if (!temp.empty())
						for (temp_size = 0; temp_size < temp.size(); temp_size++)
						{
							get_instruction(temp[temp_size], before_loop_instr_set, n);
						}
					temp.clear();
					break;

				default:
					break;
				}
			}
			else
			{
				switch (opencode)
				{
				case 1:get_instruction(line, before_loop_instr_set, n); break;
				case 26:get_instruction(line, before_loop_instr_set, n); break;
				case 27:get_instruction(line, before_loop_instr_set, n); break;
				case 28:get_instruction(line, before_loop_instr_set, n); break;
				case 29:get_instruction(line, before_loop_instr_set, n); break;
				case 49:get_instruction(line, before_loop_instr_set, n); break;
				default:
					break;
				}
			}
		}
	}
}

void get_before_loop_and_index_instrction(ifstream& in, vector<instr_info>& before_loop_instr_set, vector<instr_info>& index_instr_set, int& n, string& function_name, int start_line_num, int& opencode) {

	string line;
	int line_num;
	bool before_loop_flag = true;
	string current_function;
	int x = -1;
	int dynInstId;

	while (in.good()) {
		// read some info from the trace file
		getline(in, line);
		size_t yes_no, NumOfComma1, NumOfComma2;
		yes_no = line.find(",", 0);
		vector<string> temp;
		int temp_size = 0;
		if (yes_no == string::npos)
			//未找到字符串 
			continue;
		else {
			NumOfComma1 = 0;
			NumOfComma2 = line.find(",", NumOfComma1);
			string blockId_abnom = line.substr(NumOfComma1, NumOfComma2).c_str();
			if (blockId_abnom == "0") {
				//查找后面的函数名，定义一个flag
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				line_num = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				current_function = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				opencode = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				dynInstId = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));

				if (line_num == start_line_num && function_name == current_function)
				{
					before_loop_flag = false;
				}

				if (line_num > start_line_num && function_name == current_function)
				{
					get_instruction(line, index_instr_set, x);
					break;
				}

				if (before_loop_flag)
				{
					switch (opencode)
					{
					case 1:get_instruction(line, before_loop_instr_set, n); break;
					case 26:get_instruction(line, before_loop_instr_set, n); break;
					case 27:get_instruction(line, before_loop_instr_set, n); break;
					case 28:get_instruction(line, before_loop_instr_set, n); break;
					case 29:get_instruction(line, before_loop_instr_set, n); break;
					case 49:get_instruction(line, before_loop_instr_set, n); break;
					default:
						break;
					}
				}
				else
				{
					before_loop_flag = false;
					get_instruction(line, index_instr_set, x);
				}
			}
			else {
				if (before_loop_flag) {
					switch (opencode)
					{
					case 1:get_instruction(line, before_loop_instr_set, n); break;
					case 26:get_instruction(line, before_loop_instr_set, n); break;
					case 27:get_instruction(line, before_loop_instr_set, n); break;
					case 28:get_instruction(line, before_loop_instr_set, n); break;
					case 29:get_instruction(line, before_loop_instr_set, n); break;
					case 49:get_instruction(line, before_loop_instr_set, n); break;
					default:
						break;
					}
				}
				else
				{
					get_instruction(line, index_instr_set, x);
				}

			}
		}
	}
}

void get_inside_loop_instr(ifstream& in, vector<instr_info>& instr_info_set, int& k, string& function_name, int end_line_num, int opencode) {

	string line;
	int line_num;
	bool inside_loop_flag = true;
	string current_function;
	int dynInstId;


	while (in.good()) {
		// read some info from the trace file
		getline(in, line);

		size_t yes_no, NumOfComma1, NumOfComma2;
		yes_no = line.find(",", 0);
		vector<string> temp;
		int temp_size = 0;
		if (yes_no == string::npos)
			//未找到字符串 
			continue;
		else {
			NumOfComma1 = 0;
			NumOfComma2 = line.find(",", NumOfComma1);
			string blockId_abnom = line.substr(NumOfComma1, NumOfComma2).c_str();
			if (blockId_abnom == "0") {
				//查找后面的函数名，定义一个flag
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				line_num = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				current_function = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				opencode = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				dynInstId = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));

				if (function_name == current_function && line_num > end_line_num) {
					break;
				}

				switch (opencode)
				{
				case 1:get_instruction(line, instr_info_set, k); break;
				case 8:get_instruction(line, instr_info_set, k); break;
				case 9:get_instruction(line, instr_info_set, k); break;
				case 10:get_instruction(line, instr_info_set, k); break;
				case 11:get_instruction(line, instr_info_set, k); break;
				case 12:get_instruction(line, instr_info_set, k); break;
				case 13:get_instruction(line, instr_info_set, k); break;
				case 14:get_instruction(line, instr_info_set, k); break;
				case 15:get_instruction(line, instr_info_set, k); break;
				case 16:get_instruction(line, instr_info_set, k); break;
				case 26:get_instruction(line, instr_info_set, k); break;
				case 27:get_instruction(line, instr_info_set, k); break;
				case 28:get_instruction(line, instr_info_set, k); break;
				case 29:get_instruction(line, instr_info_set, k); break;
				case 44:get_instruction(line, instr_info_set, k); break;
				case 49:get_instruction(line, instr_info_set, k); break;
				case 104:get_instruction(line, instr_info_set, k); break;
				default:
					break;
				}

			}
			else
			{
				switch (opencode)
				{
				case 1:get_instruction(line, instr_info_set, k); break;
				case 8:get_instruction(line, instr_info_set, k); break;
				case 9:get_instruction(line, instr_info_set, k); break;
				case 10:get_instruction(line, instr_info_set, k); break;
				case 11:get_instruction(line, instr_info_set, k); break;
				case 12:get_instruction(line, instr_info_set, k); break;
				case 13:get_instruction(line, instr_info_set, k); break;
				case 14:get_instruction(line, instr_info_set, k); break;
				case 15:get_instruction(line, instr_info_set, k); break;
				case 16:get_instruction(line, instr_info_set, k); break;
				case 26:get_instruction(line, instr_info_set, k); break;
				case 27:get_instruction(line, instr_info_set, k); break;
				case 28:get_instruction(line, instr_info_set, k); break;
				case 29:get_instruction(line, instr_info_set, k); break;
				case 44:get_instruction(line, instr_info_set, k); break;
				case 49:get_instruction(line, instr_info_set, k); break;
				case 104:get_instruction(line, instr_info_set, k); break;
				default:
					break;
				}
			}
		}
	}
}




//OMP
void get_info_omp(int num_th, int max_file_id, string file_names, int start_line_num, int end_line_num, string function, int call_loc, unordered_map<string, string>& inside_loop_map, vector<instr_info>& inside_loop_instr_set, unordered_map<string, var_loc>& variable_loc) {
	vector<vector<instr_info>> instr_info_set;
	vector<unordered_map<int, int>> start_set;
	vector<unordered_map<int, int>> stop_set;

	int half_th_num = num_th / 2;
	for (size_t i = 0; i < num_th; i++)
	{
		vector<instr_info> an_private_instr_set;
		unordered_map<int, int> an_start_map;
		unordered_map<int, int> an_stop_map;
		instr_info_set.push_back(an_private_instr_set);
		start_set.push_back(an_start_map);
		stop_set.push_back(an_stop_map);
	}
	float startTime = omp_get_wtime();
	printf("Begin reading the instruction trace...\n");
#pragma omp parallel for num_threads(half_th_num)
	for (size_t i = 0; i < num_th; i++)
	{
		stringstream ss;
		if (i > 9)
			ss << file_names << "/file" << i;
		else
		{
			ss << file_names << "/file" << 0 << i;
		}
		string path = ss.str();
		ss.str() = ("");
		ss.clear();
		get_all_instr_omp(instr_info_set[i], path, start_line_num, end_line_num);
	}

	int call_dyn_id, call_file_id;
	if (call_loc != 0) {
#pragma omp parallel for num_threads(half_th_num)
		for (size_t i = 0; i < num_th; i++)
		{
			get_function_name_omp(instr_info_set[i], function, call_loc, call_dyn_id, i, call_file_id);
		}
	}

	vector<map<int, int>> start_loc_set;
	vector<map<int, int>> stop_loc_set;
	for (size_t i = 0; i < num_th; i++)
	{
		map<int, int> an_start_map;
		map<int, int> an_stop_map;
		start_loc_set.push_back(an_start_map);
		stop_loc_set.push_back(an_stop_map);
	}

	if (call_loc != 0) {
#pragma omp parallel for num_threads(half_th_num)
		for (size_t i = 0; i < num_th; i++)
		{
			get_main_loop_loction_omp(instr_info_set[i], function, call_dyn_id, start_loc_set[i], stop_loc_set[i], start_line_num, end_line_num, i);
		}
	}
	else {
#pragma omp parallel for num_threads(half_th_num)
		for (size_t i = 0; i < num_th; i++)
		{
			get_main_loop_loction_omp(instr_info_set[i], function, start_loc_set[i], stop_loc_set[i], start_line_num, end_line_num, i);
		}
	}


	int before_dyn_id, stop_dyn_id, before_file_id, stop_file_id;
	for (size_t i = 0; i < num_th; i++)
	{
		if (!start_loc_set[i].empty()) {
			auto it = start_loc_set[i].begin();
			before_dyn_id = it->first;
			before_file_id = it->second;
			break;
		}
	}
	for (int i = num_th - 1; i >= 0; i--) {
		if (!stop_loc_set[i].empty()) {
			auto it = stop_loc_set[i].rbegin();
			stop_dyn_id = it->first;
			stop_file_id = it->second;
			break;
		}
	}
	vector<instr_info> before_instr_set;
	unordered_map<string, string> before_loop_map;
	for (size_t i = 0; i < num_th; i++)
	{
		if (i < before_file_id)
			before_instr_set.insert(before_instr_set.end(), instr_info_set[i].begin(), instr_info_set[i].end());
		else if (i == before_file_id) {
			int length = instr_info_set[i].size();
			for (size_t j = 0; j < length; j++)
			{
				if (instr_info_set[i][j].dynInstId < before_dyn_id)
					before_instr_set.push_back(instr_info_set[i][j]);
				else
					inside_loop_instr_set.push_back(instr_info_set[i][j]);
			}
		}
		else if (i == stop_file_id) {
			int length = instr_info_set[i].size();
			for (size_t j = 0; j < length; j++) {
				if (instr_info_set[i][j].dynInstId < stop_dyn_id)
					inside_loop_instr_set.push_back(instr_info_set[i][j]);
			}
		}
		else
			inside_loop_instr_set.insert(inside_loop_instr_set.end(), instr_info_set[i].begin(), instr_info_set[i].end());
	}
	float endTime = omp_get_wtime();
	//cout << "Specify" << "serices to get main loop execution time: " << endTime - startTime << endl;
	set<string> global_variable_set;
	stack<set<string>> local_variable;
	get_variables(before_instr_set, before_loop_map, call_loc);
	get_variables(inside_loop_instr_set, inside_loop_map, local_variable, global_variable_set, variable_loc);
	match(before_loop_map, inside_loop_map, global_variable_set);

	
	
}



void get_all_instr_omp(vector<instr_info>& instr_set, string path, int start_line_num, int stop_line_num) {
	ifstream in;
	string outfile = path;
	in.open(outfile.c_str(), ifstream::in);

	if (!in) {
		cerr << "ERROR IN OPENNING THE TRACEFILE!" << endl;
		return;
	}
	string line;
	int line_num, opencode;
	bool before_loop_flag = true;
	string current_function;
	int x = -1;
	int dynInstId;
	while (in.good())
	{
		getline(in, line);
		size_t yes_no, NumOfComma1, NumOfComma2;
		yes_no = line.find(",", 0);
		if (yes_no == string::npos)
			break;
		else {
			NumOfComma1 = 0;
			NumOfComma2 = line.find(",", NumOfComma1);
			string blockId_abnom = line.substr(NumOfComma1, NumOfComma2).c_str();
			bool flag = false;
			if (blockId_abnom == "0") {
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				line_num = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				current_function = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				opencode = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				switch (opencode)
				{
				case 1:get_instruction(line, instr_set, x); flag = true; break;
				case 2:get_instruction(line, instr_set, x); flag = true; break;
				case 8:get_instruction(line, instr_set, x); flag = true; break;
				case 9:get_instruction(line, instr_set, x); flag = true; break;
				case 10:get_instruction(line, instr_set, x); flag = true; break;
				case 11:get_instruction(line, instr_set, x); flag = true; break;
				case 12:get_instruction(line, instr_set, x); flag = true; break;
				case 13:get_instruction(line, instr_set, x); flag = true; break;
				case 14:get_instruction(line, instr_set, x); flag = true; break;
				case 15:get_instruction(line, instr_set, x); flag = true; break;
				case 16:get_instruction(line, instr_set, x); flag = true; break;
				case 26:get_instruction(line, instr_set, x); flag = true; break;
				case 27:get_instruction(line, instr_set, x); flag = true; break;
				case 28:get_instruction(line, instr_set, x); flag = true; break;
				case 29:get_instruction(line, instr_set, x); flag = true; break;
				case 44:get_instruction(line, instr_set, x); flag = true; break;
				case 46:get_instruction(line, instr_set, x); flag = true; break;
				case 49:get_instruction(line, instr_set, x); flag = true; break;
				case 104:get_instruction(line, instr_set, x); flag = true; break;
				default:
					break;
				}
				if (flag)
					break;
			}
			else
			{
				continue;
			}
		}
	}

	while (in.good()) {
		// read some info from the trace file
		getline(in, line);
		size_t yes_no, NumOfComma1, NumOfComma2;
		yes_no = line.find(",", 0);
		if (yes_no == string::npos)
			continue;
		else {
			NumOfComma1 = 0;
			NumOfComma2 = line.find(",", NumOfComma1);
			string blockId_abnom = line.substr(NumOfComma1, NumOfComma2).c_str();
			if (blockId_abnom == "0") {
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				line_num = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				current_function = line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str();
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				NumOfComma1 = NumOfComma2;
				NumOfComma2 = line.find(",", NumOfComma1 + 1);
				opencode = (atoi(line.substr(NumOfComma1 + 1, NumOfComma2 - NumOfComma1 - 1).c_str()));
			}
			switch (opencode)
			{
			case 1:get_instruction(line, instr_set, x); break;
			case 2:get_instruction(line, instr_set, x); break;
			case 8:get_instruction(line, instr_set, x); break;
			case 9:get_instruction(line, instr_set, x); break;
			case 10:get_instruction(line, instr_set, x); break;
			case 11:get_instruction(line, instr_set, x); break;
			case 12:get_instruction(line, instr_set, x); break;
			case 13:get_instruction(line, instr_set, x); break;
			case 14:get_instruction(line, instr_set, x); break;
			case 15:get_instruction(line, instr_set, x); break;
			case 16:get_instruction(line, instr_set, x); break;
			case 26:get_instruction(line, instr_set, x); break;
			case 27:get_instruction(line, instr_set, x); break;
			case 28:get_instruction(line, instr_set, x); break;
			case 29:get_instruction(line, instr_set, x); break;
			case 44:get_instruction(line, instr_set, x); break;
			case 46:get_instruction(line, instr_set, x); break;
			case 49:get_instruction(line, instr_set, x); break;
			case 104:get_instruction(line, instr_set, x); break;
			default:
				break;
			}
		}
	}
	in.close();
}


void get_function_name_omp(vector<instr_info>& instr_set, string& function, int call_loc, int& call_dyn_id, int file_id, int& call_file_id) {

	int length = instr_set.size();
	int opencode, dyn_id, line_num;
	for (size_t i = 0; i < length; i++)
	{
		if (instr_set[i].opcodeId == 49 && instr_set[i].lineId == call_loc) {
			string reg_name = instr_set[i].oprd_line_set[0].regName;
			string::size_type idx;
			idx = reg_name.find(function);
			if (idx != string::npos) {
				function = reg_name;
				call_dyn_id = instr_set[i].dynInstId;
				call_file_id = file_id;
			}
		}
	}
}


void get_main_loop_loction_omp(vector<instr_info>& instr_set, string function, map<int, int>& start_map, map<int, int>& stop_map, int start_line_num, int stop_line_num, int file_id) {
	int length = instr_set.size();
	for (size_t i = 0; i < length; i++)
	{
		if (instr_set[i].funcName == function && instr_set[i].lineId == start_line_num) {
			start_map[instr_set[i].dynInstId] = file_id;
			//cout << "BEGIN AT: " << instr_set[i].dynInstId << endl;
		}

		else if (instr_set[i].funcName == function && instr_set[i].lineId == stop_line_num) {
			//cout << "END AT: " << instr_set[i].dynInstId << endl;
			stop_map[instr_set[i].dynInstId] = file_id;
		}

	}
}


void get_main_loop_loction_omp(vector<instr_info>& instr_set, string function, int call_dyn_id, map<int, int>& start_map, map<int, int>& stop_map, int start_line_num, int stop_line_num, int file_id) {
	int length = instr_set.size();
	int i = 0;
	while (i < length)
	{
		if (instr_set[i].dynInstId < call_dyn_id)
			i++;
		else
			break;
	}

	for (i; i < length; i++)
	{
		if (instr_set[i].funcName == function && instr_set[i].lineId == start_line_num)
			start_map[instr_set[i].dynInstId] = file_id;
		else if (instr_set[i].funcName == function && instr_set[i].lineId == stop_line_num)
			stop_map[instr_set[i].dynInstId] = file_id;
	}
}