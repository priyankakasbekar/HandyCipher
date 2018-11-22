/*======================================================================*/
/* Filename: Encrypt.cpp                                                */
/*----------------------------------------------------------------------*/
/* Handles all encryption operations for HandyCipher.cpp.               */
/*======================================================================*/

#include "Encrypt.h"
#include "HandyConvert.h"
#include "Key.h"

#include <algorithm>
#include <string>
#include <sstream>

using namespace std;


/*======================================================================*/
/*                           Encrypt::Encrypt()                         */
/*======================================================================*/
/* Initializes all blank variables used in the Encrypt methods.         */
/*======================================================================*/
Encrypt::Encrypt()
{
    plaintext               = "";
    preliminary_ciphertext  = "";
    prev_selected_component = "";
    previous_ciphertext     = "";

}   //end Encrypt::Encrypt()


/*======================================================================*/
/*                   Encrypt::initialize_encryption()                   */
/*======================================================================*/
/* Calls methods to initialize structures and strings with values.      */
/*======================================================================*/
void Encrypt::initialize_encryption(HandyFile *file_obj, Key *key_obj)
{
    initialize_lookup_table(key_obj->subkey);

    plaintext = \
        file_obj->get_contents_from_file(file_obj->input_filename);

    //replace all spaces with '^' character because
    //^ is used instead of ' ' in handycipher
    replace(plaintext.begin(), plaintext.end(), ' ', '^');

}   //end Encrypt::initialize_encryption()


/*======================================================================*/
/*                   Encrypt::begin_encryption()                        */
/*======================================================================*/
/* After plaintext is pulled from the input file, this method steps     */
/* through plaintext to encrypt every plaintext char.                   */
/*                                                                      */
/* Once selected_component and component_number are successfully        */
/* chosen, the elements of that component are extracted into the string */
/* selected_chars. For example, if Row 4 consists of {A, B, C, D, E},   */
/* the resulting selected_chars is "ABCDE".                             */
/*                                                                      */
/* binary_string is set to the 5-digit binary representation of         */
/* the character's position in the key. For example, if the             */
/* character's position in key is 10, the binary string for that        */
/* character is "01010".                                                */
/*                                                                      */
/* The ciphertext cluster is determined through a separate method.      */
/* That ciphertext cluster is added to preliminary_ciphertext, which    */
/* holds the ciphertext before null characters are inserted.            */
/*                                                                      */
/* Once the program reaches the end of plaintext, it calls a method to  */
/* insert null characters into preliminary_ciphertext. After this is    */
/* completed, the final_ciphertext (with nulls inserted) is printed     */
/* to the output file.                                                  */
/*======================================================================*/
void Encrypt::begin_encryption(HandyFile *file_obj, Key *key_obj)
{
    string final_ciphertext   = "";
    HandyConvert *convert_obj = new HandyConvert();
    initialize_encryption(file_obj, key_obj);

    for (int index = 0; index < plaintext.length(); index++)
    {

        get_current_component_details(convert_obj,               \
                                      toupper(plaintext[index]), \
                                      toupper(plaintext[index+1]));

        get_elements_from_selected_component(key_obj);

        binary_string =   \
            convert_obj-> \
                int_to_binary_string(lookup[toupper(plaintext[index])]);

        encrypt_current_char(index);

		//check encryption requirements
        if (check_encrypt_requirements(key_obj->key_char_lookup) == 0)
        {
            //if requirements are not met,
            //restart the loop for that character
            index = index - 1;
        }
        else
        {
        previous_ciphertext     = current_ciphertext;
        preliminary_ciphertext += current_ciphertext;
        prev_selected_component = selected_component + \
                                  convert_obj->        \
                                      int_to_string(component_number);
        }
        current_ciphertext.clear();
        selected_chars.clear();
    }

    final_ciphertext = insert_null_characters(file_obj, key_obj);

    //write entire ciphertext with nulls inserted
    //to output file
    file_obj->print_to_file(file_obj->output_filename, \
                            final_ciphertext);

}   //end Encrypt::begin_encryption()


/*======================================================================*/
/*                  Encrypt::initialize_lookup_table()                  */
/*======================================================================*/
/* Initializes lookup for encryption. In lookup, the key of the map is  */
/* the character in the alphabet, and the value that the key maps to is */
/* the position of that character in the key.                        */
/*======================================================================*/
void Encrypt::initialize_lookup_table(std::string subkey)
{
    string alphabet  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.^?-";

	for (int i = 0; i <= 31; i++)
	{
		int position = subkey.find_first_of(alphabet[i]);
		lookup[alphabet[i]] = position + 1;
	}

}   //end Encrypt::initialize_lookup_table()


/*======================================================================*/
/*               Encrypt::get_current_component_details()               */
/*======================================================================*/
/* The program must choose a selected_component (row, column, diagonal) */
/* and component_number such that the two are not the same as those of  */
/* the previous encrypted plaintext char. For example, if the previous  */
/* plaintext char was encrypted using Row 4, the next plaintext char    */
/* cannot be encrypted with Row 4.                                      */
/*                                                                      */
/* Additionally, if the next character is a single character (if its    */
/* index in subkey is 1, 2, 4, 8, or 16, the current selected_component */
/* may not be chosen as 'R' (row) according to the core-cipher          */
/* algorithm of which handycipher is an extension.                      */
/*======================================================================*/
void Encrypt::get_current_component_details(HandyConvert *convert_obj,
                                            char current_char,
                                            char next_char) 
{
    do
    {
        selected_component = \
            select_row_col_diag(lookup[current_char]);

        component_number = select_component_number();

    } while (((selected_component + (convert_obj-> \
                int_to_string(component_number))) ==
                 prev_selected_component)                       ||
                 (check_if_next_char_single(lookup[next_char])  && 
                  selected_component == 'R'));

}   //end Encrypt::get_current_component_details()


/*======================================================================*/
/*                    Encrypt::select_row_col_diag()                    */
/*======================================================================*/
/* Chooses random number to randomly choose to encrypt the character    */
/* using row, column, or diagonal. If the position of the character,    */
/* denoted by the variable number in this method, in the subkey is      */
/* 1, 2, 4, 8, or 16, row- and diagonal-encryption cannot be chosen for */
/* the encryption method according to Step 1 of encryption. Thus, in    */
/* this case the cipher must choose column-encryption. Otherwise, the   */
/* choice of component (row, column, or diagonal) is random.            */
/*======================================================================*/
char Encrypt::select_row_col_diag(int number)
{
    char arr[3] = { 'R', 'C', 'D' };
    int random_index = 0;

    if ((number == 1) || (number == 2) || (number == 4) ||
        (number == 8) || (number == 16))
    {
        return 'C';
    }
    else
    {
        random_index = rand() % 3;
        return arr[random_index];
    }

}   //end Encrypt::select_row_col_diag()


/*======================================================================*/
/*                 Encrypt::select_component_number()                   */
/*======================================================================*/
/* The variable selected_component denotes whether column-, row- or     */
/* diagonal-encryption. If selected_component is 'C' or 'R', a random   */
/* number out of 5 is chosen because there are 5 possible rows and 5    */
/* possible columns to choose from in the matrix. Similarly, if         */
/* selected_component is 'D', one of the 10 diagonals is chosen.        */
/*======================================================================*/
int Encrypt::select_component_number(void)
{
    if ((selected_component == 'C') || (selected_component == 'R'))
    {
        return (rand() % 5);
    }
    else
    {
        return (rand() % 10);
    }

}   //end Encrypt::select_component_number()


/*======================================================================*/
/*                get_elements_from_selected_component()                */
/*======================================================================*/
/* Gets and returns the column, row, or diagonal, depending on the      */
/* encryption and row/column/diagonal number was chosen. The full row,  */
/* column, or diagonal is returned as a single string selected_chars.   */
/* For example, "ABCDE".                                                */
/*======================================================================*/
void Encrypt::get_elements_from_selected_component(Key *key_obj)
{
     int i = 0;
     int j = 0;

     switch (selected_component)
     {
     case 'C':
         for (i = 0; i <= 4; i++)
         {
             selected_chars =     \
                 selected_chars + \
                 key_obj->Key_Matrix[i][component_number];
         }
         return;
     case 'R':
         for (j = 0; j <= 4; j++)
         {
             selected_chars =     \
                selected_chars +  \
                key_obj->Key_Matrix[component_number][j];
         } 
         return;
     case 'D':
         //row is always increasing (moving downwards) so i++
         //depending on the number of the diagonal, j is either
         //decreasing (going backwards) or increasing (going forwards)
         selected_chars = key_obj->Diagonals[component_number];
         return;
     default:
         break;
     }

}   //end Encrypt::get_elements_from_selected_component()


/*======================================================================*/
/*                Encrypt::check_if_next_char_single()                  */
/*======================================================================*/
/* Check if next char is single by checking the index of that char in   */
/* subkey by using lookup table index (number) of that char.            */
/*======================================================================*/
bool Encrypt::check_if_next_char_single(int number)
{
    if ((number == 1) || (number == 2) || (number == 4) ||
        (number == 8) || (number == 16))
    {
        return 1;
    }

    return 0;

}   //end check_if_next_char_single()


/*======================================================================*/
/*                 Encrypt::encrypt_current_char()                      */
/*======================================================================*/
/* Encrypts the plaintext character using binary_string, which denotes  */
/* which characters to take in selected_chars. If there is a 1, the     */
/* program will take the character in that position in selected_chars.  */
/* The direction of encryption traversal depends on whether the         */
/* of the plaintext character is odd or even in the plaintext. The      */
/* traversal will be right-to-left (if row) or up-to-down (if column/   */
/* diagonal) if the position of the character in the plaintext is odd.  */
/* Else, the traversal will be left-to-right (if row) or down-to-up     */
/* (if diagonal). Please note that the position is incremented by 1.    */
/* Unlike in programming, the position of the plaintext characters      */
/* start at 1 and not 0.                                                */
/*                                                                      */
/* Example 1:                                                           */
/*     plaintext: "Hello"                                               */
/*     char being evaluated: 'H' (position 1)                           */
/*     selected_component: 'R' (Row)                                    */
/*     selected_chars (Row 3): "ABCDE"                                  */
/*     selected_chars: "ABCDE"                                          */
/*     binary_string: "10010"                                           */
/*     resulting ciphertext before permutation: "AD"                    */
/*                                                                      */
/* Example 2:                                                           */
/*     plaintext: "Hello"                                               */
/*     char being evaluated: '2' (position 2)                           */
/*     selected_component: 'R' (Row)                                    */
/*     component_number: 3                                              */
/*     selected_chars (Row 3): "ABCDE"                                  */
/*     binary_string: "10010"                                           */
/*     resulting ciphertext before permutation: "EB"                    */
/*======================================================================*/
void Encrypt::encrypt_current_char(int index)

{
    string::iterator it = binary_string.begin();

    //alternates direction of traversal
    //depending on whether the position of the character
    //in the plaintext is odd or even (in respect to the plaintext)
    if (((index+1) % 2) != 0)
    {
        for (int pos = 0; pos <= 4; pos++, it++)
        {
            if (*it == '1')
            {
                current_ciphertext += selected_chars[pos];
            }
        }
    }
    else
    {
        for (int pos = 4; pos >= 0; pos--, it++)
        {
            if (*it == '1')
            {
                current_ciphertext += selected_chars[pos];
            }
         }
    }

    //randomly permute the ciphertext string
    random_shuffle(current_ciphertext.begin(), \
                   current_ciphertext.end());

}   //end Encrypt::encryption_current_char()


/*======================================================================*/
/*                 Encrypt::check_encrypt_requirements()                */
/*======================================================================*/
/* Calls methods to check encryption requirements for handycipher.      */
/* These only execute if previous_ciphertext is not blank, as in, if    */
/* the program has already encrypted at least one plaintext character.  */
/* This is because the methods call for comparison with                 */
/* previous_ciphertext. If the encryption requirements are not met,     */
/* this method returns 0 and the program must create a new ciphertext   */
/* cluster for that plaintext char. Else, the method returns 1 and the  */
/* ciphertext is added.                                                 */
/*======================================================================*/
bool Encrypt::check_encrypt_requirements(map<char,
                                         IndexDetails>key_char_lookup)
{
    if (previous_ciphertext != "")
    {
        if ((previous_ciphertext.length() == 1) &&
            (check_if_colinear(key_char_lookup) == 1))
        {
            return (0);
        }
        else if (check_if_on_prev_line(key_char_lookup) == 1)
        {
            return (0);
        }
    }

    return (1);

}   //end Encrypt::check_encrypt_requirements()


/*======================================================================*/
/*                     Encrypt::check_if_colinear()                     */
/*======================================================================*/
/* Since previous_ciphertext was a single character, this function      */
/* checks if the first character of current_ciphertext is co-linear     */
/* with the single character of previous_ciphertext. If they are co-    */
/* linear, the loop restarts and a new current_ciphertext must be       */
/* chosen. This function returns 1 if the two characters are co-linear, */
/* and 0 if they are not colinear.                                      */
/*======================================================================*/
bool Encrypt::check_if_colinear(map<char, IndexDetails>key_char_lookup)
{
    if ((key_char_lookup[current_ciphertext[0]].row_number ==
          key_char_lookup[previous_ciphertext[0]].row_number)  ||
           (key_char_lookup[current_ciphertext[0]].col_number ==
             key_char_lookup[previous_ciphertext[0]].col_number)  ||
              (key_char_lookup[current_ciphertext[0]].diag_number_1 ==
                key_char_lookup[previous_ciphertext[0]].diag_number_1)  ||
                 (key_char_lookup[current_ciphertext[0]].diag_number_2 == 
                   key_char_lookup[previous_ciphertext[0]].diag_number_2))
    {
        return (1);
    }

    return (0);

}   //end Encrypt::check_if_colinear()


/*======================================================================*/
/*                    Encrypt::check_if_on_prev_line()                  */
/*======================================================================*/
/* Checks if first current_ciphertext character lies on the line used   */
/* to encrypt the previous plaintext character. Returns 1 if the        */
/* character does lie on the previous line used for encryption, and 0   */
/* if it does not. Note that this function only checks the              */
/* prev_selected_component (component R/C/D and number), and does not   */
/* check for colinearity for all components.                            */
/*======================================================================*/
bool Encrypt::check_if_on_prev_line(map<char,
                                    IndexDetails>key_char_lookup)
{
    char current_first_char   = current_ciphertext[0];
    string tmp_number         = "";
    int component_number      = 0;
    HandyConvert *convert_obj = new HandyConvert();

    tmp_number      += prev_selected_component[1];
    component_number = convert_obj->string_to_int(tmp_number);

    if ((prev_selected_component[0] == 'R') &&
         (key_char_lookup[current_first_char].row_number ==
                                          component_number))
    {
        return (1);
    }
    else if ((prev_selected_component[0] == 'C') &&
             (key_char_lookup[current_first_char].col_number ==
                                             component_number))
    {
        return (1);
    }
    else if ((prev_selected_component[0] == 'D') &&
             ((key_char_lookup[current_first_char].diag_number_1 ==
                                                   component_number) ||
              (key_char_lookup[current_first_char].diag_number_2 ==
                                                  component_number)))
    {
        return (1);
    }

    return (0);

}   //end Encrypt::check_if_on_prev_line()


/*======================================================================*/
/*                  Encrypt::insert_null_characters()                   */
/*======================================================================*/
/* Creates the final ciphertext, ciphertext_with_nulls, with the        */
/* following steps:                                                     */
/* (1) With probability 5/8, insert the current character from          */
/*     preliminary_ciphertext (C*) into ciphertext_with_nulls (C) and   */
/*     repeat from (1) considering the next character in C*. If there   */
/*     is no next character, still repeat from (1) and stop only when   */
/*     and stop only when there is a demand for a non-null.             */
/* (2) Instead choose to insert a null into C. This null N, should be   */
/*     randomly chosen from the set of 15, but protentially rejected    */
/*     in favor of another null by considering the current last six     */
/*     characters of C. If N last appears at a position n characters    */
/*     back from the end of C, that N should be rejected with           */
/*     probability (6-n)/5. This leads to 100% rejection at n=1, i.e.   */
/*     consecutive identical characters are not allowed. Once a null is */
/*     inserted, repeat (1) with same current character in C* as        */
/*     before, i.e. all characters in C* end up in C.                   */
/*======================================================================*/
string Encrypt::insert_null_characters(HandyFile *file_obj, Key *key_obj)
{
    string ciphertext_with_nulls = "";
    string::iterator it = preliminary_ciphertext.begin();
    char null_char      = '\0';
    char prev_null_char = '\0';
    int pos = 0;

    while (it != preliminary_ciphertext.end())
    {
        if (!insert_ciphertext_char())
        {
            //Step (1)
            ciphertext_with_nulls += *it;
            it++;
        }
        else
        {
            //Step (2)
            do
            {
                null_char = select_random_null_character(key_obj);
                pos = ciphertext_with_nulls.find_last_of(null_char);

            } while (((prev_null_char == null_char) ||
                      (((pos > 0) && 
                        (ciphertext_with_nulls.length()-pos) < 5))));

            ciphertext_with_nulls += null_char;
            prev_null_char = null_char;
        }
    }

    return (ciphertext_with_nulls);

}   //end Encrypt::insert_null_characters()


/*======================================================================*/
/*                  Encrypt::insert_ciphertext_char()                   */
/*======================================================================*/
/* With probability 5/8, inserts the current character from C*, or      */
/* preliminary_ciphertext in this program, into C, or the string        */
/* ciphertext_with_nulls in this program. If this function returns      */
/* true, the character from preliminary_ciphertext is inserted into     */
/* ciphertext_with_nulls. If it returns false, the program then finds   */
/* a null character to insert into ciphertext_with_nulls.               */
/*======================================================================*/
bool Encrypt::insert_ciphertext_char(void)
{
    return ((rand() % 8) > 4);

}   //end Encrypt::insert_ciphertext_char()


/*======================================================================*/
/*              Encrypt::select_random_null_character()                 */
/*======================================================================*/
/* Selects random null character out of 15 null characters.             */
/*======================================================================*/
char Encrypt::select_random_null_character(Key* key_obj)
{
    int random_null_index = 0;
    random_null_index = rand() % 15;

    return (key_obj->Null_Characters[random_null_index]);

}   //end select_random_null_character()

