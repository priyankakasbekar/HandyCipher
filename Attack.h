#ifndef ATTACK_H
#define ATTACK_H

#include "HandyFile.h"
#include <map>
#include <string>
#include <time.h>

using namespace std;

class Attack
{
public:
	void begin_attack(HandyFile *file_obj);
	void begin_attack_using_seed_key(HandyFile *file_obj,    \
                                          time_t start_time, \
                                          time_t end_time);
	
private:
    string generate_random_keystring(void);

    map<string, long long> attempted_keys_lookup;

};  //Attack

#endif  //ATTACK_H
