/*======================================================================*/
/* Filename: HandyCipher.cpp                                            */
/*----------------------------------------------------------------------*/
/* Encrypts, decrypts, or attacks a given file using the handycipher    */
/* algorithm.                                                           */
/*======================================================================*/

//#include "stdafx.h"
#include "Attack.h"
#include "Decrypt.h"
#include "Encrypt.h"
#include "HandyFile.h"
#include "HandyMessage.h"
#include "HandyOption.h"
#include "HandyTime.h"
#include "Key.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;


/*======================================================================*/
/*                             main()                                   */
/*======================================================================*/
/* Calls methods based on the user's commmand-line input.               */
/*======================================================================*/
int main(int argc, char *argv[])
{
    srand(time(NULL));

    HandyFile    *file_obj  = new HandyFile();
    HandyMessage *msg_obj   = new HandyMessage();
    HandyOption  *handy_obj = new HandyOption();
    HandyTime    *time_obj  = new HandyTime();

    handy_obj->parse_command_line(argc, argv, file_obj, msg_obj);

    //execute selected option
    if (handy_obj->chosen_option == 0)
    {
        //encryption selected
        msg_obj->print_option_selected("Encryption", \
                                       file_obj->input_filename);
        string key = \
            file_obj->get_contents_from_file(file_obj->key_filename);

        Key *key_obj = new Key(key);
        Encrypt *encrypt_obj = new Encrypt();

        encrypt_obj->begin_encryption(file_obj, key_obj);
    }
    else if (handy_obj->chosen_option == 1)
    {
        //decryption selected
        msg_obj->print_option_selected("Decryption", \
                                       file_obj->input_filename);
        string key = \
            file_obj->get_contents_from_file(file_obj->key_filename);

        Key *key_obj = new Key(key);
        Decrypt *decrypt_obj = new Decrypt();

        decrypt_obj->begin_decryption(file_obj, key_obj);
    }
    else if (handy_obj->chosen_option == 2)
    {
        //attack selected
        msg_obj->print_option_selected("Attack", \
                                       file_obj->input_filename);
        Attack * attack_obj = new Attack();
        attack_obj->begin_attack(file_obj);
        //attack_obj->begin_attack_with_seed_key(file_obj,   \
                                                 1418601600, \
                                                 1420070400);
    }
    else
    {
        //no option selected
        msg_obj->print_no_option();
    }

    msg_obj->end_handycipher(time_obj, file_obj->output_filename);

}   //end main()

