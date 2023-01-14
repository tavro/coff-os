#include <string>

class File {
public:
    std::string name;
    std::string type;
    std::string location;

    int size;

    bool is_open;
    
    // TODO
    // Implement pointer to data stored on hard drive
    // For now, a Data structure will be used
    std::vector<std::string> content;

    File(std::string file_name, std::string file_type) {
        name = file_name;
        type = file_type;
        size = 0;
        is_open = false;
        location = "/";
    }
private:
};

// TODO
// Implement: Delete, Read, Write