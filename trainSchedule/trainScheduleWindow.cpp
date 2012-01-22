#include "trainScheduleWindow.h"
#include "weekMap.h"
#include "trainScheduleWidget.h"
#include "sessionPainter.h"
#include "defaultSessionPainter.h"
#include "createWeekDialog.h"
#include "configDialog.h"
#include "configData.h"

#include <QtGui>
#include <QSharedPointer>
#include <QFile>
#include <QtDebug>

TrainScheduleWindow::TrainScheduleWindow()
{
    spainter = new DefaultSessionPainter();
    weekMap = QSharedPointer<WeekMap>(new WeekMap());
    tsWidget = new TrainScheduleWidget(weekMap, spainter, this);
    weekMap->addWeekMapListener(tsWidget);

    createActions();
    createMenus();

    // read in schedule, if available
    ConfigData::instance().readXML();
    if ( ! ConfigData::instance().getScheduleFileName().isEmpty()) {
        weekMap->readXML(ConfigData::instance().getScheduleFileName());
        trainDataFile = ConfigData::instance().getScheduleFileName();
    }

    setCentralWidget(tsWidget);

    // check for map files
    QFile mapFile("mapsTest.html");
    QFile mapJsFile("mapsTest.js");
    if ( ! (mapFile.open(QFile::ReadOnly) && mapJsFile.open(QFile::ReadOnly))) {
        QMessageBox::critical(this, tr("Train Schedule"), tr("Map files not found!\n"
                                                             "(in application directory)"));

        //QCoreApplication::exit(1);
        exit(1);
    }
}

bool TrainScheduleWindow::okToContinue() {
    // TODO implement this
    if (isWindowModified() || tsWidget->isModified()) {
        int r = QMessageBox::warning(this, tr("Train Schedule"),
                                     tr("The schedule has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void TrainScheduleWindow::closeEvent(QCloseEvent *event) {
    if (okToContinue()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void TrainScheduleWindow::createActions() {
    newWeekAction = new QAction(tr("&New Week..."), this);
    newWeekAction->setShortcut(QKeySequence::New);
    connect(newWeekAction, SIGNAL(triggered()), this, SLOT(createNewWeek()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+x"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+s"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save As..."), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveTrainFileAs()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(tr("Ctrl+o"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    configAction = new QAction(tr("&Options..."), this);
    connect(configAction, SIGNAL(triggered()), this, SLOT(openConfigDialog()));
}

void TrainScheduleWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(newWeekAction);
    editMenu->addAction(configAction);
}

void TrainScheduleWindow::createNewWeek() {
    CreateWeekDialog cwDialog(this);
    if (cwDialog.exec()) {
        Week selectedWeek(cwDialog.getWeek(), cwDialog.getYear());
        bool success;
        if (cwDialog.doRemove()) {
            success = weekMap->removeWeek(selectedWeek);
        } else {
            success = weekMap->insertNewWeek(selectedWeek);
        }
        setWindowModified(success);
    }
}

bool TrainScheduleWindow::save() {
    if (trainDataFile.isEmpty()) {
        return saveTrainFileAs();
    } else {
        return saveTrainFile(trainDataFile);
    }
}

bool TrainScheduleWindow::saveTrainFile(const QString& fileName) {
    if ( ! weekMap->save(fileName)) {
        qDebug() << "save failed";
        return false;
    }
    setWindowModified(false);
    tsWidget->setModified(false);
    return true;
}

bool TrainScheduleWindow::saveTrainFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Training"), ".", tr("Training files (*.tr)"));
    if (fileName.isEmpty()) {
        return false;
    }
    return saveTrainFile(fileName);
}

void TrainScheduleWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Training Data"), ".", tr("Training files (*.tr)\n"
                                                                                            "All (*.*)"));
    if (!fileName.isEmpty()) {
        weekMap->readXML(fileName);
    }
}

void TrainScheduleWindow::openConfigDialog() {
    ConfigDialog configDialog(this);
    if (configDialog.exec()) {
        setWindowModified(configDialog.isModified());
        qDebug() << "configuration accepted";
    }
}
