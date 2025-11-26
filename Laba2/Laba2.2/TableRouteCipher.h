#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class cipher_error : public invalid_argument {
public:
    explicit cipher_error(const string& what_arg)
        : invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg)
        : invalid_argument(what_arg) {}
};

class Cipher {
private:
    int key;
    inline int getValidKey(int key, const string& Text);
    inline string getValidOpenText(string& s);
    inline string getValidCipherText(string& s, string& open_text);
    bool isValidUTF8Char(const string& str, size_t& pos);

public:
    Cipher() = delete;
    Cipher(int skey, string text);
    string encryption(string& text);
    string transcript(string& text, string& open_text);
};