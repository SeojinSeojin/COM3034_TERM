#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <cctype>

using namespace std;

bool isNumber(string utf8Value)
{
    return utf8Value >= "0030" && utf8Value <= "0039";
}

bool isAlphaBet(string utf8Value)
{
    return (utf8Value >= "0041" && utf8Value <= "005A") || (utf8Value >= "0061" && utf8Value <= "007A");
};

bool isKoreanJaeum(string utf8Value)
{
    return (utf8Value >= "3131" && utf8Value <= "3142") || (utf8Value >= "FFA1" && utf8Value <= "FFBE");
}

bool isKoreanMoeum(string utf8value)
{
    return (utf8value >= "3132" && utf8value <= "3163") || (utf8value >= "FFC2" && utf8value <= "FFDC");
}

char decode(const string &unicodeString)
{
    int unicodeInt = stoi(unicodeString, nullptr, 16);
    return static_cast<char>(unicodeInt);
}

string subtractFromHex(const string &hexString, const string &hexValue)
{
    istringstream iss(hexString);
    int intValue;
    iss >> hex >> intValue;

    istringstream issValue(hexValue);
    int intValueValue;
    issValue >> hex >> intValueValue;

    int result = intValue - intValueValue;

    ostringstream oss;
    oss << hex << result;
    return oss.str();
}

string koreanCharToUnicodeString(wchar_t koreanChar) // '가' -> 'AC00'
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

    if (isAlphaBet(utf8Value) || isNumber(utf8Value))
    {
        return string(1, decode(utf8Value));
    }
    if (isKoreanJaeum(utf8Value))
    {
        if (utf8Value <= "3142")
            return utf8Value;
        return subtractFromHex(utf8Value, subtractFromHex("FFA1", "3131"));
    }
    if (isKoreanMoeum(utf8Value))
    {
        if (utf8Value <= "3163")
            return utf8Value;
        return subtractFromHex(utf8Value, subtractFromHex("FFC2", "3132"));
    }

    return utf8Value;
}