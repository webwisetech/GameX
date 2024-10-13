#include <iostream>
#include "luastd.hpp"
#include <fstream>

LuaSTD lua;

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