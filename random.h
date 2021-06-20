#ifndef RANDOM_H
#define RANDOM_H
#endif

#include <time.h>

//Set Seed for rand() Function
//put it in main()
// srand(time(0));
int Random(int first, int last)
{
    int r;
    last += (1-first);
    r = rand()%last + first;
    return r;
}