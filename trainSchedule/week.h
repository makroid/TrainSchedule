#ifndef WEEK_H
#define WEEK_H

#include <QString>
#include <QXmlStreamReader>

class Week
{
public:
    Week();
    Week(int week, int year);
    Week(QXmlStreamReader&);
    Week(const Week&);

    Week& operator= (const Week&);

    int week() const { return iweek; }
    int year() const { return iyear; }
    void setWeek(int aweek) { iweek = aweek; }
    void setYear(int ayear) { iyear = ayear; }

    static const int daysPerWeek = 7;

    QString toString() const;

private:
    int iweek;
    int iyear;
};

// ordering is reverse!
// so latest (biggest) week comes up at the top
// this is more comfortable when viewing weeks in widget
inline bool operator<(const Week &w1, const Week &w2) {
    if (w1.year() > w2.year()) return true;
    if (w1.year() == w2.year()) {
        if (w1.week() > w2.week()) { return true; }
    }
    return false;
}

#endif // WEEK_H
