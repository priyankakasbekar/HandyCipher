/*======================================================================*/
/* Filename: HandyOption.cpp                                            */
/*----------------------------------------------------------------------*/
/* Parses command line and stores options/filenames accordingly.        */
/*======================================================================*/

#include "HandyFile.h"
#include "HandyOption.h"
#include "HandyMessage.h"

#include <cstring>
#include <iostream>
#include <stdlib.h>

using namespace std;


/*======================================================================*/
/*                      HandyOption::HandyOption()                      */
/*======================================================================*/
/* Initializes chosen_option as 3. If, at the end of parsing the        */
/* command line, chosen_option is still 3, the program knows that no    */
/* option has been chosen.                                              */
/*======================================================================*/
HandyOption::HandyOption()
{
    chosen_option = 3;

}   //end HandyOption::HandyOption()


/*======================================================================*/
/*                   HandyOption::parse_command_line()                  */
/*======================================================================*/
/* Parses command line and saves filenames accordingly.                 */
/*======================================================================*/
void HandyOption::parse_command_line(int argc, char *argv[],
                                     HandyFile    *file_obj,
                                     HandyMessage *msg_obj)
{

    if ((argc < 5) || (argc > 7))
    {
        msg_obj->print_help_message();
        exit(-1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            msg_obj->print_help_message();
            exit(0);
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            chosen_option = 0;
            file_obj->input_filename = strdup(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            chosen_option = 1;
            file_obj->input_filename = strdup(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-k") == 0)
        {
            file_obj->key_filename = strdup(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            chosen_option = 2;
            file_obj->input_filename = strdup(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            file_obj->known_filename = strdup(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            file_obj->output_filename = strdup(argv[i+1]);
            i++;
        }
    }

    //if -o option not selected,
    //sets default output file name
    if (strlen(file_obj->output_filename) == 0)
    {
        file_obj->output_filename = strdup("handycipher_output.txt");
    }

}   //end HandyOption::parse_command_line()

