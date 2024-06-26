#pragma once
#include<vector>
#include <iostream>
#include<unordered_map>
using namespace std;
struct dependency {
	int line;
	vector<int> dependency_info_set;
};

struct cpk_variable {
	unordered_map<string, string> update_map;
	unordered_map<string, string> input_map;
};

struct used_info {
	int init_state = 0;
	int factor_state = 0;
	int times = 0;
	int line = 0;
};

class dependecy_info
{
public:
	vector<dependency> dependency_set;
	unordered_map<string, used_info> variable_used_info;
	//key : variable name
	//value : 1 first used init
	//value : 0 other situation
	dependecy_info();
	~dependecy_info();
};

