#ifndef MYROUTESLISTENER_H
#define MYROUTESLISTENER_H

class MyRoutesChange;

class MyRoutesListener {
public:
    virtual void changeInMyRoutes(MyRoutesChange) = 0;
};

#endif // MYROUTESLISTENER_H
