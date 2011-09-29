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
// =====================================================================================
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QFileDialog>

#include "geodl.h"


GeoDL::GeoDL()
{
    ui.setupUi(this);
    connect(ui.mapWidget,SIGNAL(setDLEnabled(bool)),ui.btnDownload,SLOT(setEnabled(bool)));
    //connect(mapWidget,SIGNAL(coordChange(double,double)),this,SLOT())
   // mapWidget = new MapWidget;
   // ui.vLayout->addWidget(mapWidget,2);
}

GeoDL::~GeoDL()
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
        ui.mapWidget->setCouche(CASSINI);
}

void GeoDL::on_radioButton_2_toggled(bool checked)
{
    if(checked)
        ui.mapWidget->setCouche(CADASTRE);
}

void GeoDL::on_btnLongLat_clicked()
{
    double longitude = ui.edtLongitude->text().toDouble();
    double latitude = ui.edtLatitude->text().toDouble();
    ui.mapWidget->goToLongLat(longitude, latitude);
}

void GeoDL::on_mapWidget_coordChange(double longit, double latit)
{
    ui.edtLongitude->setText(QString("%1").arg(longit));
    ui.edtLatitude->setText(QString("%1").arg(latit));
}

void GeoDL::on_mapWidget_mouseCoordChange(double longit, double latit)
{
    ui.statusbar->showMessage(QString("%1 , %2").arg(longit).arg(latit));
}

void GeoDL::on_btnDownload_clicked()
{
    ui.mapWidget->downloadSelection(6,ui.chkSplit->isChecked(),ui.edtMaxWidth->text().toInt(),ui.edtMaxHeight->text().toInt());
}


void GeoDL::on_pushButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this);
    if(!directory.isEmpty())
    {
        ui.mapWidget->saveCache(directory);
    }
}
