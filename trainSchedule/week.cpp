#include "week.h"
#include <QStringList>
#include <QtDebug>

Week::Week() : iweek(0), iyear(0)
{}

Week::Week(int aweek, int ayear) : iweek(aweek), iyear(ayear)
{}

Week::Week(QXmlStreamReader &xmlReader) {
    QString text = xmlReader.readElementText();

    QStringList weekyear = text.split(":");
    if (weekyear.length() != 2) {
        // TODO: handle
    }
    int week = weekyear[0].toInt();
    int year = weekyear[1].toInt();
    iweek = week;
    iyear = year;
    xmlReader.readNext(); // </week>
}

Week::Week(const Week &aweek) : iweek(aweek.iweek), iyear(aweek.iyear)
{}

Week& Week::operator =(const Week& other) {
    if (this != &other) {
        iweek = other.week();
        iyear = other.year();
    }
    return *this;
}

QString Week::toString() const {
    return QString::number(iweek) + ":" + QString::number(iyear);
}
