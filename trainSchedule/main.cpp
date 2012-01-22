#include <iostream>

#include <QApplication>
#include <QDebug>

#include "trainScheduleWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TrainScheduleWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
