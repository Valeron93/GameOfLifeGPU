#include "util.h"
#include <SDL3/SDL.h>

namespace util {

auto get_resources_path()
{
#ifdef __APPLE__
    auto path = SDL_GetBasePath();
#else
    auto path = SDL_GetCurrentDirectory();
#endif
    return path;
}

std::string read_all_text(const char* filename)
{
    std::filesystem::path base = get_resources_path();
    std::filesystem::path path = filename;

    auto filepath = base / path;
    std::stringstream ss;
    std::ifstream file(filepath);
    ss << file.rdbuf();
    return ss.str();
}

}