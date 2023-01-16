#include <string>

void remove_last_substring(std::string& str) {
    str.erase(str.size()-1, 1);
    int start_index = str.find_last_of('/')+1;
    int last_index = str.size()-1;
    str.erase(start_index, last_index);
}