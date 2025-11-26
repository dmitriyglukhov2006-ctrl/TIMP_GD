#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;

wstring string_to_wstring(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstring_to_string(const wstring& wstr) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void check(const string& Text, const string& key, const bool destructCipherText = false)
{
    try {
        wstring cipherText;
        wstring decryptedText;
        wstring wText = string_to_wstring(Text);
        wstring wKey = string_to_wstring(key);
        
        if (wText.empty() || wKey.empty()) {
            throw cipher_error("Текст или ключ не могут быть пустыми.");
        }

        modAlphaCipher cipher(wKey);
        cipherText = cipher.encrypt(wText);
        
        if (destructCipherText && cipherText.length() > 1) {
            cipherText[1] = L'Я'; // Изменяем символ для теста
        }

        decryptedText = cipher.decrypt(cipherText);
        
        cout << "Ключ = " << key << endl;
        cout << "Исходный текст: " << Text << endl;
        cout << "Зашифрованный текст: " << wstring_to_string(cipherText) << endl;
        cout << "Расшифрованный текст: " << wstring_to_string(decryptedText) << endl;
        cout << "---" << endl;
    } catch (const cipher_error& e) {
        cerr << "Ошибка шифрования: " << e.what() << endl;
        cout << "---" << endl;
    } catch (const exception& e) {
        cerr << "Общая ошибка: " << e.what() << endl;
        cout << "---" << endl;
    }
}

int main()
{
    try {
        locale::global(locale(""));
    } catch (...) {
    }
    
    cout << "Тестирование шифра:" << endl;
    cout << "===================" << endl;
    
    check("ЛАБУБУ", "ВАЙБИК");
    check("ТИМП", "ПРОГА");
    check("РАСШИВРОВКА", "КОДИК");
    check("123", "КЛЮЧ");
    check("ТИМП", "");
    check("Privet", "KEY");
    check("ТИМП", "КЛЮЧ", true);
    
    return 0;
}