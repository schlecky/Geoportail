// =====================================================================================
// 
//       Filename:  mainwindow.cpp
// 
//    Description:  Telechargement d'images de géoportail
// 
//        Version:  1.0
//        Created:  26.08.2009 23:12:32
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Renaud Schleck (), 
//        Company:  
// 
// =====================================================================================
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>

#include "geodl.h"


GeoDL::GeoDL()
{
    ui.setupUi(this);
   // mapWidget = new MapWidget;
   // ui.vLayout->addWidget(mapWidget,2);

}

GeoDL::~GeoDL()
{

}

void GeoDL::on_dlBtn_clicked(bool chk)
{

}



int main(int argc, char* argv[])
{
    QApplication app(argc,argv);
    GeoDL geodl;
    geodl.show();
    return app.exec();
}



void GeoDL::on_radioCarte_toggled(bool checked)
{
    if(checked)
        ui.mapWidget->setCouche(CARTE_IGN);
}

void GeoDL::on_radioPhotos_toggled(bool checked)
{
    if(checked)
        ui.mapWidget->setCouche(PHOTOS);
}

void GeoDL::on_radioButton_toggled(bool checked)
{
    if(checked)
        ui.mapWidget->setCouche(TEST);
}
