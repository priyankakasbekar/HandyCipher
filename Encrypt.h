#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "HandyConvert.h"
#include "HandyFile.h"
#include "Key.h"

#include <map>
#include <string>

using namespace std;

class Encrypt
{
public:
    Encrypt();
    void begin_encryption(HandyFile *file_obj, Key *key_obj);

private:
    void initialize_encryption(HandyFile *file_obj, Key *key_obj);
    void initialize_lookup_table(string subkey);
    void get_current_component_details(HandyConvert *convert_obj, \
                                       char current_char,         \
                                       char previous_char);
    char select_row_col_diag(int number);
    int select_component_number(void);
    void get_elements_from_selected_component(Key *key_obj);
    bool check_if_next_char_single(int number);
    void encrypt_current_char(int index);
    bool check_encrypt_requirements(map<char, \
                                    IndexDetails>key_char_lookup);
    bool check_if_colinear(map<char, IndexDetails>key_char_lookup);
    bool check_if_on_prev_line(map<char, IndexDetails>key_char_lookup);
    string insert_null_characters(HandyFile *file_obj, Key *key_obj);
    bool insert_ciphertext_char(void);
    char select_random_null_character(Key* key_obj);


    map<char, int>  lookup;
    char   current_char;
    char   next_char;
    char   selected_component;
    int    component_number;
    string binary_string;
    string current_ciphertext;
    string ciphertext_with_nulls;
    string plaintext;
    string preliminary_ciphertext;
    string prev_selected_component;
    string previous_ciphertext;
    string selected_chars;

};  //Encrypt

#endif //ENCRYPT_H
