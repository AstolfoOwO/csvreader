#include "../include/tools.h"
#include <cstdlib>

using namespace std;

namespace tools {

    /*
        Разбиение запроса
        Параметры:
            query - запрос
    */
    vector<vector<string>> split_query(const string& query){

        std::vector<char> chars(query.begin(), query.end());
        chars.erase(std::remove_if(chars.begin(), chars.end(), [](char c){ return c == ','; }), chars.end());
        std::string line(chars.begin(), chars.end());
        
        vector<vector<string>> results;
    
        stringstream ss(line);
        string word;
        vector<string> currentWords;

        string tempWord = "";

        while (ss >> word) {
            if (word == "INTSERT" || word == "SELECT" || word == "DELETE" || word == "UPDATE" || word == "SET" || word == "WHERE") {
                if (!currentWords.empty()) {
                    results.push_back(currentWords);
                    currentWords.clear();
                }
                currentWords.push_back(word);
            } else {
                if (word.front() == '\'' && word.back() == '\'') {
                    auto st = word.cbegin();
                    word.erase(st);
                    word.erase(st + word.length() - 1);
                    currentWords.push_back(word);
                    continue;
                }
                if (word.front() == '\'') {
                    tempWord += word.substr(1);
                } else if (tempWord!= "" && word.back() == '\'') {
                    tempWord += " " + word.substr(0, word.length()-1); 
                    currentWords.push_back(tempWord);
                    tempWord = "";
                } else if (tempWord!= "") {
                    tempWord += " " + word;
                } else {
                    currentWords.push_back(word);
                }
            }
        }

        if (!currentWords.empty()) {
            results.push_back(currentWords);
        }

        return results;
    }
    
    /*
        Безопасное преобразование типов
    */
    auto safeStoLL(const std::string& str) -> std::pair<int, std::string> {
        try {
            return {std::stoi(str), ""}; 
        } catch (const std::invalid_argument&) {
            return {0, str};
        } catch (const std::out_of_range&) {
            return {0, str};
        }
    }   

    /*
        Проверка условий запроса
        Параметры:
            mean - значение которое мы сравниеваем с value
            comparisonOperator - оператор сравнение
            value - значение с которым мы сравниваем mean
    */
    bool checkCondition(const std::string& mean, const std::string& comparisonOperator, const std::string& value) {
        auto [mean_one, mean_two] = safeStoLL(mean);
        auto [value_one, value_two] = safeStoLL(value);

        int meanValue = mean_one;
        std::string meanString = mean_two;
        int valueValue = value_one;
        std::string valueString = value_two;
        if (mean_one!= 0 && value_one!= 0) {
            if (comparisonOperator == ">") {
                return meanValue > valueValue;
            } else if (comparisonOperator == "<") {
                return meanValue < valueValue;
            } else if (comparisonOperator == ">=") {
                return meanValue >= valueValue;
            } else if (comparisonOperator == "<=") {
                return meanValue <= valueValue;
            } else if (comparisonOperator == "!=") {
                return meanValue!= valueValue;
            } else if (comparisonOperator == "=") {
                return meanValue == valueValue;
            }
        } else {
            if (comparisonOperator == ">") {
                return meanString > valueString;
            } else if (comparisonOperator == "<") {
                return meanString < valueString;
            } else if (comparisonOperator == ">=") {
                return meanString >= valueString;
            } else if (comparisonOperator == "<=") {
                return meanString <= valueString;
            } else if (comparisonOperator == "!=") {
                return meanString!= valueString;
            } else if (comparisonOperator == "=") {
                return meanString == valueString;
            }
        }

        return false;
    }

    string centered(string text, int width) {
        int padding = (width - text.length()) / 2;
        std::string fill(padding, ' ');
        return fill + text + fill;
    }
}