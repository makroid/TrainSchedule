#ifndef TRAINDATA_H
#define TRAINDATA_H

#include <QSharedPointer>

#include "weekMap.h"
#include "myRoutesModel.h"

class QString;
class QXmlStreamReader;

class TrainingData
{
public:
    typedef QSharedPointer<WeekMap> WeekMapPtr;
    typedef QSharedPointer<MyRoutesModel> MyRoutesModelPtr;

    TrainingData();

    WeekMapPtr getWeekMap() const { return weekMap; }
    MyRoutesModelPtr getMyRoutesModel() const { return myRoutesModel; }

    bool save(const QString& filename);
    bool open(const QString& filename);

private:
    WeekMapPtr weekMap;
    MyRoutesModelPtr myRoutesModel;

    void readXML(QXmlStreamReader&);
};

#endif // TRAINDATA_H
