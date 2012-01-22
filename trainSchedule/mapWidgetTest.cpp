//#include "mapWidgetTest.h"
//#include <QtDebug>

//MapWidgetTest::MapWidgetTest(QWidget* parent) : QWidget(parent)
//{
//    mapWidget = new MapWidget(this);
//    mapWidget->load(QUrl("/home/armin/source/maptest/mapsTest.html"));

//    textEdit = new QTextEdit(this);
//    testButton = new QPushButton("test", this);

//    mapControlPanel = new MapControlPanel(mapWidget);

//    bottomLayout = new QHBoxLayout;
//    bottomLayout->addWidget(textEdit);
//    bottomLayout->addWidget(testButton);

//    rightLayout = new QVBoxLayout;
//    rightLayout->addWidget(mapWidget);
//    rightLayout->addLayout(bottomLayout);

//    mainLayout = new QHBoxLayout;
//    mainLayout->addWidget(mapControlPanel);
//    mainLayout->addLayout(rightLayout);

//    setLayout(mainLayout);

//    connect(testButton, SIGNAL(clicked()), this, SLOT(updateTextEdit()));
//}

//void MapWidgetTest::updateTextEdit() {
//    double d = mapWidget->getMapPage()->evalGetDistanceInMeter();
//    textEdit->append(tr("distance=%1m").arg(d));
//}

////void MapWidgetTest::setAutomaticMode(bool isAutomatic) {
////    if (isAutomatic) {
////     //   qDebug() << "setAutomaticModus(true)";
////        mapWidget->getMapPage()->evalSetModus(MapRoute::automatic);
////    }
////}

////void MapWidgetTest::setManualMode(bool isManual) {
////    if (isManual) {
////     //   qDebug() << "setAutomaticManual(true)";
////        mapWidget->getMapPage()->evalSetModus(MapRoute::manual);
////    }
////}
