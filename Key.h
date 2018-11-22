#ifndef KEY_H
#define KEY_H

#include <map>
#include <string>

using namespace std;

class IndexDetails
{
public:
    int row_number;
    int col_number;
    int diag_number_1;
    int diag_number_2;
    int diag_index_1;
    int diag_index_2;

    IndexDetails(int row_num, int col_num)
    {
        row_number = row_num;
        col_number = col_num;
    }
    IndexDetails()
    {
    }

};  //IndexDetails


class Key
{
public:
	Key(string key);

    char   Key_Matrix[5][8];
    char   Null_Characters[15];
    string Diagonals[10];

    string subkey;
    string key;	
    map<char, IndexDetails> key_char_lookup;

private:
    void extract_subkey(string key);
    void initialize_key_matrix(string key);
    void get_all_diagonals();
    void get_all_null_characters();

};  //Key

#endif  //KEY_H

