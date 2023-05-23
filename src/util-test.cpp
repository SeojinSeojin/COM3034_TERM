#include <iostream>
#include "utils.cpp"

void utilTest()
{
    wchar_t kiyeok = L'ㄱ';
    wchar_t nieun = L'ㄴ';
    wchar_t zzapNieun = L'ﾤ';
    wchar_t lowerA = L'a';
    wchar_t upperA = L'A';
    wchar_t one = L'1';

    wstring wideChars = L"~`!@#$%&*(){}[]-_=+;:'\",.?/\\";
    cout << koreanCharToUnicodeString(kiyeok) << endl;
    cout << koreanCharToUnicodeString(nieun) << endl;
    cout << koreanCharToUnicodeString(zzapNieun) << endl;
    cout << koreanCharToUnicodeString(lowerA) << endl;
    cout << koreanCharToUnicodeString(upperA) << endl;
    cout << koreanCharToUnicodeString(one) << endl;
    for (wchar_t targetChar : wideChars)
    {
        cout << koreanCharToUnicodeString(targetChar) << endl;
    }
}