#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include <cwctype>
#include <iostream>

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::wstring& what_arg);
    explicit cipher_error(const char* what_arg);
};

class modAlphaCipher
{
private:
    int key;
    int getValidKey(const int k);
    std::wstring toValidText(const std::wstring& s);
    std::vector<std::vector<wchar_t>> createTable(const std::wstring& text);
    std::wstring readTableVertical(const std::vector<std::vector<wchar_t>>& table);
    std::wstring readTableHorizontal(const std::vector<std::vector<wchar_t>>& table);
    
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const int k);
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};