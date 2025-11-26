#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <memory>
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

bool isValidKey(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return !s.empty();
}

// Функция для проверки, является ли символ кириллицей
bool isCyrillicChar(wchar_t c) {
    return (c >= L'А' && c <= L'Я') || 
           (c >= L'а' && c <= L'я') || 
           c == L'Ё' || c == L'ё';
}

// Функция для проверки, содержит ли строка только кириллицу
bool containsOnlyCyrillic(const wstring& text) {
    for (wchar_t c : text) {
        if (!isCyrillicChar(c) && c != L' ') {
            return false;
        }
    }
    return true;
}

class CipherManager {
private:
    std::unique_ptr<modAlphaCipher> cipher;
    int current_key;
    
public:
    CipherManager() : current_key(0) {}
    
    bool setKey(int key) {
        try {
            cipher = std::make_unique<modAlphaCipher>(key);
            current_key = key;
            return true;
        } catch (const cipher_error& e) {
            return false;
        }
    }
    
    std::wstring encrypt(const std::wstring& text) {
        if (!cipher) {
            throw cipher_error("Ключ не установлен!");
        }
        return cipher->encrypt(text);
    }
    
    std::wstring decrypt(const std::wstring& text) {
        if (!cipher) {
            throw cipher_error("Ключ не установлен!");
        }
        return cipher->decrypt(text);
    }
    
    int getCurrentKey() const {
        return current_key;
    }
    
    bool isKeySet() const {
        return cipher != nullptr;
    }
};

// Функция для ввода текста с проверкой на кириллицу
string inputText(const string& prompt) {
    string text;
    while (true) {
        cout << prompt;
        getline(cin, text);
        
        if (text.empty()) {
            cout << "Ошибка: текст не может быть пустым. Попробуйте снова." << endl;
            continue;
        }
        
        // Преобразуем в wstring для проверки кириллицы
        wstring wtext = string_to_wstring(text);
        
        // Проверяем, есть ли в тексте хотя бы одна кириллическая буква
        bool hasCyrillic = false;
        for (wchar_t c : wtext) {
            if (isCyrillicChar(c)) {
                hasCyrillic = true;
                break;
            }
        }
        
        if (!hasCyrillic) {
            cout << "Ошибка: текст должен содержать хотя бы одну русскую букву. Попробуйте снова." << endl;
            continue;
        }
        
        // Проверяем, что текст содержит только кириллицу и пробелы
        if (!containsOnlyCyrillic(wtext)) {
            cout << "Ошибка: текст должен содержать только русские буквы и пробелы. Попробуйте снова." << endl;
            continue;
        }
        
        break;
    }
    return text;
}

// Функция для ввода ключа с проверкой
int inputKey() {
    string key_str;
    while (true) {
        cout << "Введите ключ (положительное число): ";
        cin >> key_str;
        cin.ignore(); // Очищаем буфер после ввода числа
        
        if (!isValidKey(key_str)) {
            cout << "Ошибка: ключ должен быть положительным числом! Попробуйте снова." << endl;
            continue;
        }
        
        int key = stoi(key_str);
        if (key <= 0) {
            cout << "Ошибка: ключ должен быть положительным числом! Попробуйте снова." << endl;
            continue;
        }
        
        return key;
    }
}

int main()
{
    // Настройка локали для поддержки русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "=== ШИФР МАРШРУТНОЙ ПЕРЕСТАНОВКИ ===" << endl;
    
    CipherManager cipherManager;
    
    // Установка начального ключа
    int initial_key = inputKey();
    if (!cipherManager.setKey(initial_key)) {
        cerr << "Ошибка при установке ключа!" << endl;
        return 1;
    }
    
    cout << "Ключ загружен" << endl;
    
    unsigned op;
    do {
        cout << "\nТекущий ключ: " << cipherManager.getCurrentKey() << endl;
        cout << "Выберите операцию:" << endl;
        cout << "0 - Выход" << endl;
        cout << "1 - Зашифровать текст" << endl;
        cout << "2 - Расшифровать текст" << endl;
        cout << "3 - Сменить ключ" << endl;
        cout << "Ваш выбор: ";
        cin >> op;
        cin.ignore(); // Очищаем буфер после ввода числа
        
        switch (op) {
            case 1: {
                string text = inputText("Введите текст для зашифрования: ");
                
                try {
                    wstring wtext = string_to_wstring(text);
                    wstring encrypted = cipherManager.encrypt(wtext);
                    cout << "Зашифрованный текст: " << wstring_to_string(encrypted) << endl;
                } catch (const cipher_error& e) {
                    cout << "Ошибка при шифровании: " << e.what() << endl;
                }
                break;
            }
            
            case 2: {
                string text = inputText("Введите текст для расшифрования: ");
                
                try {
                    wstring wtext = string_to_wstring(text);
                    wstring decrypted = cipherManager.decrypt(wtext);
                    cout << "Расшифрованный текст: " << wstring_to_string(decrypted) << endl;
                } catch (const cipher_error& e) {
                    cout << "Ошибка при расшифровании: " << e.what() << endl;
                }
                break;
            }
            
            case 3: {
                int new_key = inputKey();
                if (cipherManager.setKey(new_key)) {
                    cout << "Ключ успешно изменен на: " << new_key << endl;
                } else {
                    cout << "Ошибка при смене ключа!" << endl;
                }
                break;
            }
            
            case 0:
                cout << "Выход из программы." << endl;
                break;
                
            default:
                cout << "Неверная операция! Попробуйте снова." << endl;
                break;
        }
    } while (op != 0);
    
    return 0;
}