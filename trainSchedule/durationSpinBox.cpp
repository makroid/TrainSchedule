#include <QRegExpValidator>
#include <QtDebug>

#include "durationSpinBox.h"

DurationSpinBox::DurationSpinBox(QWidget* parent) : QSpinBox(parent)
{
    timeFormat = QRegExp("^(\\d+):(\\d\\d):(\\d\\d)$");
    validator = new QRegExpValidator(timeFormat, this);

    // single step increments/decrements 1 min
    setSingleStep(60);
    setMaximum(3600*24);
    // set default 1h
    setValue(3600);
}

QValidator::State DurationSpinBox::validate(QString &ainput, int &apos) const {
    return validator->validate(ainput, apos);
}

QString DurationSpinBox::textFromValue(int avalue) const {
    int hour, min, sec;
    toSixty(avalue, hour, min, sec);

    QString text = "";
    if (hour < 10) {
        text.append("0");
    }
    text.append(QString::number(hour) + ":");
    if (min < 10) {
        text.append("0");
    }
    text.append(QString::number(min) + ":");
    if (sec < 10) {
        text.append("0");
    }
    text.append(QString::number(sec));

    return text;
}


int DurationSpinBox::valueFromText(const QString &text) const {
    if (timeFormat.exactMatch(text)) {
        timeFormat.indexIn(text);
        int hour = timeFormat.cap(1).toInt();
        int min = timeFormat.cap(2).toInt();
        int sec = timeFormat.cap(3).toInt();
        return 3600*hour + 60*min + sec;
    } else {
        return 0;
    }

}

void DurationSpinBox::toSixty(int durInSecs, int &h, int &m, int &s) const {
    int sec = durInSecs;
    int min = 0;
    int hour = 0;

    int minsInSecs = static_cast<int>(sec/60);
    min += minsInSecs;
    sec -= minsInSecs*60;
    int hoursInMins = static_cast<int>(min/60);
    hour += hoursInMins;
    min -= hoursInMins*60;

    h = hour; m = min; s = sec;
}
