#include <UnitTest++/UnitTest++.h>
#include "route.h"
#include <string>

SUITE(KeyTest) {
    TEST(ValidKey) {
        code cipher(3, "HELLO");
        CHECK_EQUAL("LEHLO", cipher.encryption("HELLO"));
    }
    TEST(InvalidKeyTooSmall) {
        CHECK_THROW(code(1, "TEST"), cipher_error);
    }
    TEST(InvalidKeyTooLarge) {
        CHECK_THROW(code(10, "SHORT"), cipher_error);
    }
}

struct KeyThree_fixture {
    code * t;
    KeyThree_fixture() {
        t = new code(3, "HELLO");
    }
    ~KeyThree_fixture() {
        delete t;
    }
};

SUITE(EncryptTest) {
    TEST_FIXTURE(KeyThree_fixture, UpCaseString) {
        std::string input = "HELLO";
        CHECK_EQUAL("LEHLO", t->encryption(input));
    }
    TEST_FIXTURE(KeyThree_fixture, LowCaseString) {
        std::string input = "hello";
        CHECK_EQUAL("lehlo", t->encryption(input));
    }
    TEST_FIXTURE(KeyThree_fixture, StringWithWhitspace) {
        std::string input = "HE LL O";
        CHECK_EQUAL("LEHLO", t->encryption(input));
    }
    TEST_FIXTURE(KeyThree_fixture, StringWithNumbers) {
        std::string input = "HE1LO";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, EmptyString) {
        std::string input = "";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, NoAlphaString) {
        std::string input = "123+456";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST(SquareTable) {
        code cipher(4, "PROGRAM");
        std::string input = "PROGRAM";
        CHECK_EQUAL("GORPRAM", cipher.encryption(input));
    }
}

SUITE(DecryptTest) {
    TEST_FIXTURE(KeyThree_fixture, UpCaseString) {
        std::string encrypted = "LEHLO";
        std::string original = "HELLO";
        CHECK_EQUAL("HELLO", t->transcript(encrypted, original));
    }
    TEST_FIXTURE(KeyThree_fixture, LowCaseString) {
        std::string encrypted = "lehlo";
        std::string original = "hello";
        CHECK_EQUAL("hello", t->transcript(encrypted, original));
    }
    TEST_FIXTURE(KeyThree_fixture, WhitespaceInCipher) {
        std::string encrypted = "LEH LO";
        std::string original = "HELLO";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, DigitsInCipher) {
        std::string encrypted = "LE1LO";
        std::string original = "HELLO";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, DifferentLength) {
        std::string encrypted = "SHORT";
        std::string original = "LONGER";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, EmptyStrings) {
        std::string encrypted = "";
        std::string original = "";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST(SquareTableDecrypt) {
        code cipher(4, "PROGRAM");
        std::string encrypted = "GORPRAM";
        std::string original = "PROGRAM";
        CHECK_EQUAL("PROGRAM", cipher.transcript(encrypted, original));
    }
}

int main(int argc, char **argv) {
    return UnitTest::RunAllTests();
}