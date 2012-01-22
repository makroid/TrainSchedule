#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QPointF>
#include <QString>

class ConfigData
{
public:
    static ConfigData& instance() {
        static ConfigData config;
        return config;
    }

    QPointF getStartLocation() { return instance().startLocation; }
    QString getScheduleFileName() { return instance().scheduleFileName; }
    bool doCompressSdlFile() { return instance().compressSdlFile; }

    void setStartLat(double lat) { instance().startLocation.setX(lat); }
    void setStartLng(double lng) { instance().startLocation.setY(lng); }
    void setScheduleFileName(QString fileName) { instance().scheduleFileName = fileName; }
    void setCompressSdlFile(bool b) { instance().compressSdlFile = b; }

    bool writeXML();
    bool readXML();

private:
    ConfigData();
    ~ConfigData() {}
    ConfigData(const ConfigData&);

    QPointF startLocation;
    QString scheduleFileName;
    const QString configFileName;
    bool compressSdlFile;
};

#endif // CONFIGDATA_H
