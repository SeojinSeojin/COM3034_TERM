#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <cctype>

using namespace std;

string koreanCharToUnicodeString(wchar_t koreanChar)
{
    wstring unicodeValue;
    wstringstream stream;

    stream << hex << setw(4) << setfill(L'0') << static_cast<int>(koreanChar);
    unicodeValue = stream.str();

    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string utf8Value = converter.to_bytes(unicodeValue);
    for (char &c : utf8Value)
    {
        c = toupper(c);
    }

    return utf8Value;
}