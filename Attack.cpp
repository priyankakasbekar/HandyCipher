/*======================================================================*/
/* Filename: Attack.cpp                                                 */
/*----------------------------------------------------------------------*/
/* Handles all attack operations for HandyCipher.cpp.                   */
/*======================================================================*/

#include "Attack.h"
#include "Decrypt.h"
#include "Key.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

#define MAX_BRUTEFORCE_ATTEMPTS 2^40

#define USEDEBUG
#ifdef USEDEBUG
#define debug(x) cout << x
#else
#define debug(x)
#endif


/*======================================================================*/
/*                  Attack::generate_random_keystring()                 */
/*======================================================================*/
/* Generates a random key with the 41 characters of alphabet. This      */
/* key is "random" in relation to timestamp.                            */
/*======================================================================*/
string Attack::generate_random_keystring(void)
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.-?^0123456789";
    string key      = "";
    char next_selected_char;

    while (key.length() < 41)
    {
        do
        {
            next_selected_char = alphabet[rand() % 41];

        } while ((key.find(next_selected_char) < key.length()) &&
                 (key.length() != 41));

        key += next_selected_char;
    }

    return (key);

}   //end Attack::generate_random_keystring()


/*======================================================================*/
/*                        Attack::begin_attack()                        */
/*======================================================================*/
/* This method implements an attack with no known information about     */
/* the timestamps or key used to generate the ciphertext. The attacker  */
/* only has access to a known plaintext.                                */
/*                                                                      */
/* Gets known_plaintext from known_filename and saves substring of the  */
/* first 10 characters of known_plaintext into compare_known. Since     */
/* the decrypted plaintext will all be uppercase, compare_known is also */
/* transformed to uppercase to compare with the decrypted plaintext.    */
/* Only the first 10 characters of both known_plaintext and, later, the */
/* decrypted plaintext, to compare.                                     */
/*                                                                      */
/* For each keystring that has not already been tried, the program      */
/* "decrypts" the ciphertext with that keystring. If the first 10       */
/* characters of the known_plaintext and the first 10 characters of the */
/* decrypted plaintext are the same, the program registers that the     */
/* correct key has been found and prints the key to the output file.    */
/* Else, the program increments count and tries the next random, not    */
/* used before keystring. The program only tries up to a maximum of     */
/* 2^40 keystrings.                                                     */
/*======================================================================*/
void Attack::begin_attack(HandyFile *file_obj)
{
    long long count = 0;

    string ciphertext      = "";
    string compare_known   = "";
    string keystring       = "";
    string known_plaintext = "";

    fstream input_file;
    fstream output_file;
    fstream possible_plaintext;
    Key *key_obj;

    //get known_plaintext
    known_plaintext = \
         file_obj->get_contents_from_file(file_obj->known_filename);

    compare_known = known_plaintext.substr(0, 10);
    transform(compare_known.begin(), compare_known.end(), \
              compare_known.begin(), ::toupper);

    input_file.open(file_obj->input_filename, fstream::in);	
    file_obj->check_file_status(input_file, file_obj->input_filename);

    //get ciphertext
    getline(input_file, ciphertext);
            keystring = generate_random_keystring();

    srand(time(NULL));

    while (count < MAX_BRUTEFORCE_ATTEMPTS)
    {
        do
        {
            keystring = generate_random_keystring();

        } while (((attempted_keys_lookup.find(keystring) !=
                   attempted_keys_lookup.end())));

        //add to lookup to check if key was previously tried
        attempted_keys_lookup[keystring] = count;

        key_obj = new Key(keystring);
        Decrypt *decrypt_obj = new Decrypt();

        decrypt_obj->begin_decryption(file_obj, key_obj);
        debug("Decrypted_plaintext substring: ");
        debug(decrypt_obj->decrypted_plaintext.substr(0, 10) << endl);

        if (compare_known == 
            decrypt_obj->decrypted_plaintext.substr(0, 10))
        {
            file_obj->print_to_file(file_obj->output_filename, \
                                    keystring);

            debug("Found valid keystring! Printing to file.");
            debug("Keystring: " << keystring << endl);
            debug(endl);
            break;
        }
        else
        {
            debug("Attempted keystring " << count << ": ");
            debug(keystring << endl);
            debug(endl);
            count++;
        }

        delete (decrypt_obj);
        delete (key_obj);
    }

}   //end Attack::begin_attack()


/*======================================================================*/
/*                 Attack::begin_attack_using_seed_key()                */
/*======================================================================*/
/* This method implements an attack on our own system.                  */
/*                                                                      */
/* In this method, the start and end timestamps speculating when the    */
/* key for encryption was generated using this program, are sent in as  */
/* parameters. This greatly reduces the computation time.               */
/*                                                                      */
/* The reason this works is because when rand() is used with the        */
/* srand() that takes the seed, it takes a particular timestamp to      */
/* generate the key. If the attacker has a general idea of the          */
/* beginning and ending timestamp of encryption, s/he would have a      */
/* much easier time attacking our system.                               */
/*                                                                      */
/* This method is the same as the above method except that it           */
/* generates and tries a keystring for each time within the timestamp   */
/* range. This method removes the need for the map used in the above    */
/* method, attempted_keys_lookup, because none of the keystrings        */
/* generated will have been completed already in this attack. This is   */
/* because the key-generation is following the same pattern it          */
/* did in the past during the timespan specified.                       */
/*======================================================================*/
void Attack::begin_attack_using_seed_key(HandyFile *file_obj,
                                        time_t start,
                                        time_t end)
 
{
    long long count = 0;

    time_t t = time(NULL);

    string ciphertext      = "";
    string compare_known   = "";
    string keystring       = "";
    string known_plaintext = "";

    fstream input_file;
    fstream output_file;
    fstream possible_plaintext;
    Key *key_obj;

    //get known_plaintext
    known_plaintext = \
         file_obj->get_contents_from_file(file_obj->known_filename);

    compare_known = known_plaintext.substr(0, 10);
    transform(compare_known.begin(), compare_known.end(), \
              compare_known.begin(), ::toupper);

    input_file.open(file_obj->input_filename, fstream::in);	
    file_obj->check_file_status(input_file, file_obj->input_filename);

    //get ciphertext
    getline(input_file, ciphertext);

    for (time_t i = start; i < end; i++)
    {
        srand(unsigned(i));

        keystring = generate_random_keystring();
        key_obj = new Key(keystring);

        Decrypt *decrypt_obj = new Decrypt();
        decrypt_obj->begin_decryption(file_obj, key_obj);

        if (compare_known ==
            decrypt_obj->decrypted_plaintext.substr(0, 10))
        {
            file_obj->print_to_file(file_obj->output_filename, \
                                    keystring);
            break;
        }
        else
        {
            count++;
        }

        delete (decrypt_obj);
        delete (key_obj);
    }

}   //end Attack::begin_attack_with_seed_key()

