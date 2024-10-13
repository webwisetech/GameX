#pragma once

#include <lua.hpp>
#include <raylib.h>
#include <string>
#include <vector>
#define LuaPLFile int

class LuaSTD {
    public:
        std::string readFile(const std::string& filename);
        std::vector<std::string> preloaded{};
        lua_State* L;
        void initialize();
        template<typename T>
        T getVar(const std::string &varname);
        int preload(const std::string& filename);
        void run(int preloadedID);
};