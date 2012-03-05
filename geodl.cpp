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
    connect(ui.mapWidget,SIGNAL(setDLEnabled(bool)),ui.btnAtlas,SLOT(setEnabled(bool)));
    connect(ui.mapWidget,SIGNAL(zoomChanged(int)),ui.sliderDisp,SLOT(setValue(int)));
    connect(ui.chkAutoSave,SIGNAL(toggled(bool)),ui.mapWidget,SLOT(setAutoSave(bool)));
    connect(ui.mapWidget,SIGNAL(afficheDist(double)),this,SLOT(afficheDist(double)));
    connect(ui.btnLoadGPX,SIGNAL(clicked()),ui.mapWidget,SLOT(loadGPX()));
    connect(ui.btnDelTraces,SIGNAL(clicked()),ui.mapWidget,SLOT(removeTraces()));
    ui.chkAutoSave->setChecked(true);
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
    {
        ui.mapWidget->setCouche(CARTE_IGN);
        ui.sliderDisp->setMinimum(ui.mapWidget->getZoomLevelMin());
        ui.sliderDown->setMinimum(ui.mapWidget->getZoomLevelMin());
    }
}

void GeoDL::on_radioPhotos_toggled(bool checked)
{
    if(checked)
    {
        ui.mapWidget->setCouche(PHOTOS);
        ui.sliderDisp->setMinimum(ui.mapWidget->getZoomLevelMin());
        ui.sliderDown->setMinimum(ui.mapWidget->getZoomLevelMin());
    }
}

void GeoDL::on_radioButton_toggled(bool checked)
{
    if(checked)
    {
        ui.mapWidget->setCouche(CASSINI);
        ui.sliderDisp->setMinimum(ui.mapWidget->getZoomLevelMin());
        ui.sliderDown->setMinimum(ui.mapWidget->getZoomLevelMin());
    }
}

void GeoDL::on_radioButton_2_toggled(bool checked)
{
    if(checked)
    {
        ui.mapWidget->setCouche(CADASTRE);
        ui.sliderDisp->setMinimum(ui.mapWidget->getZoomLevelMin());
        ui.sliderDown->setMinimum(ui.mapWidget->getZoomLevelMin());
    }
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
    ui.mapWidget->downloadSelection(ui.sliderDown->value(),ui.chkSplit->isChecked(),
                                    ui.edtMaxWidth->text().toInt(),ui.edtMaxHeight->text().toInt(),
                                    ui.chkTilesOnly->isChecked());
}


void GeoDL::on_pushButton_clicked()
{
    ui.mapWidget->saveCache();
}


void GeoDL::on_comboBox_currentIndexChanged(int index)
{
    ui.mapWidget->setGeoEngineMode(GeoEngineMode(index));
}

void GeoDL::on_sliderDisp_valueChanged(int value)
{
    ui.lblZoomDisp->setText(QString("Zoom : %1").arg(value));
    ui.mapWidget->setZoomLevel(value);
}

void GeoDL::on_sliderDown_valueChanged(int value)
{
    ui.lblZoomDown->setText(QString("Zoom : %1").arg(value).toAscii());
}

void GeoDL::on_btnLigne_toggled(bool checked)
{
    if(checked)
        ui.mapWidget->setSelectionType(SEL_LIGNE);
}

void GeoDL::on_btnChemin_toggled(bool checked)
{
    if(checked)
        ui.mapWidget->setSelectionType(SEL_CHEMIN);
}

void GeoDL::on_toolBox_currentChanged(int index)
{
    switch(index)
    {
    case 0:
        if(ui.btnLigne->isChecked())
            ui.mapWidget->setSelectionType(SEL_LIGNE);
        if(ui.btnChemin->isChecked())
            ui.mapWidget->setSelectionType(SEL_CHEMIN);
        ui.btnAtlas->setEnabled(false);
        ui.btnDownload->setEnabled(false);
        break;

    case 3:
        ui.mapWidget->setSelectionType(SEL_TELECHARGEMENT);
        break;
    }
}

void GeoDL::afficheDist(double dist)
{
    ui.lblDist->setText(QString("Dist : %1 m").arg(dist,1,'g',-1));
}

void GeoDL::on_btnGotoAddress_clicked()
{
    ui.mapWidget->goToAddress(ui.edtAddress->text());
}


void GeoDL::on_btnAtlas_clicked()
{
    ui.mapWidget->exportAtlas(ui.sliderDown->value());
}
