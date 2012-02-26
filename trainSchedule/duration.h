#ifndef DURATION_H
#define DURATION_H

#include <QString>

class QXmlStreamWriter;
class QXmlStreamReader;

class Duration
{
public:
    Duration();
    Duration(int inSecs);
    Duration(int hour, int min, int sec);
    Duration(QXmlStreamReader&);

    Duration(const Duration&);
    ~Duration() {}

    void setDuration(int ahour, int amin, int asec);
    Duration getDuration() {
        return *this;
    }
    void setHour(int ahour) {hour = ahour; }
    void setMin(int amin) { min = amin; }
    void setSec(int asec) { sec = asec; }
    void setZero() { hour=0; min=0; sec=0; }

    int getHour() const { return hour; }
    int getMin() const { return min; }
    int getSec() const { return sec; }
    int getDurationInSecs() const { return 3600*hour+60*min+sec; }

    void toMin();
    void toSec();

    void toSixty();
    QString toString(const QString& format = "") const;

    Duration operator+(const Duration&) const;
    Duration& operator+=(const Duration&);
    Duration operator-(const Duration&) const;
    Duration& operator-=(const Duration&);
    Duration& operator=(const Duration&);

    void writeXml(QXmlStreamWriter& writer) const;
private:
    int hour;
    int min;
    int sec;
};

#endif // DURATION_H
