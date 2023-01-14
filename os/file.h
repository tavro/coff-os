#include <string>

class File {
public:
    std::string name;
    std::string type;
    std::string location;

    int size;
    
    // TODO
    // Implement pointer to data stored on hard drive
    // For now, a Data structure will be used

    File(std::string file_name, std::string file_type) {
        name = file_name;
        type = file_type;
        size = 0;
        location = "/";
    }
private:
};

// TODO
// Implement: Create, Delete, Read, Write