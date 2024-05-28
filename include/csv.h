#pragma once



#ifndef CORE_H
#define CORE_H

#include "../libs/nlohmann/json.hpp"
#include "tools.h"
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;
using fpath = std::filesystem::path;

class Csv {
private:
    fpath path;
    json j;
protected:
    fpath get_path() const;
    json get_j() const;

	void set_path(fpath _path);
	void set_j(json _j);

    void to_json();

    static void _select(vector<string> &, json &);
    static void _select(vector<string> &, vector<string> &, json &);
    
    void _delete(vector<string> &, json &);
    void _delete(vector<string> &, vector<string> &, json &);

    void _update(vector<string> &, vector<string> &, json &);
    void _update(vector<string> &, vector<string> &, vector<string> &, json &);

    void _insert(vector<string> &, json &);
    
public:
    Csv(fpath path);
    void execute(const string& query);
    void commit();
};


#endif