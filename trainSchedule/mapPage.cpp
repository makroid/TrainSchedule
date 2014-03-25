#include "mapPage.h"
#include "configData.h"
#include "helpers.h"
#include "kmlParameters.h"

#include <QtDebug>

MapPage::MapPage(QWidget *parent) : QWebPage(parent) {
}

//QString MapPage::userAgentForUrl(const QUrl&) const {
//    return "Chrome/1.0";
//}

bool MapPage::evalIsCurRouteNull() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("isCurRouteNull();");
    qDebug() << "evalIsCurRouteNull.toString()=" << result.toString();
    return result.toBool();
}

void MapPage::evalUndo() const {
    this->mainFrame()->evaluateJavaScript("curRoute.undoStep();");
}

int MapPage::evalStartNewRoute() const {
    QVariant id = this->mainFrame()->evaluateJavaScript("startNewSingleRoute();");
    return id.toInt();
}

bool MapPage::evalSwitchToRoute(int aid) const {
    QVariant result = this->mainFrame()->evaluateJavaScript("switchToRoute(" + QString::number(aid) + ");");
    return result.toBool();
}

bool MapPage::evalDeleteCurRoute() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("deleteCurRoute();");
    return result.toBool();
}

void MapPage::evalSetCenter() const {
    QString cmd = "setCenter(" + QString::number(ConfigData::instance().getStartLocation().x(), 'f', 7) + "," + QString::number(ConfigData::instance().getStartLocation().y(), 'f', 7) + ");";
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalMoveToCenter() const {
    QString cmd = "setCenter(" + QString::number(ConfigData::instance().getStartLocation().x(), 'f', 7) + "," + QString::number(ConfigData::instance().getStartLocation().y(), 'f', 7) + ");";
    cmd += "moveToCenter();";
    qDebug() << "evalMoveToCenter():" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

// TODO: implement this
QString MapPage::evalExportAsKml(const KmlParameters& kml) const {
    QString cmd = QString("generateKmlCode(%1,%2,%3);").arg(kml.durationInSec).arg(kml.altitudeInMeter).arg(kml.rangeInMeter);
    qDebug() << "evalExportAsKml()=" << cmd;
    QVariant kmlCode = this->mainFrame()->evaluateJavaScript(cmd);

    return kmlCode.toString();
}

void MapPage::evalGeocodeAddress(const QString &address) const {
    QString cmd = QString("geocodeLocationForAddress('%1');").arg(address);
    qDebug() << "evalGeocodeAddress=" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalCreateAndAddRouteMarker(int posX, int posY, const QString& markerText) const {
    QString cmd = QString("curRoute.createAndAddRouteMarker(%1,%2,'%3');").arg(posX).arg(posY).arg(markerText);
    qDebug() << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalJoinWithCurRoute(const MapRoute &amapRoute) const {
    QString cmd = "var poly=[];";
    for (int i=0; i<amapRoute.getPolyline().length(); i++) {
        cmd += "poly.push(" + LatLng(amapRoute.getPolyline().at(i)) + ");";
    }
    cmd += QString("var len=%1;").arg(amapRoute.getDistanceInMeter());
    cmd += "curRoute.joinWithCurRoute(poly,len);";
   // qDebug() << "evalJoinWithCurRoute=" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

/////////////////////////////////////////////////////////////////////////////////////
// setters

void MapPage::evalSetCurPos(QPointF acurPos) const {
    QString cmd = "curRoute.setCurPos(" + LatLng(acurPos) + ");";
    qDebug() << "evalSetCurPos()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetPrevPos(QPointF aprevPos) const {
    QString cmd = "curRoute.setPrevPos(" + LatLng(aprevPos) + ");";
    qDebug() << "evalSetPrevPos()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetDistanceInMeter(double adistanceInMeter)  const {
    QString cmd = QString("curRoute.setDistance(%1);").arg(adistanceInMeter);
    qDebug() << "evalSetDistance()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetName(QString aname) const {
    QString cmd = "curRoute.setName(" + aname + ");";
    qDebug() << "evalSetName()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetPolylineAndFitBounds(const QList<QPointF> &apoly) const {
    QString cmd = "var poly=[];";
    for (int i=0; i<apoly.length(); i++) {
        cmd += "poly.push(" + LatLng(apoly[i]) + ");";
    }
    cmd += "curRoute.setPolylineAndFitBounds(poly);";
    // qDebug() << "evalSetPolylineAndFitBounds()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetHistorySteps(const QList<int> &asteps) const {
    QString cmd = "var poly=[];";
    for (int i=0; i<asteps.length(); i++) {
        cmd += QString("poly.push(%1);").arg(asteps[i]);
    }
    cmd += "curRoute.setHistorySteps(poly);";
    // qDebug() << "evalSetHistorySteps()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetHistoryDists(const QList<double> &adists) const {
    QString cmd = "var dists=[];";
    for (int i=0; i<adists.length(); i++) {
        cmd += QString("dists.push(%1);").arg(adists[i]);
    }
    cmd += "curRoute.setHistoryDists(dists);";
    // qDebug() << "evalSetHistoryDists()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetId(int aid) const {
    QString cmd = QString("curRoute.setId(%1);").arg(aid);
    qDebug() << "evalSetId()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetModus(MapRoute::clickmodus amodus) const {
    QString cmd = "curRoute.setModus(" + QString::number(amodus) + ");";
    qDebug() << "evalSetModus()" << cmd;
    this->mainFrame()->evaluateJavaScript(cmd);
}

void MapPage::evalSetDistanceMarkers() const {

    QString cmd = "curRoute.updateDistMarkers(curRoute.poly.getPath().getArray(), 0);";
    this->mainFrame()->evaluateJavaScript(cmd);
}

int MapPage::loadMapRoute(const MapRoute& amapRoute) const {
    qDebug() << "START loadMapRoute";
    int id = evalStartNewRoute();
    qDebug() << "evalStartNewRoute";
    evalSetName(amapRoute.getName());
    qDebug() << "getName";
    evalSetPolylineAndFitBounds(amapRoute.getPolyline());
    qDebug() << "getpolyline";
    evalSetHistorySteps(amapRoute.getHistorySteps());
    qDebug() << "historysteps";
    evalSetHistoryDists(amapRoute.getHistoryDists());
    qDebug() << "historydists";
    evalSetId(amapRoute.getMapId());
    qDebug() << "mapid";
    evalSetCurPos(amapRoute.getCurPos());
    qDebug() << "curPos";
    evalSetPrevPos(amapRoute.getPrevPos());
    qDebug() << "prevPos";
    evalSetModus(amapRoute.getModus());
    qDebug() << "modus";
    evalSetDistanceMarkers();
    evalSetDistanceInMeter(amapRoute.getDistanceInMeter());
    qDebug() << "setDistance";
    qDebug() << "END loadMapRoute";
    return id;
}

// if there is already an existing route => extend it by amapRoute
// otherwise just load amapRoute (extend it to empty route)
bool MapPage::extendMapRoute(const MapRoute &amapRoute) const {
    bool isCurRouteNull = evalIsCurRouteNull();
    bool startNewRoute = false;
    if (isCurRouteNull) {
        loadMapRoute(amapRoute);
        startNewRoute = true;
    } else {
        evalJoinWithCurRoute(amapRoute);
    }
    return startNewRoute;
}

/////////////////////////////////////////////////////////////////////////////////////
// getters

QString MapPage::evalGetName() const {
    QVariant name = this->mainFrame()->evaluateJavaScript("curRoute.getName();");
    return name.toString();
}

int MapPage::evalGetId() const {
    QVariant id = this->mainFrame()->evaluateJavaScript("curRoute.getId();");
    return id.toInt();
}

// return in meter
double MapPage::evalGetDistanceInMeter() const {
    QVariant distance = this->mainFrame()->evaluateJavaScript("curRoute.getDistance();");
    return distance.toDouble();
}

QPointF MapPage::evalGetCurPos() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("curRoute.getCurPos();");
    QPointF curPos;
    variantToPointF(result, curPos);
    return curPos;
}

QPointF MapPage::evalGetPrevPos() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("curRoute.getPrevPos();");
    QPointF prevPos;
    variantToPointF(result, prevPos);
    return prevPos;
}

QList<QPointF> MapPage::evalGetPolyline() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("curRoute.getPolyline();");

    QList<QPointF> polyline;
    variantToPointFList(result, polyline);

    for (int i=0; i<polyline.length(); i++) {
         qDebug() << "polyline[" << i << "]=" << polyline[i].x() << "," << polyline[i].y();
    }
    return polyline;
}

QList<int> MapPage::evalGetHistorySteps() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("curRoute.getHistorySteps();");
    QVariantList vpath = result.toList();

    QList<int> historySteps;
    for (int i=0; i<vpath.length(); i++) {
        historySteps.push_back(vpath[i].toInt());
    }
    return historySteps;
}

QList<double> MapPage::evalGetHistoryDists() const {
    QVariant result = this->mainFrame()->evaluateJavaScript("curRoute.getHistoryDists();");
    QVariantList vpath = result.toList();

    QList<double> historyDists;
    for (int i=0; i<vpath.length(); i++) {
        historyDists.push_back(vpath[i].toDouble());
    }
    return historyDists;
}

void MapPage::fillMapRoute(MapRoute &amapRoute) const {
    bool noRoute = evalIsCurRouteNull();
    if (noRoute) {
        qDebug() << "curRoute is null";
        return;
    }

    qDebug() << "START fillMapRoute";
    amapRoute.setMapId(evalGetId());
    qDebug() << "setMapId";
    amapRoute.setCurPos(evalGetCurPos());
    qDebug() << "setCurPos";
    amapRoute.setPrevPos(evalGetPrevPos());
    qDebug() << "setPrevPos";
    amapRoute.setModus(MapRoute::runnerAutomatic);
    qDebug() << "setModus";
    amapRoute.setPolyline(evalGetPolyline());
    qDebug() << "setPolyline";
    amapRoute.setDistanceInMeter(evalGetDistanceInMeter());
    qDebug() << "setDistance";
    amapRoute.setHistorySteps(evalGetHistorySteps());
    qDebug() << "setHistorySteps";
    amapRoute.setHistoryDists(evalGetHistoryDists());
    qDebug() << "setHistoryDists";
    amapRoute.setName(evalGetName());
    qDebug() << "setName";
    qDebug() << "END fillMapRoute";
}



QString MapPage::LatLng(const QPointF &apoint) const {
    // increase precision for apoint, because default values are too low!
    return QString("new google.maps.LatLng(%1,%2)").arg(apoint.x(), 0, 'f', 8).arg(apoint.y(), 0, 'f', 8);
}

void MapPage::variantToPointF(const QVariant &aval, QPointF& p) const {
    QVariantList vlist = aval.toList();
    p.setX(0);
    p.setY(0);
    if (vlist.size() == 2) {
        p.setX(vlist[0].toDouble());
        p.setY(vlist[1].toDouble());
    }
}

void MapPage::variantToPointFList(const QVariant &aval, QList<QPointF> &pl) const {
    QVariantList vlist = aval.toList();
    for (int i=0; i<vlist.size(); i+=2) {
        pl.push_back(QPointF(vlist[i].toDouble(), vlist[i+1].toDouble()));
    }
}
