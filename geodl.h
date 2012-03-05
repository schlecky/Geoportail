// =====================================================================================
// 
//       Filename:  mainwindow.h
// 
//    Description:  Telechargement d'images geoportail
// 
//        Version:  1.0
//        Created:  26.08.2009 23:13:31
//       Revision:  none
//       Compiler:  g++
// 
// =====================================================================================
#include <QMainWindow>
#include <QFile>
#include <QHttp>
#include <QBuffer>
#include <QUrl>
#include <QNetworkAccessManager>
#include "tile.h"
#include "mapwidget.h"
#include "ui_mainWindow.h"
#include "geoengine.h"

class GeoDL : public QMainWindow
{
    Q_OBJECT
    public: 
        GeoDL();
        ~GeoDL();
    public slots:
        void afficheDist(double dist); // affiche la distance en m
    private:
        Ui::MainWindow ui;

private slots:
        void on_radioCarte_toggled(bool checked);
        void on_radioPhotos_toggled(bool checked);
        void on_radioButton_toggled(bool checked);
        void on_radioButton_2_toggled(bool checked);
        void on_btnLongLat_clicked();
        void on_mapWidget_coordChange(double,double);
        void on_mapWidget_mouseCoordChange(double,double);
        void on_btnDownload_clicked();
        void on_pushButton_clicked();
        void on_comboBox_currentIndexChanged(int index);
        void on_sliderDisp_valueChanged(int value);
        void on_sliderDown_valueChanged(int value);
        void on_btnLigne_toggled(bool checked);
        void on_btnChemin_toggled(bool checked);
        void on_toolBox_currentChanged(int index);
        void on_btnGotoAddress_clicked();
        void on_btnAtlas_clicked();
};
