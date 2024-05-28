#pragma once

#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <regex>
#include <stdexcept>

using namespace std;

namespace tools {
    bool is_valid_query(const string& query);
    vector<vector<string>> split_query(const string& line);
    bool checkCondition(const std::string& mean, const std::string& comparisonOperator, const std::string& value);
    string centered(string text, int width);
} 

#endif 
