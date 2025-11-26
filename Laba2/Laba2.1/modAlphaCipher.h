#pragma once
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
using namespace std;

class modAlphaCipher
{
private:
    wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    map<wchar_t, int> alphaNum;
    vector<int> key;
    vector<int> convert(const wstring& s);
    wstring convert(const vector<int>& v);
    wstring getValidKey(const wstring& s);
    wstring getValidOpenText(const wstring& s);
    wstring getValidCipherText(const wstring& s);
    wstring to_upper(const wstring& s);

public:
    modAlphaCipher() = delete;
    modAlphaCipher(const wstring& skey);
    wstring encrypt(const wstring& open_text);
    wstring decrypt(const wstring& cipher_text);
};

class cipher_error : public invalid_argument
{
public:
    explicit cipher_error(const string& what_arg)
        : invalid_argument(what_arg)
    {
    }
    explicit cipher_error(const char* what_arg)
        : invalid_argument(what_arg)
    {
    }
};