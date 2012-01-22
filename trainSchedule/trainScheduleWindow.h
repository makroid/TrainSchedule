#ifndef TRAINSCHEDULE_H
#define TRAINSCHEDULE_H

#include <QMainWindow>
#include <QSharedPointer>

#include "weekMap.h"

class TrainScheduleWidget;
class SessionPainter;
class QAction;

class TrainScheduleWindow : public QMainWindow
{
    Q_OBJECT

public:
    TrainScheduleWindow();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void createNewWeek();
    void open();
    bool save();
    bool saveTrainFile(const QString&);
    bool saveTrainFileAs();
    void openConfigDialog();

private:
    void createActions();
    void createMenus();

    bool okToContinue();

    SessionPainter* spainter;
    QSharedPointer<WeekMap> weekMap;
    TrainScheduleWidget* tsWidget;

    QAction* newWeekAction;
    QAction* exitAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* openAction;
    QAction* configAction;

    QMenu* fileMenu;
    QMenu* editMenu;

    QString trainDataFile;
};

#endif // TRAINSCHEDULE_H
