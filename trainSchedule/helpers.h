#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include <QStringList>
#include <QPointF>
#include <QXmlStreamReader>

namespace Helper {

    inline QString pointToQString(const QPointF& p) {
        return QString(QString::number(p.x(), 'f', 8) + "," + QString::number(p.y(), 'f', 8));
    }

    inline QPointF stringToPoint(const QString& str) {
        QStringList sl = str.split(",");
        return QPointF(sl[0].toDouble(), sl[1].toDouble());
    }

    inline void skipUnknownElements(QXmlStreamReader& xmlReader) {
        xmlReader.readNext();
        while(!xmlReader.atEnd()) {
            if (xmlReader.isEndElement()) {
                xmlReader.readNext();
                break;
            }
            if (xmlReader.isStartElement()) {
                skipUnknownElements(xmlReader);
            } else {
                xmlReader.readNext();
            }
        }
    }
}

#endif // HELPERS_H
