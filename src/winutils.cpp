#include <windows.h>
#include <string>
#include "util.hpp"

int Util::DisplayErrorMessageBox(LPCSTR error) {
    int msgboxID = MessageBoxA(
        NULL,
        error,
        "Error",
        MB_ICONEXCLAMATION | MB_OK
    );

    if (msgboxID == IDOK)
    {
        exit(1);
    }

    return msgboxID;    
}

wchar_t * Util::convertCharArrayToLPCWSTR(const char* charArray){
    wchar_t* wString=new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}