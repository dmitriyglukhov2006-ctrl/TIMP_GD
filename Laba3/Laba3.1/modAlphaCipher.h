#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <locale>
#include <codecvt>

/**
 * @file modAlphaCipher.h
 * @author Глухов Дмитрий
 * @version 1.0
 * @date 2025-12-5
 * @brief Заголовочный файл для модуля шифрования методом Гронсфельда
 */

class cipher_error: public std::invalid_argument {
    public:
        explicit cipher_error (const std::string& what_arg):
        std::invalid_argument(what_arg) {}
        explicit cipher_error (const char* what_arg):
        std::invalid_argument(what_arg) {}
};

/**
 * @class modAlphaCipher
 * @brief Класс для шифрования и расшифрования текста методом Гронсфельда
 * @details Использует алфавит из прописных английских букв
 */
class modAlphaCipher {
    private:
        std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
        std::map<wchar_t,int> alphaNum;
        std::vector<int> key;
        std::vector<int> convert(const std::string& s);
        std::string convert(const std::vector<int>& v);
        std::string getValidKey(const std::string & s);
        std::string getValidOpenText(const std::string & s);
        std::string getValidCipherText(const std::string & s);
    public:
        modAlphaCipher()=delete;
        
        /**
         * @brief Конструктор с ключом
         * @param skey Ключ в виде строки
         * @throw cipher_error Если ключ пустой или содержит недопустимые символы
         */
        modAlphaCipher(const std::string& skey);
        
        /**
         * @brief Шифрует открытый текст
         * @param open_text Текст для шифрования
         * @return Зашифрованная строка
         * @throw cipher_error Если текст пустой
         */
        std::string encrypt(const std::string& open_text);
        
        /**
         * @brief Расшифровывает зашифрованный текст
         * @param cipher_text Текст для расшифрования
         * @return Расшифрованная строка
         * @throw cipher_error Если текст пустой
         */
        std::string decrypt(const std::string& cipher_text);
};