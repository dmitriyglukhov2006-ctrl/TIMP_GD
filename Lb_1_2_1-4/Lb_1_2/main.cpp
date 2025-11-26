#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

using namespace std;

// Функции для конвертации между string и wstring
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wstring_to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void check(const string& Text, const int key)
{
    try {
        // Преобразуем в wide string
        wstring wText = string_to_wstring(Text);
        
        modAlphaCipher cipher(key);
        wstring cipherText = cipher.encrypt(wText);
        wstring decryptedText = cipher.decrypt(cipherText);
        
        cout << "key=" << key << endl;
        cout << "text=" << Text << endl;
        cout << "encrypted=" << wstring_to_string(cipherText) << endl;
        cout << "decrypted=" << wstring_to_string(decryptedText) << endl;
        
        if (wText == decryptedText)
            cout << "Ok\n";
        else
            cout << "Err\n";
            
    } catch (const cipher_error& e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << "---" << endl;
}

int main()
{
    // Устанавливаем локаль для поддержки русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Тестирование маршрутной перестановки с русским текстом
    check("ПРОГРАММИРОВАНИЕ", 4);
    check("ПРИВЕТМИР", 3);
    check("ШИФРОВАНИЕТЕКСТА", 5);
    check("КОМПЬЮТЕРНАЯЛИНГВИСТИКА", 6);
    
    return 0;
}