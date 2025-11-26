#include <iostream>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

using namespace std;

// Функция для преобразования string в wstring
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// Функция для преобразования wstring в string
std::string wstring_to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void check(const string& Text, const string& key)
{
    try {
        // Преобразуем в wide string
        wstring wText = string_to_wstring(Text);
        wstring wKey = string_to_wstring(key);
        
        modAlphaCipher cipher(wKey);
        wstring cipherText = cipher.encrypt(wText);
        wstring decryptedText = cipher.decrypt(cipherText);
        
        cout << "key=" << key << endl;
        cout << "text=" << Text << endl;
        cout << "cipherText=" << wstring_to_string(cipherText) << endl;
        cout << "decryptedText=" << wstring_to_string(decryptedText) << endl;
        
        if (wText == decryptedText)
            cout << "Ok\n";
        else
            cout << "Err\n";
            
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << "---" << endl;
}

int main()
{
    // Устанавливаем локаль для поддержки русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Тестируем с русским текстом
    check("ПРИВЕТ", "КЛЮЧ");
    check("ПРОГРАММИРОВАНИЕ", "ШАШЛЫК");
    check("КОМПЬЮТЕР", "ПАРОЛЬ");
    
    return 0;
}