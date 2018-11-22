#ifndef DECRYPT_H
#define DECRYPT_H

#include "Encrypt.h"
#include "HandyFile.h"
#include "Key.h"
#include <map>
#include <string>

using namespace std;

class Decrypt
{
public:
    Decrypt();
    void begin_decryption(HandyFile *file_obj, Key *key_obj);

    string decrypted_plaintext;

private:
	void initialize_decryption(HandyFile *file_obj, Key *key_obj);
	void initialize_reverse_lookup(string subkey);
	void get_ciphertext_for_char(int &i, Key *key_obj);
    bool get_selected_component(char current_char, Key* key_obj);
    bool check_if_colinear(char current_char, Key * key_obj);
    void update_binary_string(Key *key_obj);
	void get_component_index(char current_char, \
                             map<char, IndexDetails>key_char_lookup);
	void decrypt_current_ciphertext();

    string binary_string;
    string current_ciphertext;
    string selected_component;
    string ciphertext;
    int component_index;
    bool decryption_direction;
    map<int, char> reverse_lookup;

};  //Decrypt

#endif //DECRYPT_H

