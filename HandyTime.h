#ifndef HANDYTIME_H
#define HANDYTIME_H

#include <ctime>

using namespace std;

class HandyTime
{
public:
    HandyTime();

    long  get_CPU_time(void);
    char *get_CPU_time_string(void);

    char *get_end_date(void);
    char *start_date;

private:
    long begin_time;

};  //HandyTime

#endif  //HANDYTIME_H
