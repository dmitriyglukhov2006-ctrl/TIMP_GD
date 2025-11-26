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

void runTest(const string& testName, const string& text, int key, bool shouldPass = true) {
    wcout << L"\n=== ТЕСТ: " << string_to_wstring(testName) << L" ===" << endl;
    
    try {
        modAlphaCipher cipher(key);
        wstring wtext = string_to_wstring(text);
        
        wstring encrypted = cipher.encrypt(wtext);
        wstring decrypted = cipher.decrypt(encrypted);
        
        wcout << L"Ключ: " << key << endl;
        wcout << L"Исходный текст: " << string_to_wstring(text) << endl;
        wcout << L"Зашифрованный: " << wstring_to_string(encrypted) << endl;
        wcout << L"Расшифрованный: " << wstring_to_string(decrypted) << endl;
        
        if (wtext == decrypted) {
            wcout << L"✅ ТЕСТ ПРОЙДЕН" << endl;
        } else {
            wcout << L"❌ ТЕСТ НЕ ПРОЙДЕН: тексты не совпадают" << endl;
        }
        
    } catch (const cipher_error& e) {
        if (shouldPass) {
            wcout << L"❌ ТЕСТ НЕ ПРОЙДЕН: " << e.what() << endl;
        } else {
            wcout << L"✅ ТЕСТ ПРОЙДЕН (ожидалась ошибка): " << e.what() << endl;
        }
    }
}

void runErrorTest(const string& testName, const string& text, int key) {
    wcout << L"\n=== ТЕСТ ОШИБКИ: " << string_to_wstring(testName) << L" ===" << endl;
    
    try {
        modAlphaCipher cipher(key);
        wstring wtext = string_to_wstring(text);
        wstring encrypted = cipher.encrypt(wtext);
        
        wcout << L"❌ ТЕСТ НЕ ПРОЙДЕН: ожидалась ошибка, но шифрование выполнено" << endl;
        wcout << L"Результат: " << wstring_to_string(encrypted) << endl;
        
    } catch (const cipher_error& e) {
        wcout << L"✅ ТЕСТ ПРОЙДЕН: " << e.what() << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    wcout << L"=== АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ ===" << endl;
    
    // Позитивные тесты
    runTest("Простой текст", "ПРИВЕТ", 3);
    runTest("Длинный текст", "ПРОГРАММИРОВАНИЕ", 4);
    runTest("Текст с пробелами", "ПРИВЕТ МИР", 2);
    runTest("Текст со знаками", "Привет, мир!", 3);
    runTest("Только русские буквы", "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", 5);
    
    // Тесты ошибок
    runErrorTest("Пустой текст", "", 3);
    runErrorTest("Текст без букв", "12345!@#$", 3);
    runErrorTest("Нулевой ключ", "ТЕКСТ", 0);
    runErrorTest("Отрицательный ключ", "ТЕКСТ", -5);
    
    wcout << L"\n=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===" << endl;
    
    return 0;
}