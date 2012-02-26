#include "configData.h"
#include "helpers.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QTextStream>

ConfigData::ConfigData()
    : startLocation(QPointF(48.1414805, 11.560707)),
      configFileName("schedule.config"),
      compressSdlFile(true)
{ }

bool ConfigData::readXML() {
    QFile file(configFileName);
    if ( ! file.open(QFile::ReadOnly | QFile::Text)) {
        // TODO: warn
        qDebug() << "no config file found!";
        return false;
    }

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    qDebug() << "first element" << xmlReader.name();
    while( ! xmlReader.atEnd()) {
        if (xmlReader.isStartElement()) {
            qDebug() << "isStartElement();";
            if (xmlReader.name() == "startlat") {
                qDebug() << "startlat";
                double lat = xmlReader.readElementText().toDouble();
                startLocation.setX(lat);
                xmlReader.readNext();
            } else if (xmlReader.name() == "startlng")             {
                qDebug() << "startlng";
                double lng = xmlReader.readElementText().toDouble();
                startLocation.setY(lng);
                xmlReader.readNext();
            } else if (xmlReader.name() == "schedulefile") {
                qDebug() << "schedulefile";
                scheduleFileName = xmlReader.readElementText();
                xmlReader.readNext();
            } else if (xmlReader.name() == "scheduleconfig") {
                xmlReader.readNext();
            } else if (xmlReader.name() == "compresssdl") {
                int b = xmlReader.readElementText().toInt();
                if (b == 1) {
                    compressSdlFile = true;
                } else {
                    compressSdlFile = false;
                }
                xmlReader.readNext();
            } else {
                qDebug() << "skipUnknownElements()";
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            qDebug() << "readNext()";
            xmlReader.readNext();
        }
    }
    file.close();
    return true;
}

bool ConfigData::writeXML() {
    QFile file(configFileName);

    if ( ! file.open(QFile::WriteOnly | QFile::Text)) {
        // TODO: warn
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("scheduleconfig");
    xmlWriter.writeTextElement("startlat", QString::number(startLocation.x(), 'f', 8));
    xmlWriter.writeTextElement("startlng", QString::number(startLocation.y(), 'f', 8));
    xmlWriter.writeTextElement("schedulefile", scheduleFileName);
    xmlWriter.writeTextElement("compresssdl", QString::number(compressSdlFile));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
    return true;
}
