#ifndef HANDYFILE_H
#define HANDYFILE_H

#include <fstream>
#include <string>

using namespace std;

class HandyFile
{
public:
    HandyFile();
    void print_to_file(char *output_filename, string print_string);
    void check_file_status(fstream &file, char *filename);
    char *remove_hidden_end_characters(char *file_name);
	string get_contents_from_file(char* filename);

    char *input_filename;
    char *key_filename;
    char *output_filename;
	char *known_filename;

};  //HandyFile

#endif  //HANDYFILE_H
