#include <string>

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
        if (c == '"') {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}