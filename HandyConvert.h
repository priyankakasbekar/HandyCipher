#ifndef HANDYCONVERT_H
#define HANDYCONVERT_H

#include <string>

using namespace std;

class HandyConvert
{
public:
    HandyConvert();

    string int_to_binary_string(int decimal_number);
    string int_to_string(int number);
    int string_to_int(string number_string);

};  //HandyConvert

#endif  //HANDYCONVERT_H
