#include <iostream>
#include <string>
#include "modAlphaCipher.h"

using namespace std;

bool isValidKey(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return !s.empty();
}

int main()
{
    string key_str;
    string text;
    unsigned op;
    
    cout << "Route Cipher ready. Input key (number): ";
    cin >> key_str;
    
    if (!isValidKey(key_str)) {
        cerr << "Error: key must be positive number\n";
        return 1;
    }
    
    int key = stoi(key_str);
    try {
        modAlphaCipher cipher(key);
        cout << "Key loaded\n";
        
        do {
            cout << "Input operation (0-exit, 1-encrypt, 2-decrypt): ";
            cin >> op;
            
            if (op > 2) {
                cout << "Illegal operation\n";
            } else if (op > 0) {
                cout << "Input text: ";
                cin.ignore();
                getline(cin, text);
                
                try {
                    if (op == 1) {
                        cout << "Encrypted text: " << cipher.encrypt(text) << endl;
                    } else {
                        cout << "Decrypted text: " << cipher.decrypt(text) << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Operation aborted: " << e.what() << endl;
                }
            }
        } while (op != 0);
        
    } catch (const cipher_error& e) {
        cerr << "Initialization error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}