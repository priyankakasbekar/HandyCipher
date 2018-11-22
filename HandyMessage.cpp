/*======================================================================*/
/* Filename: HandyMessage.cpp                                           */
/*----------------------------------------------------------------------*/
/* Contains all message-printing methods for HandyCipher.cpp.           */
/*======================================================================*/

#include "HandyMessage.h"
#include "HandyTime.h"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;


/*======================================================================*/
/*                     HandyMessage::HandyMessage()                     */
/*======================================================================*/
/* Prints header for handycipher.                                       */
/*======================================================================*/
HandyMessage::HandyMessage()
{
    cout << endl;
    cout << "==========================";
    cout << " HANDYCIPHER ";
    cout << "==========================";
    cout << endl << endl;

}   //end HandyMessage::HandyMessage()


/*======================================================================*/
/*                HandyMessage::print_help_message()                    */
/*======================================================================*/
/* Prints help message containing the program's usage and command       */
/* option information.                                                  */
/*======================================================================*/
void HandyMessage::print_help_message(void)
{

    cout << "Usage: ./handycipher [-h] [-e FILE] [-d FILE] [-k FILE]";
    cout << endl;
    cout << "                     [-a FILE] [-p FILE] [-o FILE]" << endl;
    cout << endl;

    cout << "Options:" << endl;
    cout << "    -h             : Shows this help message and exits.";
    cout << endl;

    cout << "    -e FILE        : Reads and encrypts plaintext from";
    cout << " FILE." << endl;
    cout << "                     If this option is chosen, the user";
    cout << " must" << endl;
    cout << "                     also specify a k file using the -k";
    cout << " option." << endl;

    cout << "    -d FILE        : Reads and decrypts plaintext from";
    cout << " FILE." << endl;
    cout << "                     If this option is chosen, the user";
    cout << " must" << endl;
    cout << "                     also specify a k file using the -k";
    cout << " option." << endl;

    cout << "    -k FILE        : Reads key from FILE. Must be included";
    cout << endl;
    cout << "                     if -e or -d options are specified, -k";
    cout << endl;
    cout << "                     option must also be specified.";
    cout << endl;

    cout << "    -a FILE        : Attempts an attack on the ciphertext";
    cout << " FILE." << endl;
    cout << "                     If the attack is successful, the";
    cout << " working" << endl;
    cout << "                     key is the output." << endl;

    cout << "    -p FILE        : Reads given plaintext from FILE to";
    cout << " attempt" << endl;
    cout << "                     attack. If -a option is specified, -p";
    cout << endl;
    cout << "                     option must also be specified.";
    cout << endl;

    cout << "    -o FILE        : Writes plaintext or ciphertext to FILE";
    cout << endl;
    cout << "                     specified by user. WARNING: If the";
    cout << " output" << endl;
    cout << "                     file chosen already exists, it will be";
    cout << endl;
    cout << "                     overwritten.";
    cout << endl << endl;

    cout << "Examples to run handycipher:" << endl;
    cout << "    ./handycipher -h" << endl;
    cout << "    ./handycipher -e plaintext.txt -k key.txt" << endl;
    cout << "    ./handycipher -d ciphertext.txt -k key.txt";
    cout << " -o my_plain.txt";

    cout << endl;
    cout << "    ./handycipher -a ciphertext.txt -p known_plaintext.txt";
    cout << endl;
    cout << endl;

    cout << "Default output saved to ./handycipher_output.txt.";

    cout << endl << endl;
    cout << "----------------------------------";
    cout << "-----------------------------";

    cout << endl << endl;

}   //end HandyMessage::print_help_message()

 
/*======================================================================*/
/*                HandyMessage::print_option_selected()                 */
/*======================================================================*/
/* Prints beginning message after option has been chosen. Indicates     */
/* which option has been chosen and tells that the program is now       */
/* beginning to execute that option.                                    */
/*======================================================================*/
void HandyMessage::print_option_selected(string option,
                                         char *input_filename)
{
    cout << option << " selected!" << endl;

    transform(option.begin(), option.end(), option.begin(), ::tolower);

    cout << "Commencing " << option << " of ";
    cout << input_filename << " ..." << endl << endl;
 
}   //end HandyMessage::print_option_selected()


/*======================================================================*/
/*                   HandyMessage::print_no_option()                    */
/*======================================================================*/
/* If no option is selected, prints this message.                       */
/*======================================================================*/
void HandyMessage::print_no_option(void)
{
    print_help_message();

    cout << "No option to encrypt, decrypt, or attack a file";
    cout << endl;
    cout << "has been chosen. Please refer to the above help";
    cout << endl;
    cout << "message for handycipher usage.";
    cout << endl << endl;
    cout << "Thank you! Exiting program." << endl;
    cout << "=======================================================";
    cout << "=========";
    cout << endl << endl;

}   //end HandyMessage::print_no_option()


/*======================================================================*/
/*                   HandyMessage::end_handycipher()                    */
/*======================================================================*/
/* Now that all output has been completed, prints end message.          */
/*======================================================================*/
void HandyMessage::end_handycipher(HandyTime *time_obj,
                                   char *output_filename)
{
    cout << endl;
    cout << "Finished output to file " << output_filename;
    cout << "." << endl << endl;

    cout << "Start: " << time_obj->start_date;
    cout << "End:   " << time_obj->get_end_date();
    cout << "TOTAL CPU TIME = " << time_obj->get_CPU_time_string();
    cout << endl << endl;

    cout << "Thank you! Exiting program." << endl;
    cout << "=======================================================";
    cout << "=========";
    cout << endl << endl;

}   //end end_handycipher()

