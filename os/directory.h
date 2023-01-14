#include <string>
#include <vector>

class Directory {
public:
    std::string name;
    std::string location;

    std::vector<File*> files;

    Directory(std::string dir_name, std::string dir_location) {
        name = dir_name;
        location = dir_location;
    }
private:
};

// TODO
// Implement: Delete, Read