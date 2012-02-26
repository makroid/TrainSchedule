#ifndef WEEKMAPLISTENER_H
#define WEEKMAPLISTENER_H

class WeekChange;

class WeekMapListener {
public:
    virtual void changeInWeek(WeekChange) = 0;

};

#endif // WEEKMAPLISTENER_H
