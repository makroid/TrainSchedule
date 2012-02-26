#ifndef RUNROUTE_H
#define RUNROUTE_H

#include <QString>
#include <QPointF>
#include <QList>

class QXmlStreamWriter;
class QXmlStreamReader;


class MapRoute {
public:
    MapRoute();
    ~MapRoute();

    typedef enum {runnerAutomatic=1, bikerAutomatic=2, manual=3} clickmodus;

    // getters
    QPointF getCurPos() const;
    QPointF getPrevPos() const;
    QString getName() const;
    int getMapId() const;
    clickmodus getModus() const;
    double getDistanceInMeter() const; // return in meter
    const QList<int>& getHistorySteps() const;
    const QList<double>& getHistoryDists() const;
    const QList<QPointF>& getPolyline() const;
    QPointF getStartPos() const;

    // setters
    void setCurPos(QPointF curPos);
    void setPrevPos(QPointF prevPos);
    void setName(QString name);
    void setMapId(int id);
    void setModus(clickmodus modus);
    void setDistanceInMeter(double distanceInMeter);
    void setHistorySteps(const QList<int>& historySteps);
    void setHistoryDists(const QList<double>& distances);
    void setPolyline(const QList<QPointF>& polyline);

    QString toString() const;
    void readXML(QXmlStreamReader&);
    void writeXml(QXmlStreamWriter& writer) const;

private:
    void readHistoryStepsXML(QXmlStreamReader&);
    void readDistancesXML(QXmlStreamReader&);
    void readPolylineXML(QXmlStreamReader&);

    QPointF curPos;
    QPointF prevPos;
    QString name;
    int mapId;
    double distanceInMeter;
    clickmodus modus;
    QList<int> historySteps;
    QList<double> distances;
    QList<QPointF> polyline;
};

#endif // RUNROUTE_H
