#ifndef DISTANCE_H
#define DISTANCE_H

#include <QString>

class QXmlStreamWriter;
class QXmlStreamReader;

class Distance
{
public:
    Distance();
    explicit Distance(double d);
    Distance(QXmlStreamReader&);
    Distance(const Distance&);
    ~Distance() {}

    enum Unit {METER, KM, MILE};

    double dist;
    Unit unit;

    void kmToMile();
    void mileToKm();
    void kmToMeter();
    void mileToMeter();

    static QString unitToQString(Unit u);
    QString toTestString() const;
    QString toString() const;
    QString getStrUnit() const { return unitToQString(unit); }
    void writeXml(QXmlStreamWriter& writer) const;

    Distance& operator+= (const Distance& other) {
        dist += other.dist;
        return *this;
    }

    Distance& operator-= (const Distance& other) {
        dist -= other.dist;
        return *this;
    }
};



#endif // DISTANCE_H
