#include <string>

class Util {
    public:
        void replaceString(std::string& str, const std::string& from, const std::string& to);
        std::string escapeString(const std::string& input);
        template <typename T>
        T isSTD(T i);
        std::string readFile(const std::string& filename);
        int DisplayErrorMessageBox(const char* error);
        wchar_t *convertCharArrayToLPCWSTR(const char* charArray);
};