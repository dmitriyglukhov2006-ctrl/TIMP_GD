#include "modAlphaCipher.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

// Реализация cipher_error для wstring
cipher_error::cipher_error(const std::wstring& what_arg) : 
    std::invalid_argument(std::string(what_arg.begin(), what_arg.end())) {}

cipher_error::cipher_error(const char* what_arg) : 
    std::invalid_argument(what_arg) {}

modAlphaCipher::modAlphaCipher(const int k)
{
    key = getValidKey(k);
}

int modAlphaCipher::getValidKey(const int k)
{
    if (k <= 0)
        throw cipher_error(L"Неверный ключ: ключ должен быть положительным числом");
    return k;
}

std::wstring modAlphaCipher::toValidText(const std::wstring& s)
{
    if (s.empty())
        throw cipher_error(L"Пустой текст");
    
    std::wstring result;
    for (wchar_t c : s) {
        // Проверяем, является ли символ русской буквой
        if ((c >= L'А' && c <= L'Я') || (c >= L'а' && c <= L'я') || c == L'Ё' || c == L'ё') {
            // Преобразуем в верхний регистр
            if (c >= L'а' && c <= L'я') {
                result.push_back(c - L'а' + L'А');
            } else if (c == L'ё') {
                result.push_back(L'Ё');
            } else {
                result.push_back(c);
            }
        }
    }
    
    if (result.empty())
        throw cipher_error(L"В тексте нет русских букв");
    
    return result;
}

std::vector<std::vector<wchar_t>> modAlphaCipher::createTable(const std::wstring& text)
{
    int len = text.length();
    int rows = (len + key - 1) / key;
    
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));
    
    int pos = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (pos < len) {
                table[i][j] = text[pos++];
            }
        }
    }
    
    return table;
}

std::wstring modAlphaCipher::readTableVertical(const std::vector<std::vector<wchar_t>>& table)
{
    std::wstring result;
    int rows = table.size();
    int cols = table[0].size();
    
    // Считывание сверху вниз, справа налево
    for (int j = cols - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (table[i][j] != L' ') {
                result.push_back(table[i][j]);
            }
        }
    }
    
    return result;
}

std::wstring modAlphaCipher::readTableHorizontal(const std::vector<std::vector<wchar_t>>& table)
{
    std::wstring result;
    int rows = table.size();
    int cols = table[0].size();
    
    // Считывание по горизонтали слева направо, сверху вниз
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (table[i][j] != L' ') {
                result.push_back(table[i][j]);
            }
        }
    }
    
    return result;
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    std::wstring text = toValidText(open_text);
    std::vector<std::vector<wchar_t>> table = createTable(text);
    return readTableVertical(table);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring text = toValidText(cipher_text);
    int len = text.length();
    int rows = (len + key - 1) / key;
    
    // Создаем пустую таблицу
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));
    
    // Заполняем таблицу в порядке считывания (сверху вниз, справа налево)
    int pos = 0;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            int original_pos = i * key + j;
            if (original_pos < len && pos < len) {
                table[i][j] = text[pos++];
            }
        }
    }
    
    return readTableHorizontal(table);
}