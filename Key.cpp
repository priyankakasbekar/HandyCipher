/*======================================================================*/
/* Filename: Key.cpp                                                    */
/*----------------------------------------------------------------------*/
/* Handles all key operations and stores all key-related variables and  */
/* structures for Handycipher.cpp.                                      */
/*======================================================================*/

#include "Key.h"

#include <iostream>

using namespace std;


/*======================================================================*/
/*                               Key::Key()                             */
/*======================================================================*/
/* Initializes all variables and structures needed for the Key object.  */
/*======================================================================*/
Key::Key(string key)
{
	this->key = key;
	extract_subkey(this->key);
	initialize_key_matrix(this->key);

}   //end Key::Key()


/*======================================================================*/
/*                        Key::extract_subkey()                         */
/*======================================================================*/
/* A 31-plaintext-character subkey is derived from the key by omitting  */
/* the decimal digits [1].                                              */
/*======================================================================*/
void Key::extract_subkey(string key)
{
    string::iterator it = key.begin();

    //extract subkey from key
    for (int i = 0; i <= 40; i++)
    {
        if (!isdigit(*it))
        {
            subkey += *it;
        }
        it++;
    }

}   //end Key::extract_subkey()


/*======================================================================*/
/*                     Key::initialize_key_matrix()                     */
/*======================================================================*/
/* The 40 non-space characters (omitting '^') of the key are displayed  */
/* as a 5 x 8 table, Key_Matrix [1].                                    */
/*======================================================================*/
void Key::initialize_key_matrix(string key)
{
    //first remove '^' character
    key.erase(key.find_first_of('^'), 1);

    string::iterator it = key.begin();

    //fill the key matrix
    it = key.begin();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Key_Matrix[i][j] = *it;

            //assign row and column number
            IndexDetails *num = new IndexDetails(i, j);
            key_char_lookup[*it] = *num;

            it++;
        }
    }

    get_all_diagonals();
    get_all_null_characters();

}   //end Key::initialize_key_matrix()

 
/*======================================================================*/
/*                     Key::get_all_diagonals()                         */
/*======================================================================*/
/* Gets all 10 diagonals in Key_Matrix. Diagonals are saved as a single */
/* string. For example, Diagonals[2] = "ABCDE".                         */
/*                                                                      */
/* Each non-null character belongs to two diagonals.                    */
/* diag_number_1 denotes diagonal numbers 0 to 4.                       */
/* diag_number_2 denotes diagonal numbers 5 to 9.                       */
/*                                                                      */
/* diag_index_1 stores the index of the character in diag_number_1.     */
/* diag_index_2 stores the index of the character in diag_number_2.     */
/*                                                                      */
/* In our above example, when evaluating character 'D', the program     */
/* would store its diag_number_1 as 2 (since it is in Diagonals[2]),    */
/* and its diag_index_1 as 3 (since it is position 3 in Diagonals[2]).  */
/*======================================================================*/
void Key::get_all_diagonals()
{
    int i = 0, j = 0;
    
    //row is always increasing (moving downwards) so i++
    //depending on the number of the diagonal, j is either
    //decreasing (going backwards) or increasing (going forwards)
    for (int number = 0; number < 5; number++)
    {
        i = 0;
        j = number;

        for (int counter = 0; counter < 5; counter++)
        {
            Diagonals[number] = Diagonals[number] + Key_Matrix[i][j];

            key_char_lookup[Key_Matrix[i][j]].diag_number_1 = number;
            key_char_lookup[Key_Matrix[i][j]].diag_index_1  = counter;

            i++;
            j++; 

            if ((j > 4) && (counter < 4))
            {
               j = 0;
            }
        }
    }
    for (int number = 5; number < 10; number++)
    {
        i = 0;
        j = number - 5; 
        for (int counter = 0; counter < 5; counter++)
        {
            Diagonals[number] = Diagonals[number] + Key_Matrix[i][j];
            key_char_lookup[Key_Matrix[i][j]].diag_number_2 = number;
            key_char_lookup[Key_Matrix[i][j]].diag_index_2  = counter;
            i++;
            j--; 

            if ((j < 0) && (counter < 4))
            {
               j = 4;
            }
        } 
    }


}   //end Key::get_all_diagonals()


/*======================================================================*/
/*                    Key::get_all_null_characters()                    */
/*======================================================================*/
/* Gets all 15 null characters from Key_Matrix as an array of chars,    */
/* Null_Characters[15]. The null characters lie in columns 5 to 7.      */
/*======================================================================*/
void Key::get_all_null_characters()
{
    int index = 0;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 5; j < 8; j++)
        {
            Null_Characters[index++] = Key_Matrix[i][j];
        }
    }

}   //end Key::get_all_null_characters()

