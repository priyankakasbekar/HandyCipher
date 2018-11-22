/*======================================================================*/
/* Filename: Decrypt.cpp                                                */
/*----------------------------------------------------------------------*/
/* Handles all decryption operations for HandyCipher.cpp.               */
/*======================================================================*/

#include "Decrypt.h"
#include "Encrypt.h"
#include "Key.h"
#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>

using namespace std;


/*======================================================================*/
/*                          Decrypt::Decrypt()                          */
/*======================================================================*/
/* Initializes all blank variables and strings used in Decrypt methods. */
/*======================================================================*/
Decrypt::Decrypt()
{
    binary_string        = "00000";
    selected_component   = "";
    current_ciphertext   = "";
    decrypted_plaintext  = "";

    component_index      = 0;
	decryption_direction = 0;

}   //end Decrypt::Decrypt()


/*======================================================================*/
/*                  Decrypt::initialize_decryption()                    */
/*======================================================================*/
/* Calls methods to initialize structures and strings with values.      */
/*======================================================================*/
void Decrypt::initialize_decryption(HandyFile *file_obj, Key *key_obj)
{
    initialize_reverse_lookup(key_obj->subkey);

    ciphertext = \
        file_obj->get_contents_from_file(file_obj->input_filename);

}   //end Decrypt::initialize_decryption()


/*======================================================================*/
/*                     Decrypt::begin_decryption()                      */
/*======================================================================*/
/* After ciphertext is pulled from the input file, this method steps    */
/* through ciphertext until a non-null, non-space character is found.   */
/*                                                                      */
/* Then, it calls methods to get the entire ciphertext for a single     */
/* plaintext char and to decrypt the plaintext char. After each         */
/* plaintext char is added to decrypted_plaintext, the strings          */
/* binary_string and current_ciphertext must be "cleared" or returned   */
/* to their original value at the beginning of Decrypt::Decrypt() to be */
/* used for the next cluster of ciphertext chars.                       */
/*                                                                      */
/* Once the program has reached the end of ciphertext, the final        */
/* decrypted_plaintext is printed to file.                              */
/*======================================================================*/
void Decrypt::begin_decryption(HandyFile *file_obj, Key *key_obj)
{
    initialize_decryption(file_obj, key_obj);

    for (int i = 0; i < ciphertext.length(); i++)
    {
        if (ciphertext[i] == ' ')
        {
            //if space, ignore
        }
        else if (key_obj->key_char_lookup[ciphertext[i]].col_number > 4)
        {
            //col_number must be equal to or less than 4
            //for the char to not be a null character

            //if null character, ignore
        }
        else if (current_ciphertext == "")
        {
            get_ciphertext_for_char(i, key_obj);
            decrypt_current_ciphertext();

            //"clear" strings
            current_ciphertext = "";
        	binary_string      = "00000";
        }
    }

    file_obj->print_to_file(file_obj->output_filename, \
                            decrypted_plaintext);

}   //end Decrypt::begin_decryption()


/*======================================================================*/
/*                Decrypt::initialize_reverse_lookup()                  */
/*======================================================================*/
/* Initializes reverse_lookup for decryption. In reverse_lookup, the    */
/* key of the map is the position of the character in the subkey, and   */
/* the value that the key maps to is the character in the alphabet that */
/* corresponds to that position.                                        */
/*======================================================================*/
void Decrypt::initialize_reverse_lookup(string subkey)
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.^?-";

    for (int i = 0; i <= 31; i++)
    {
        int position = subkey.find_first_of(alphabet[i]);
        reverse_lookup[position + 1] = alphabet[i];
    }

}   //end Decrypt::initialize_reverse_lookup()


/*======================================================================*/
/*                   Decrypt::get_ciphertext_for_char()                 */
/*======================================================================*/
/* Continues to append non-space, non-null characters that are colinear */
/* with the first character of current_ciphertext. Once a non-colinear  */
/* character is found, the binary string used for encryption can be     */
/* determined.                                                          */
/*======================================================================*/
void Decrypt::get_ciphertext_for_char(int &i, Key *key_obj)
{
    current_ciphertext = ciphertext[i++];

    while (1)
    {
        if (i >= ciphertext.length())
        {
            //if reach end of ciphertext
            if (current_ciphertext.length() == 1)
            {
                selected_component = "C";
                component_index = key_obj-> \
                    key_char_lookup[current_ciphertext[0]].row_number;
            }
            update_binary_string(key_obj);
            break;
        }
        else if (ciphertext[i] == ' ')
        {
            //if space character, ignore
            i++;
        }
        else if (key_obj->key_char_lookup[ciphertext[i]].col_number > 4)
        {
            //col_number must be equal to or less than 4
            //for the char to not be a null character

            //if null character, ignore
            i++;
        }
        else if ((current_ciphertext.length() == 1) &&
                 (get_selected_component(ciphertext[i],
                                         key_obj) == 1))
        {
            current_ciphertext += ciphertext[i];
            i++;
        }
        else if ((current_ciphertext.length() == 1) &&
                 (get_selected_component(ciphertext[i],
                                         key_obj) == 0))
        {
            //single character not colinear with
            //the next non-null and non-space character
            //so we know the index of the character is
            //1, 2, 4, 8, or 16, and the component must be column
            selected_component = "C";
            update_binary_string(key_obj);
            i--;
            break;
        }
        else if ((!current_ciphertext.empty()) &&
                 (check_if_colinear(ciphertext[i], key_obj) == 1))
        {
            //append to the existing current_ciphertext
            current_ciphertext += ciphertext[i];
            i++;
        }
        else
        {
            //non-colinear char is found
            //index is decreased to re-evaluate that char
            update_binary_string(key_obj);
            i--;
            break;
        }
    }

}   //end Decrypt::get_ciphertext_for_char()


/*======================================================================*/
/*                   Decrypt::get_selected_component()                  */
/*======================================================================*/
/* Checks whether the first and next non-space non-null characters of   */
/* current_ciphertext are colinear by checking if their row, column, or */
/* diagonal numbers are the same. If a match is found, the              */
/* selected_component is set to the component they both lie on, and 1   */
/* is returned. If none of their component numbers are the same, the    */
/* two characters are not colinear and 0 is returned.                   */
/*======================================================================*/
bool Decrypt::get_selected_component(char current_char, Key *key_obj)
{
    char previous_char = current_ciphertext[0];

    if (key_obj->key_char_lookup[current_char].row_number ==
        key_obj->key_char_lookup[previous_char].row_number)
    {
        selected_component = "R";
        return (1);
    }
    else if (key_obj->key_char_lookup[current_char].col_number ==
             key_obj->key_char_lookup[previous_char].col_number)
    {
        selected_component = "C";
        return (1);
    }
    else if ((key_obj->key_char_lookup[current_char].diag_number_1 ==
              key_obj->key_char_lookup[previous_char].diag_number_1))
    {
        selected_component = "D1";
        return (1);
    }
    else if ((key_obj->key_char_lookup[current_char].diag_number_2 ==
              key_obj->key_char_lookup[previous_char].diag_number_2))
    {
        selected_component = "D2";
        return (1);
    }

    return (0);

}   //end Decrypt::get_selected_component()


/*======================================================================*/
/*                    Decrypt::check_if_colinear()                      */
/*======================================================================*/
/* If current_ciphertext is of length 2 or greater, this function       */
/* determines whether the next non-null, non-space character in cipher- */
/* text is colinear with the previous characters in current_ciphertext. */
/* If the next char is colinear with the previous chars, the function   */
/* returns 1. Else, the function returns 0.                             */
/*======================================================================*/
bool Decrypt::check_if_colinear(char current_char, Key *key_obj)
{
    char previous_char = \
         current_ciphertext[current_ciphertext.length()-1];

    if ((selected_component == "R") &&
         (key_obj->key_char_lookup[current_char].row_number ==
           key_obj->key_char_lookup[previous_char].row_number))
    {
        return (1);
    }
    else if ((selected_component == "C") &&
              (key_obj->key_char_lookup[current_char].col_number ==
                key_obj->key_char_lookup[previous_char].col_number))
    {
        return (1);
    }
    else if ((selected_component == "D1") &&
              (key_obj->key_char_lookup[current_char].diag_number_1 ==
                key_obj->key_char_lookup[previous_char].diag_number_1))
    {
        return (1);
    }
    else if ((selected_component == "D2") &&
              (key_obj->key_char_lookup[current_char].diag_number_2 ==
                key_obj->key_char_lookup[previous_char].diag_number_2))
    {
        return (1);
    }

    return (0);

}   //end Decrypt::check_if_colinear()


/*======================================================================*/
/*                  Decrypt::update_binary_string()                     */
/*======================================================================*/
/* For each character in current_ciphertext, this function inserts a    */
/* '1' for that character's component_index if the current              */
/* decryption_direction is 0 (indicating right-to-left for rows, or up- */
/* to-down for columns or diagonals). If the current                    */
/* decryption_direction is 1 (indicating left-to-right for rows, or     */
/* down-to-up for columns or diagonals), the binary string should be    */
/* reversed. For example, if the binary string is 01000 when            */
/* decryption_direction is 0, then it should be 00010 when              */
/* decryption_direction is 1. Thus, instead of inserting the '1' at     */
/* position 1, the function will instead insert the '1' at position 3.  */
/* Since the direction of encryption alternates for every plaintext     */
/* char that is encrypted, decryption_direction is also changed after   */
/* each ciphertext cluster is found.                                    */
/*======================================================================*/
void Decrypt::update_binary_string(Key *key_obj)
{
	for (int i = 0; i < current_ciphertext.length(); i++)
	{
		get_component_index(current_ciphertext[i], \
                            key_obj->key_char_lookup);

		if (decryption_direction == 0)
		{
			binary_string.at(component_index) = '1';
		}
		else
		{
            //reverse binary string traversal
			if (component_index == 4)      component_index = 0;
			else if (component_index == 3) component_index = 1;
			else if (component_index == 1) component_index = 3;
			else if (component_index == 0) component_index = 4;

			binary_string.at(component_index) = '1';
		}
	}

    //update decryption_direction
	(decryption_direction == 0) ?
         decryption_direction = 1 : decryption_direction = 0;


}   //end Decrypt::update_binary_string()


/*======================================================================*/
/*                   Decrypt::get_component_index()                     */
/*======================================================================*/
/* Gets component_index, which is the index of the character on the     */
/* component number. For example, if the row 5 is "ABCDE", the          */
/* component_index of "C" in row 5 is 2, since it is index 2 in the     */
/* row.                                                                 */
/*======================================================================*/
void Decrypt::get_component_index(char current_char,
                                  map<char, IndexDetails>key_char_lookup)
{
	if (selected_component     == "R")
	{
		component_index = key_char_lookup[current_char].col_number;
	}
	else if (selected_component == "C")
	{
		component_index = key_char_lookup[current_char].row_number;
	}
	else if (selected_component == "D1")
	{
		component_index = key_char_lookup[current_char].diag_index_1;
	}
	else if (selected_component == "D2")
	{
		component_index = key_char_lookup[current_char].diag_index_2;
	}

}   //end Decrypt::get_component_index()


/*======================================================================*/
/*                 Decrypt::decrypt_current_ciphertext()                */
/*======================================================================*/
/* Now that the program has the binary string used for traversal, it    */
/* can find the encrypted plaintext char using reverse_lookup.          */
/*======================================================================*/
void Decrypt::decrypt_current_ciphertext()
{
    char *end;

    int converted_int = strtol(binary_string.c_str(), &end, 2);

    //write decrypted plaintext char to output file	
    if (reverse_lookup[converted_int] == '^')
    {
         decrypted_plaintext += ' ';
    }
    else
    {
         decrypted_plaintext += reverse_lookup[converted_int];
    }

}   //end Decrypt::decrypt_current_ciphertext()

