#include <utility>

#include "../include/csv.h"

using namespace std;
using json = nlohmann::json;
using mss = std::map<string, long long unsigned int>;
using fpath = std::filesystem::path;

/*
    Конструктор для создания класса Csv
    Параметры: 
        _path - путь к файлу
*/
Csv::Csv(fpath _path) : path(std::move(_path)), j() {to_json();}

/*
    Установить путь хранения файла
    Параметры: 
        _path - путь к файлу
*/
void Csv::set_path(fpath _path) { 
    this->path = std::move(_path);
    to_json();
}

/*
    Установить значение для JSON
    Параметры: 
        _j - JSON-объект
*/
void Csv::set_j(json _j) { this->j = std::move(_j); }

/*
    Установить значение для сохранения результата последнего запроса
    Параметры:
        result - результат запроса
*/

/*
    Возвращает путь в котором хранится открытый файл
*/
fpath Csv::get_path() const { return this->path; }

/*
    Возвращает JSON-объект таблицы
*/
json Csv::get_j() const { return this->j; }


/*
    Конвертация Csv-файла в JSON-объект
*/
void Csv::to_json() {
	fpath _path = get_path();
	std::ifstream file(_path);

    if (!file.is_open()) {
        std::cerr << "Failed to open filepath: " << path << std::endl;
        return;
    }
    
	vector<string> columnsNames;
    vector<vector<string>> values;
	std::string line;
	getline(file, line); 

	std::stringstream ss(line);
	std::string column_name;
	while (getline(ss, column_name, ',')) {
	    columnsNames.push_back(column_name);
	}

	std::vector<std::string> row;
	while (getline(file, line)) {
	    std::stringstream _stringstream(line);
	    std::string cell_value;
	    while (getline(_stringstream, cell_value, ',')) {
	        row.push_back(cell_value);
	    }
	    values.push_back(row);
	    row.clear(); 
	}
	

    mss columns;
    for (long long unsigned int i = 0; i < columnsNames.size(); i++) {
        columns[columnsNames[i]] = i;
    }
    json _j{ { "columns", columns }, { "values", values }};
    set_j(_j);
}

/*
    Сохранение изменений в таблице
*/
void Csv::commit() {
    std::ifstream _file(path);
    if (!_file.is_open()) {
        std::cerr << "Failed to open filepath: " << path << std::endl;
        return;
    }

    try {
        if (!j.contains("columns") ||!j.contains("values")) {
            std::cerr << "JSON does not contain required fields." << std::endl;
            return;
        }
        string fir; 
        getline(_file, fir);
        _file.close();

        std::ofstream file(path, std::ios::out);
        file << fir;
        file << '\n';
        for (const auto& row : j.at("values")) {
            for (const auto& value : row) {
                std::string cleaned_value = std::regex_replace(value.get<std::string>(), std::regex("\\\\|\"", std::regex_constants::icase), "");
                file << cleaned_value;
                if (&value != &row.back()) {
                    file << ',';
                }
            }
            file << '\n';
        }
        file.close();

    } catch (const std::exception& e) {
        std::cerr << "Error writing to file: " << e.what() << std::endl;
    }
}

/*
    Запрос на получение/создание/удаление/изменение данных в (из) таблице(-ы)
    Параметры:
        query - запрос на языке SQL
*/
void Csv::execute(const string& query) {
    vector<vector<string>> container = tools::split_query(query);
    
    if (container.empty()) {
        std::cerr << "The request is invalid" << '\n';
    }
   
    string operation = container[0][0];
    unsigned long long size = container.size();
    vector<string> object = container[0];
    json _j = get_j();

    
    switch (size) {
    	case 1: {
            if (operation == "SELECT") { _select(object, _j); }
            if (operation == "DELETE") { _delete(object, _j); }
            if (operation == "INSERT") { _insert(object, _j); }
            break;
        }
    	case 2: {
            vector<string> conditions = container[1];
            if (operation == "SELECT") { _select(object, conditions, _j); }
            if (operation == "DELETE") { _delete(object, conditions, _j); }
            if (operation == "UPDATE") { _update(object, conditions, _j); }
            break;
        }
    	case 3: {
            vector<string> set = container[1];
            vector<string> conditions = container[2];
            _update(object, set, conditions, _j);
            break;
        }
        default: {
            return;
        }
    }
    
    
}



/*
    Получение данных из таблицы
    Параметры:
        object - колонки
        _j - JSON-объект таблицы
        (optional) conditions - условия при которых нужно выгружать данные
*/
void Csv::_select(vector<string> &object, json &_j) {
    auto op = object.cbegin();
    object.erase(op);
    try {
        for (const string& obj: object) {
            cout << left << setw(15) << tools::centered(obj, 15);
        }
        cout << "\n";
        for (vector<string> element : _j["values"]) {
            vector<string> record;
            for (const string& obj : object) {
                int key = _j["columns"].at(obj);
                record.push_back(element[key]);
            }
            for (const string& elem : record) {
                cout << left << setw(15) << tools::centered(elem, 15);
                if (elem == record.back()) {
                    cout << "\n";
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}
void Csv::_select(vector<string> &object, vector<string> &conditions, json &_j) {
    auto op = object.cbegin();
    object.erase(op);
    try {
        for (const string& obj: object) {
            cout << left << setw(15) << tools::centered(obj, 15);
        }
        cout << "\n";
        for (vector<string> el : _j["values"]) {
            vector<string> record;
            string columnName = conditions[1];
            string comparisonOperator = conditions[2];
            string value = conditions[3];

            if (tools::checkCondition(el[_j["columns"].at(columnName)], comparisonOperator, value)) {
                for (const string& obj : object) {
                    int key = _j["columns"].at(obj);
                    record.push_back(el[key]);
                }
            }
            for (const string& element : record) {
                cout << left << setw(15) << tools::centered(element, 15);
                if (element == record.back()) {
                    cout << "\n";
                }
            }
        }
    
      
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}
    
/*
    Удаление записей из таблицы
    Параметры:
        object - колонки
        _j - JSON-объект таблицы
        (optional) conditions - условия при которых нужно удалять записи
*/
void Csv::_delete(vector<string> &object, json &_j) {
    try {
        _j["values"].clear();
        set_j(_j);
        cout << "Deleted\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}
void Csv::_delete(vector<string> &object, vector<string> &conditions, json &_j) {
    vector<vector<string>> selectResult;
    try {
        for (long long unsigned int i = 0; i < _j["values"].size(); ++i) {
            string columnName = conditions[1];
            string comparisonOperator = conditions[2];
            string value = conditions[3];
            vector<string> el = _j["values"][i];
            if (tools::checkCondition(el[_j["columns"].at(columnName)], comparisonOperator, value)) {
                _j["values"].erase(i);}
        }
        set_j(_j);
        cout << "Deleted\n";
    } catch (const std::exception& e) {
       std::cerr << e.what() << endl;
    }
}

/*
    Изменение данных в таблице
    Параметры:
        object - колонки
        set - данные для изменения
        _j - JSON-объект таблицы
        (optional) conditions - условия при которых нужно менять данные
*/
void Csv::_update(vector<string> &object, vector<string> &set, json &_j) {
    vector<vector<string>> selectResult;
    auto op = set.cbegin();
    set.erase(op);
    try {
        for (long long unsigned int i = 0; i < _j["values"].size(); ++i) {
            for (unsigned long long k = 0; k < set.size(); k+=3) {
                int key = _j["columns"].at(set[k]);
                string value = set[k+2];
                _j["values"][i][key] = value;
            }
        }
        set_j(_j);
        cout << "Updated\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}
void Csv::_update(vector<string> &object, vector<string> &set, vector<string> &conditions, json &_j) {
    vector<vector<string>> selectResult;
    auto op = set.cbegin();
    set.erase(op);
    try {
        string columnName = conditions[1];
        string comparisonOperator = conditions[2];
        string value = conditions[3];
        for (long long unsigned int i = 0; i < _j["values"].size(); ++i) {
            vector<string> el = _j["values"][i];
            if (tools::checkCondition(el[_j["columns"].at(columnName)], comparisonOperator, value)) {
                for (long long unsigned int k = 0; k < set.size(); k+=3) {
                    int key = _j["columns"].at(set[k]);
                    cout << key << endl;
                    string val = set[k+2];
                    _j["values"][i][key] = val;
                }
            }
        }
        set_j(_j);
        cout << "Updated\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
    }
}

/*
    Создание записи в таблице
    Параметры:
        object - данные
        _j - JSON-объект таблицы
*/
void Csv::_insert(vector<string> &object, json &_j) {
    auto op = object.cbegin();
    object.erase(op);
    try {
        if (object.size() != _j["columns"].size()) {
            std::cerr << "Кол - во данных не соответствует кол - во колонок\n";
            exit(1);
        }
        _j["values"].push_back(object);
        set_j(_j);
        cout << "Inserted\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << endl;
        exit(1);
    }
}