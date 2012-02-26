#ifndef WEEKCHANGE_H
#define WEEKCHANGE_H

#include "week.h"

class WeekChange
{
public:
    WeekChange(Week w, int a);

    Week week;
    int action;

    static const int WeekInserted = 1;
    static const int WeekRemoved = 2;
};

#endif // WEEKCHANGE_H
