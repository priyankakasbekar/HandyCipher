/*======================================================================*/
/* Filename: HandyFile.cpp                                              */
/*----------------------------------------------------------------------*/
/* Handles all file operations and stores all filenames used in         */
/* HandyCipher.cpp functions.                                           */
/*======================================================================*/

#include "HandyFile.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;


/*======================================================================*/
/*                       HandyFile::HandyFile()                         */
/*======================================================================*/
/* Initializes all filenames to NULL.                                   */
/*======================================================================*/
HandyFile::HandyFile()
{
    input_filename  = '\0';
    key_filename    = '\0';
    output_filename = '\0';
    known_filename  = '\0';

}   //end HandyFile::HandyFile()


/*======================================================================*/
/*                   HandyFile::print_to_file()                         */
/*======================================================================*/
/* Prints print_string to output file. Truncates the file if the file   */
/* already exists.                                                      */
/*======================================================================*/
void HandyFile::print_to_file(char *output_filename, string print_string)
{
    fstream output_file;

    output_file.open(output_filename, fstream::out | fstream::trunc);
    check_file_status(output_file, output_filename);
    output_file << print_string;
    output_file.close();

}   //end HandyFile::print_to_file()
 

/*======================================================================*/
/*                  HandyFile::check_file_status()                      */
/*======================================================================*/
/* Checks whether the file was able to be opened. If not, prints an     */
/* error message.                                                       */
/*======================================================================*/
void HandyFile::check_file_status(fstream &file, char *filename)
{
    if (file.fail())
    {
        cout << "Failed to access/read file ";
        cout << "'" << filename << "'." << endl;
        cout << "Exiting ...";
        cout << endl << endl;
        exit(-1);
    }

}   //end HandyFile::check_file_status()


/*======================================================================*/
/*                       get_contents_from_file()                       */
/*======================================================================*/
/* Opens file and returns the concatenated contents as a string.        */
/*======================================================================*/
string HandyFile::get_contents_from_file(char *filename)
{
    string contents = "";
    string line     = "";
    fstream input_file;

    filename = remove_hidden_end_characters(filename);

    input_file.open(filename, fstream::in);
    check_file_status(input_file, filename);

    //continues to append line from input_file to contents string
    //until reaches end of file
    while (getline(input_file, line))
    {
        contents = contents + line;
    }

    input_file.close();

    //remove temporary file
    remove(filename);

    return (contents);

}   //end HandyFile::get_contents_from_file()


/*======================================================================*/
/*               HandyFile::remove_hidden_end_characters()              */
/*======================================================================*/
/* Removes \r\n (^M) characters from files imported from or             */
/* edited with Windows programs. These hidden characters can appear on  */
/* Unix-based machines and cause issues with reading the file. In the   */
/* case that write_input_file is of shorter length than read_input_file */
/* (in which case, the remnants of read_input_file would still be at    */
/* the end of the new contents of write_input_file, the program first   */
/* writes to a temporary file. So to not overwrite the original file,   */
/* this new_filename is returned and used as the filename. Once the     */
/* temporary file is no longer needed, it is removed.                   */
/*======================================================================*/
char* HandyFile::remove_hidden_end_characters(char *filename)
{
    fstream read_input_file;
    fstream write_input_file;

	char *new_filename = (char*)malloc(500); 

    string read_line  = ""; 
    string write_line = "";

    sprintf(new_filename, "tmp_file.txt");

    read_input_file.open(filename, fstream::in);
    check_file_status(read_input_file, filename);

    write_input_file.open(new_filename, fstream::out | fstream::trunc);
    check_file_status(write_input_file, new_filename);

	while (getline(read_input_file, read_line))
	{
		int len;

		write_line = read_line;   //to preserve lines from input file
		len = write_line.length();

		while (len > 0)
		{
			if ((!write_line.empty() && write_line[len-1] == '\r'))
			{
				write_line.erase(len-1);
				len--;
			}
			else
			{
				break;
			}
		}
		//print to file
		write_input_file << write_line << endl;
	}

    read_input_file.close(); 
    write_input_file.close();

    return (new_filename);

}   //end HandyFile::remove_hidden_end_characters()

