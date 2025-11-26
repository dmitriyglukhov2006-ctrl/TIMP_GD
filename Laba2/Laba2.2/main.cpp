#include <iostream>
#include "TableRouteCipher.h"
#include <limits>

using namespace std;

void encryptText() {
    string text;
    int key;
    
    cout << "Введите текст для шифрования (только латинские буквы): ";
    cin.ignore();
    getline(cin, text);
    
    cout << "Введите ключ (2-" << text.size() << "): ";
    cin >> key;
    
    try {
        Cipher cipher(key, text);
        string cipherText = cipher.encryption(text);
        cout << "Зашифрованный текст: " << cipherText << endl;
        
        // Показываем процесс для наглядности
        cout << "Проверка - расшифровка: ";
        string decryptedText = cipher.transcript(cipherText, text);
        cout << decryptedText << endl;
        cout << "Корректность: " << (text == decryptedText ? "✓ УСПЕХ" : "✗ ОШИБКА") << endl;
        
    } catch (const cipher_error & e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void decryptText() {
    string cipherText;
    string originalText;
    int key;
    
    cout << "Введите зашифрованный текст: ";
    cin.ignore();
    getline(cin, cipherText);
    
    cout << "Введите исходный текст (для проверки): ";
    getline(cin, originalText);
    
    cout << "Введите ключ (2-" << originalText.size() << "): ";
    cin >> key;
    
    try {
        Cipher cipher(key, originalText);
        string decryptedText = cipher.transcript(cipherText, originalText);
        cout << "Расшифрованный текст: " << decryptedText << endl;
        cout << "Совпадение с исходным: " << (originalText == decryptedText ? "✓ ДА" : "✗ НЕТ") << endl;
    } catch (const cipher_error & e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void testExamples() {
    cout << "=== ТЕСТОВЫЕ ПРИМЕРЫ ===" << endl;
    
    vector<pair<string, int>> testCases = {
        {"HELLO", 2},
        {"ABC", 2},
        {"TEST", 3},
        {"HelloWorld", 3},
        {"PostavteMaksimum", 4},
        {"Programming", 4},
        {"Cipher", 2},
        {"Text", 3}
    };
    
    for (auto& test : testCases) {
        string text = test.first;
        int key = test.second;
        
        cout << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬" << endl;
        cout << "Ключ: " << key << ", Текст: " << text << endl;
        try {
            Cipher cipher(key, text);
            string cipherText = cipher.encryption(text);
            string decryptedText = cipher.transcript(cipherText, text);
            
            cout << "Зашифрованный: " << cipherText << endl;
            cout << "Расшифрованный: " << decryptedText << endl;
            cout << "Результат: " << (text == decryptedText ? "✓ УСПЕХ" : "✗ ОШИБКА") << endl;
        } catch (const cipher_error & e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬" << endl;
}

void showMenu() {
    cout << "\n"
         << "╔══════════════════════════════════════╗\n"
         << "║      МАРШРУТНОЕ ШИФРОВАНИЕ          ║\n"
         << "║             ТАБЛИЦЕЙ                ║\n"
         << "╠══════════════════════════════════════╣\n"
         << "║ 1. Зашифровать текст                ║\n"
         << "║ 2. Расшифровать текст               ║\n"
         << "║ 3. Показать тестовые примеры        ║\n"
         << "║ 4. Инструкция                       ║\n"
         << "║ 0. Выход                            ║\n"
         << "╚══════════════════════════════════════╝\n"
         << "Выберите действие: ";
}

void showInstructions() {
    cout << "\n"
         << "╔══════════════════════════════════════╗\n"
         << "║           ИНСТРУКЦИЯ                ║\n"
         << "╚══════════════════════════════════════╝\n"
         << "▸ Текст записывается в таблицу по строкам\n"
         << "▸ Чтение происходит по столбцам:\n"
         << "  • Справа налево\n"
         << "  • В каждом столбце снизу вверх\n"
         << "▸ Ключ - количество столбцов в таблице\n"
         << "▸ Ключ должен быть от 2 до длины текста\n"
         << "▸ Поддерживаются ТОЛЬКО латинские буквы\n"
         << "▸ Пробелы автоматически удаляются\n\n"
         << "Пример для 'HELLO' с ключом 2:\n"
         << "Таблица: H E\n"
         << "         L L\n"
         << "         O _\n"
         << "Шифрование: OLHLE\n";
}

int main() {
    int choice;
    
    cout << "Добро пожаловать в программу маршрутного шифрования!" << endl;
    
    do {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                encryptText();
                break;
            case 2:
                decryptText();
                break;
            case 3:
                testExamples();
                break;
            case 4:
                showInstructions();
                break;
            case 0:
                cout << "Спасибо за использование программы! До свидания!" << endl;
                break;
            default:
                cout << "❌ Неверный выбор! Попробуйте снова." << endl;
                break;
        }
        
        if (choice != 0) {
            cout << "\nНажмите Enter для продолжения...";
            cin.ignore();
            cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}