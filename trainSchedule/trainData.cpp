#include "trainData.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>
#include <QtDebug>

#include "helpers.h"
#include "configData.h"


TrainingData::TrainingData()
    : weekMap(QSharedPointer<WeekMap>(new WeekMap())),
      myRoutesModel(QSharedPointer<MyRoutesModel>(new MyRoutesModel()))
{
}

bool TrainingData::save(const QString &filename) {
    bool doCompress = ConfigData::instance().doCompressSdlFile();
    QString result;
    QXmlStreamWriter xmlWriter(&result);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("traininglog");
    weekMap->writeXML(xmlWriter);
    myRoutesModel->writeXML(xmlWriter);
    xmlWriter.writeEndDocument();

    QFile file(filename);
    // write string to file
    if (doCompress) {
        qDebug() << "writing compressed file";
        if ( ! file.open(QFile::WriteOnly)) {
            // TODO warn
            qDebug() << "Warning: can not write" << filename;
            return false;
        }
        QByteArray ba = qCompress(result.toUtf8());
        QDataStream dataOut(&file);
        dataOut << ba;
        file.close();
    } else {
        qDebug() << "writing UNcompressed file";
        if ( ! file.open(QFile::WriteOnly)) {
            // TODO warn
            qDebug() << "Warning: can not write" << filename;
            return false;
        }
        QTextStream txtStream(&file);
        txtStream << result;       
    }
    file.close();
    return true;
}

bool TrainingData::open(const QString& filename) {
    QFile file(filename);
    if ( ! file.open(QFile::ReadOnly)) {
        // TODO: warn
        return false;
    }

    QString xmlString;

    if (ConfigData::instance().doCompressSdlFile()) {
        qDebug() << "reading from compressed file";
        QDataStream dataIn(&file);
        QByteArray compressedByteArray;
        dataIn >> compressedByteArray;
        QByteArray xmlByteArray = qUncompress(compressedByteArray);
        xmlString = QString::fromUtf8(xmlByteArray.data(), xmlByteArray.size());
    } else {
        qDebug() << "reading UNcompressed file";
        QTextStream txtIn(&file);
        xmlString = txtIn.readAll();
    }

    file.close();

    QXmlStreamReader xmlReader(xmlString);
    readXML(xmlReader);
    return true;
}

void TrainingData::readXML(QXmlStreamReader& xmlReader) {
    xmlReader.readNext();
    while( ! xmlReader.atEnd()) {
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "traininglog") {               
                xmlReader.readNext();
            } else if (xmlReader.name() == "weekmap") {
                weekMap->readXML(xmlReader);
            } else if (xmlReader.name() == "myroutes") {
                myRoutesModel->readXML(xmlReader);
            } else {
                Helper::skipUnknownElements(xmlReader);
            }
        } else {
            xmlReader.readNext();
        }
    }
}
