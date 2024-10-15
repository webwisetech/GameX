#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include "util.hpp"

void Util::replaceString(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::string Util::escapeString(const std::string& input) {
    std::string escaped;
    for (char c : input) {
        if (c == '"') {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}

template <typename T>
T Util::isSTD(T i) {
    auto p = std::make_unique<T>(std::move(i)); std::stringstream s; 
    if constexpr (std::is_same_v<T, std::string>) s << "y" << std::boolalpha << ((std::string*)p.get())->empty(); 
    else s << "n" << *p; 
    return [&]{std::string t; return (s >> t, t != "n");}();
}

std::string Util::readFile(const std::string& filename){
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