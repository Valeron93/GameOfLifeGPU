#include "util.h"

namespace util {

std::string read_all_text(const char* filename)
{
    std::stringstream ss;
    std::ifstream file(filename);
    ss << file.rdbuf();
    return ss.str();
}

}