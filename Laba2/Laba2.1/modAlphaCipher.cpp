#include "modAlphaCipher.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Убираем глобальные переменные с локалью
wstring_convert<codecvt_utf8<wchar_t>, wchar_t> codec;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    for(unsigned i = 0; i < numAlpha.size(); i++)
        alphaNum[numAlpha[i]] = i;
    key = convert(getValidKey(skey));
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    vector<int> work = convert(getValidOpenText(open_text));
    for(unsigned i = 0; i < work.size(); i++)
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    vector<int> work = convert(getValidCipherText(cipher_text));
    for(unsigned i = 0; i < work.size(); i++)
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    return convert(work);
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for(auto c : s)
        result.push_back(alphaNum[c]);
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for(auto i : v)
        result.push_back(numAlpha[i]);
    return result;
}

wstring modAlphaCipher::to_upper(const wstring& s)
{
    wstring result;
    for(auto c : s) {
        if(c >= L'а' && c <= L'я')
            result.push_back(c - 32);
        else if(c == L'ё')
            result.push_back(L'Ё');
        else
            result.push_back(c);
    }
    return result;
}

wstring modAlphaCipher::getValidKey(const wstring& s)
{
    if(s.empty())
        throw cipher_error("Пустой ключ");
    
    wstring tmp = to_upper(s);
    for(auto c : tmp) {
        if(c < L'А' || (c > L'Я' && c != L'Ё'))
            throw cipher_error("Неправильный ключ: содержит недопустимые символы");
    }
    return tmp;
}

wstring modAlphaCipher::getValidOpenText(const wstring& s)
{
    wstring tmp;
    for(auto c : s) {
        if((c >= L'А' && c <= L'Я') || c == L'Ё' || 
           (c >= L'а' && c <= L'я') || c == L'ё') {
            if(c >= L'а' && c <= L'я')
                tmp.push_back(c - 32);
            else if(c == L'ё')
                tmp.push_back(L'Ё');
            else
                tmp.push_back(c);
        }
    }
    if(tmp.empty())
        throw cipher_error("Отсутствует текст");
    return tmp;
}

wstring modAlphaCipher::getValidCipherText(const wstring& s)
{
    if(s.empty())
        throw cipher_error("Пустой текст");
    
    wstring tmp = to_upper(s);
    for(auto c : tmp) {
        if((c < L'А' || c > L'Я') && c != L'Ё')
            throw cipher_error("Неправильно зашифрованный текст: содержит недопустимые символы");
    }
    return tmp;
}