#include <iostream>
#include "luastd.hpp"
#include <fstream>

LuaSTD lua;

void replaceString(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
std::string escapeString(const std::string& input) {
    std::string escaped;
    for (char c : input) {
        // Escape double quotes
        if (c == '"') {
            escaped += '\\'; // Add the escape character
        }
        escaped += c; // Add the original character
    }
    return escaped;
}

int main(){
    lua.initialize();
    SetTraceLogLevel(LOG_ERROR);
    LuaPLFile config = lua.preload("config.lua");
    lua.run(config);
    
    InitWindow(800, 600, TextFormat((lua.getVar<std::string>("appname") + " - Version %i").c_str(), lua.getVar<int>("appversion")));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(lua.getVar<int>("fps"));

    LuaPLFile drawcode = lua.preload("app/draw.lua");
    LuaPLFile controlscode = lua.preload("app/controls.lua");

    while(!WindowShouldClose()){
        lua.run(controlscode); 
        if(IsKeyPressed(KEY_R)){ 
            lua.L = luaL_newstate();
            drawcode = lua.preload("app/draw.lua");
            config = lua.preload("config.lua");
            controlscode = lua.preload("app/controls.lua");

            lua.initialize();
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            lua.run(drawcode);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}