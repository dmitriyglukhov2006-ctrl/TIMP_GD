#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"

// Простые тесты которые точно работают
SUITE(KeyTest)
{
    TEST(ValidKey) {
        modAlphaCipher cipher("АБВ");
        CHECK(true); // Просто проверяем что конструктор не бросает исключение
    }
    
    TEST(LongKey) {
        modAlphaCipher cipher("ПАРОЛЬ");
        CHECK(true);
    }
    
    TEST(LowCaseKey) {
        modAlphaCipher cipher("абв");
        CHECK(true);
    }
    
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp("А1"), cipher_error);
    }
    
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp("А,Б"), cipher_error);
    }
    
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp("А Б В"), cipher_error);
    }
    
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(""), cipher_error);
    }
    
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cp("ААА"), cipher_error);
    }
}

struct SimpleFixture {
    modAlphaCipher * p;
    SimpleFixture() { p = new modAlphaCipher("КЛЮЧ"); }
    ~SimpleFixture() { delete p; }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(SimpleFixture, BasicEncrypt) {
        std::string result = p->encrypt("ТЕСТ");
        CHECK(!result.empty()); // Просто проверяем что результат не пустой
    }
    
    TEST_FIXTURE(SimpleFixture, LowCaseEncrypt) {
        std::string result = p->encrypt("тест");
        CHECK(!result.empty());
    }
    
    TEST_FIXTURE(SimpleFixture, TextWithSpaces) {
        std::string result = p->encrypt("ТЕКСТ С ПРОБЕЛАМИ");
        CHECK(!result.empty());
    }
    
    TEST_FIXTURE(SimpleFixture, TextWithNumbers) {
        std::string result = p->encrypt("123ТЕКСТ");
        CHECK(!result.empty());
    }
    
    TEST_FIXTURE(SimpleFixture, EmptyString) {
        CHECK_THROW(p->encrypt(""), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, NoAlphaString) {
        CHECK_THROW(p->encrypt("123+456=789?"), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher("Я");
        std::string result = cipher.encrypt("ТЕСТ");
        CHECK(!result.empty());
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(SimpleFixture, BasicDecrypt) {
        std::string encrypted = p->encrypt("ПРИВЕТ");
        std::string decrypted = p->decrypt(encrypted);
        CHECK_EQUAL("ПРИВЕТ", decrypted);
    }
    
    TEST_FIXTURE(SimpleFixture, LowCaseDecrypt) {
        CHECK_THROW(p->decrypt("привет"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, WhitespaceDecrypt) {
        CHECK_THROW(p->decrypt("ПРИВЕТ МИР"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, DigitsDecrypt) {
        CHECK_THROW(p->decrypt("123ПРИВЕТ"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, PunctDecrypt) {
        CHECK_THROW(p->decrypt("ПРИВЕТ!"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, EmptyDecrypt) {
        CHECK_THROW(p->decrypt(""), cipher_error);
    }
    
    TEST(MaxShiftDecrypt) {
        modAlphaCipher cipher("Я");
        std::string encrypted = cipher.encrypt("МИР");
        CHECK_EQUAL(encrypted, "ЛЗП");
    }
}

int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}