#ifndef HANDYOPTION_H
#define HANDYOPTION_H

#include "HandyFile.h"
#include "HandyMessage.h"

class HandyOption
{
public:
    HandyOption();
    void parse_command_line(int argc, char *argv[], \
                            HandyFile    *file_obj, \
                            HandyMessage *msg_obj);

    int chosen_option;  //0 for encrypt, 1 for decrypt, 2 for attack

};  //HandyOption

#endif  //HANDYOPTION_H
