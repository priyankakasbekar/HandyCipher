/*======================================================================*/
/* Filename: HandyConvert.cpp                                           */
/*----------------------------------------------------------------------*/
/* Contains methods to convert ints to strings or vice versa.           */
/*======================================================================*/

#include "HandyConvert.h"

#include <algorithm>
#include <sstream>

using namespace std;


/*======================================================================*/
/*                     HandyConvert::HandyConvert()                     */
/*======================================================================*/
/* HandyConvert constructor.                                            */
/*======================================================================*/
HandyConvert::HandyConvert()
{

}   //end HandyConvert::HandyConvert()


/*======================================================================*/
/*                 HandyConvert::int_to_binary_string()                 */
/*======================================================================*/
/* Converts decimal int to a string representing its binary equivalent  */
/* in 5 digits. For example, if decimal_number = 4, this function would */
/* return the string "00100".                                           */
/*======================================================================*/
string HandyConvert::int_to_binary_string(int decimal_number)
{
    string binary_string = "";

    if (decimal_number != 0)
    {
        //must be long for shifting more than 32 bits
        long decimal_long = decimal_number;
        while (decimal_long)
        {
            if ((decimal_long & 1) == 0)
            {
                binary_string = binary_string + "0";
            }
            else
            {
                binary_string = binary_string + "1";
            }
            decimal_long = decimal_long >> 1L;
        }
        reverse(binary_string.begin(), binary_string.end());
    }
    else
    {
        binary_string = "0";
    }

    //make the binary string 5 digits long
    //to use in matrix traversal
    if (binary_string.length() < 5)
    {
        int length = binary_string.length();

        for (int i = 0; i < (5-length); i++)
        {
            binary_string = "0" + binary_string;
        }
    }

    return (binary_string); 

}   //end HandyConvert::int_to_binary_string()


/*======================================================================*/
/*                     HandyConvert::int_to_string()                    */
/*======================================================================*/
/* Converts an input int to a string. Example: 5 to "5".                */
/*======================================================================*/
string HandyConvert::int_to_string(int number)
{
    ostringstream number_string;

    number_string << number; 

    return (number_string.str());

}   //end HandyConvert::int_to_string()


/*======================================================================*/
/*                   HandyConvert::string_to_int()                      */
/*======================================================================*/
/* Converts an input string to an int. Example: "5" to 5.               */
/*======================================================================*/
int HandyConvert::string_to_int(string number_string)
{
    int number_int = 0;

    istringstream buffer(number_string);

    buffer >> number_int;

    return (number_int);

}   //end HandyConvert::string_to_int()

