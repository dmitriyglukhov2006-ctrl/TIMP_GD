#include "TableRouteCipher.h"
#include <algorithm>
#include <iostream>
#include <vector>

Cipher::Cipher(int skey, string text) {
    key = getValidKey(skey, text);
}

string Cipher::encryption(string& text) {
    string t = getValidOpenText(text);
    int length = t.size();
    
    // Расчет количества строк
    int rows = (length + key - 1) / key;
    
    // Создание таблицы
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    // Заполнение таблицы по строкам слева направо
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < length) {
                table[i][j] = t[index++];
            }
        }
    }
    
    // Чтение по спирали: СНИЗУ ВВЕРХ по столбцам СПРАВА НАЛЕВО
    string result;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = rows - 1; i >= 0; i--) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }
    
    return result;
}

string Cipher::transcript(string& text, string& open_text) {
    string t = getValidCipherText(text, open_text);
    int length = t.size();
    
    // Расчет количества строк
    int rows = (length + key - 1) / key;
    
    // Создание таблицы
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    // Определяем количество пустых ячеек в последней строке
    int empty_cells = rows * key - length;
    
    // Заполнение таблицы в обратном порядке шифрования
    int index = 0;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = rows - 1; i >= 0; i--) {
            // Пропускаем пустые ячейки в последней строке
            if (i == rows - 1 && j >= key - empty_cells) {
                continue;
            }
            if (index < length) {
                table[i][j] = t[index++];
            }
        }
    }
    
    // Чтение по строкам слева направо
    string result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }
    
    return result;
}

inline string Cipher::getValidCipherText(string& s, string& open_text) {
    if (s.empty())
        throw cipher_error("Отсутствует зашифрованный текст");
    return s;
}

inline string Cipher::getValidOpenText(string& s) {
    // Удаляем пробелы
    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    if (s.empty())
        throw cipher_error("Отсутствует открытый текст");

    // Проверяем что все символы - латинские буквы
    for (char c : s) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            throw cipher_error("Некорректные символы в строке. Разрешены только латинские буквы (A-Z, a-z).");
        }
    }

    return s;
}

inline int Cipher::getValidKey(int key, const string& Text) {
    if (key < 2 || key > Text.size())
        throw cipher_error("Некорректный размер ключа");
    
    return key;
}