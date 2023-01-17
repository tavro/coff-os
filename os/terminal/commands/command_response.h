#include "response_type.h"

class CommandResponse {
private:
    ResponseType response_type;

    File* file;
    Directory* dir;
    std::string msg;

public:
    void set_file(File* f) {
        file = f;
    }

    void set_dir(Directory* d) {
        dir = d;
    }

    void set_msg(std::string m) {
        msg = m;
    }

    void set_response_type(ResponseType rt) {
        response_type = rt;
    }

    CommandResponse() {

    }
    
    CommandResponse(File* f) {
        set_file(f);
        set_response_type(ResponseType::FILE_RESPONSE);
    }

    CommandResponse(Directory* d) {
        set_dir(d);
        set_response_type(ResponseType::DIR_RESPONSE);
    }

    CommandResponse(std::string m, bool err) {
        set_msg(m);

        if(!err)
            set_response_type(ResponseType::MSG_RESPONSE);
        else
            set_response_type(ResponseType::ERR_RESPONSE);
    }

    ResponseType get_response_type() {
        return response_type;
    }

    File* get_file() {
        return file;
    }

    Directory* get_dir() {
        return dir;
    }

    std::string get_msg() {
        return msg;
    }
};