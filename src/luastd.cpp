#include "luastd.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Font> fonts{};
Font defaultFont;

struct LuaRectangle {
    float x, y, width, height;
    
    LuaRectangle(float x, float y, float w, float h)
        : x(x), y(y), width(w), height(h) {}
};

struct LuaCircle {
    float x, y, radius;

    LuaCircle(float x, float y, float r)
        : x(x), y(y), radius(r) {}
};

struct LuaTriangle {
    float x1, y1, x2, y2, x3, y3;

    LuaTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
        : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}
};

int showFPS(lua_State *L){
    DrawFPS(20, 20);
    return 0;
}

int lua_newRectangle(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float w = luaL_checknumber(L, 3);
    float h = luaL_checknumber(L, 4);

    LuaRectangle** udata = (LuaRectangle**) lua_newuserdata(L, sizeof(LuaRectangle*));
    *udata = new LuaRectangle(x, y, w, h); // Create a new Rectangle instance

    // Set the metatable for the Rectangle
    luaL_getmetatable(L, "RectangleMetaTable");
    lua_setmetatable(L, -2);

    return 1; // Return the userdata (Rectangle) to Lua
}

int drawRectangle(lua_State* L) {
    // Retrieve the Rectangle from the Lua stack
    LuaRectangle** udata = (LuaRectangle**) luaL_checkudata(L, 1, "RectangleMetaTable");
    LuaRectangle* rect = *udata;

    // Now draw the rectangle using your drawing function (e.g., using Raylib)
    DrawRectangle(rect->x, rect->y, rect->width, rect->height, RED); // Example using Raylib

    return 0; // No values returned to Lua
}

void registerRectangle(lua_State* L) {
    luaL_newmetatable(L, "RectangleMetaTable");

    lua_newtable(L);
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);

    lua_pushcfunction(L, lua_newRectangle);
    lua_setglobal(L, "Rectangle");
}

int lua_newCircle(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float r = luaL_checknumber(L, 3);

    LuaCircle** udata = (LuaCircle**) lua_newuserdata(L, sizeof(LuaCircle*));
    *udata = new LuaCircle(x, y, r);

    luaL_getmetatable(L, "CircleMetaTable");
    lua_setmetatable(L, -2);

    return 1;
}

int drawCircle(lua_State* L) {
    LuaCircle** udata = (LuaCircle**) luaL_checkudata(L, 1, "CircleMetaTable");
    LuaCircle* circle = *udata;

    // Draw circle using your drawing library
    DrawCircle(circle->x, circle->y, circle->radius, BLUE); // Example using Raylib

    return 0;
}

void registerCircle(lua_State* L) {
    luaL_newmetatable(L, "CircleMetaTable");

    lua_newtable(L);
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);

    lua_pushcfunction(L, lua_newCircle);
    lua_setglobal(L, "Circle");

    lua_pushcfunction(L, drawCircle);
    lua_setglobal(L, "drawCircle");
}

int lua_newTriangle(lua_State* L) {
    float x1 = luaL_checknumber(L, 1);
    float y1 = luaL_checknumber(L, 2);
    float x2 = luaL_checknumber(L, 3);
    float y2 = luaL_checknumber(L, 4);
    float x3 = luaL_checknumber(L, 5);
    float y3 = luaL_checknumber(L, 6);

    LuaTriangle** udata = (LuaTriangle**) lua_newuserdata(L, sizeof(LuaTriangle*));
    *udata = new LuaTriangle(x1, y1, x2, y2, x3, y3);

    luaL_getmetatable(L, "TriangleMetaTable");
    lua_setmetatable(L, -2);

    return 1;
}

int drawTriangle(lua_State* L) {
    LuaTriangle** udata = (LuaTriangle**) luaL_checkudata(L, 1, "TriangleMetaTable");
    LuaTriangle* circle = *udata;
    std::cout << "curazi: " << circle->x1 << ", " << circle->y1 << "; " << circle->x2 << ", " << circle->y2 << "; " << circle->x3 << ", " << circle->y3 << "\n";
    DrawTriangle({ circle->x1, circle->y1 }, { circle->x2, circle->y2 }, { circle->x3, circle->y3 }, RED);

    return 0;
}

void registerTriangle(lua_State* L) {
    luaL_newmetatable(L, "TriangleMetaTable");

    lua_newtable(L);
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);

    lua_pushcfunction(L, lua_newTriangle);
    lua_setglobal(L, "Triangle");

    lua_pushcfunction(L, drawTriangle);
    lua_setglobal(L, "drawTriangle");
}

void registerDrawFunctions(lua_State* L) {
    lua_pushcfunction(L, drawRectangle);
    lua_setglobal(L, "drawRectangle"); 
    lua_pushcfunction(L, drawCircle);
    lua_setglobal(L, "drawCircle"); 
    lua_pushcfunction(L, drawTriangle);
    lua_setglobal(L, "drawTriangle"); 
}



int drawText(lua_State *L){
    int argc = lua_gettop(L);

    if (argc != 3) {
        lua_pushstring(L, "Expected 3 arguments");
        lua_error(L);
    }

    if (!lua_isstring(L, 1) || !lua_isinteger(L, 2) || !lua_isinteger(L, 3)) {
        lua_pushstring(L, "Usage: drawText(text, x, y)");
        lua_error(L);
    }

    int pos = lua_tointeger(L, 2);
    int pos1 = lua_tointeger(L, 3);

    std::string text = lua_tostring(L, 1);

    DrawText(text.c_str(), pos, pos1, 40, WHITE);
    
    return 0;
}

int keyDown(lua_State *L){
    int argc = lua_gettop(L);

    if(argc != 1){
        lua_pushstring(L, "Expected 1 argument");
        lua_error(L);
    }

    if(!lua_isinteger(L, 1)){
        lua_pushstring(L, "Usage: keyDown(key)");
        lua_error(L);
    }

    int key = lua_tointeger(L, 1);

    bool down = IsKeyDown(key);

    lua_pushboolean(L, down);

    return 1;
}

int loadFont(lua_State *L){
    int argc = lua_gettop(L);

    if(argc != 1){
        lua_pushstring(L, "Expected 1 argument");
        lua_error(L);
    }

    if(!lua_isstring(L, 1)){
        lua_pushstring(L, "Usage: loadFont(fontpath)");
        lua_error(L);
    }

    std::string font = lua_tostring(L, 1);

    Font foont = LoadFont(font.c_str());
    fonts.push_back(std::move(foont));

    lua_pushinteger(L, fonts.size() - 1);

    return 1;
}

int useFont(lua_State *L){
    int argc = lua_gettop(L);

    if(argc != 1){
        lua_pushstring(L, "Expected 1 argument");
        lua_error(L);
    }

    if(!lua_isinteger(L, 1)){
        lua_pushstring(L, "Usage: useFont(font)");
        lua_error(L);
    }

    int fontID = lua_tointeger(L, 1);

    Font font = fonts[fontID];
    defaultFont = std::move(font);

    return 1;
}

void LuaSTD::initialize(){
    L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "drawText", drawText);
    lua_register(L, "showFPS", showFPS);
    lua_register(L, "keyDown", keyDown);
    registerRectangle(L);
    registerCircle(L);
    registerTriangle(L);
    registerDrawFunctions(L);
    defaultFont = GetFontDefault();
}

template<>
bool LuaSTD::getVar(const std::string &varname) {
    lua_getglobal(L, varname.c_str());
    if (lua_isboolean(L, -1)) {
        bool value = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return value;
    }
    lua_pop(L, 1);
    throw std::runtime_error("Type error: expected boolean");
}

template<>
double LuaSTD::getVar(const std::string &varname) {
    lua_getglobal(L, varname.c_str());
    if (lua_isnumber(L, -1)) {
        double value = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return value;
    }
    lua_pop(L, 1);
    throw std::runtime_error("Type error: expected number");
}

template<>
std::string LuaSTD::getVar(const std::string &varname) {
    lua_getglobal(L, varname.c_str());
    if (lua_isstring(L, -1)) {
        std::string value = lua_tostring(L, -1);
        lua_pop(L, 1);
        return value;
    }
    lua_pop(L, 1);
    throw std::runtime_error("Type error: expected string");
}

template<>
int LuaSTD::getVar(const std::string &varname) {
    lua_getglobal(L, varname.c_str());
    if (lua_isnumber(L, -1)) {
        int value = static_cast<int>(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return value;
    }
    lua_pop(L, 1);
    throw std::runtime_error("Type error: expected number (int)");
}

template<>
void* LuaSTD::getVar( const std::string &varname) {
    lua_getglobal(L, varname.c_str());
    if (lua_isuserdata(L, -1)) {
        void* value = lua_touserdata(L, -1);
        lua_pop(L, 1);
        return value;
    }
    lua_pop(L, 1);
    throw std::runtime_error("Type error: expected userdata");
}
std::string LuaSTD::readFile(const std::string& filename){
    std::fstream input(filename, std::ios::in);
    
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << input.rdbuf();
    std::string str = ss.str();
    
    return str;
}

int LuaSTD::preload(const std::string& filename) {
    std::string str = readFile(filename);

    preloaded.push_back(std::move(str));

    return preloaded.size() - 1;
}


void LuaSTD::run(int preloadedID){
    std::string preloadedFile = preloaded[preloadedID];
    int result = luaL_dostring(L, preloadedFile.c_str());
    
    // Check for errors
    if (result != LUA_OK) {
        // Retrieve the error message from the top of the stack
        const char* errorMessage = lua_tostring(L, -1);
        // Log or print the error message
        std::cerr << "Lua Error: " << errorMessage << std::endl;
        // Pop the error message from the stack
        lua_pop(L, 1);
    }

    return;
}