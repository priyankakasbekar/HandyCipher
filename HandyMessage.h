#ifndef HANDYMESSAGE_H
#define HANDYMESSAGE_H

#include "HandyTime.h"

#include <fstream>
#include <string>

using namespace std;

class HandyMessage
{
public:
    HandyMessage();

    void print_help_message();
    void print_option_selected(string option, \
                               char *input_filename);
    void print_no_option(void);
    void end_handycipher(HandyTime *time_obj, char *output_filename);

};  //HandyMessage

#endif  //HANDYMESSAGE_H
