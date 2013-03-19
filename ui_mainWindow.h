/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created: Sun 18. Nov 02:29:11 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "mapwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QToolBox *toolBox;
    QWidget *pgMesures;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QPushButton *btnLigne;
    QPushButton *btnChemin;
    QLabel *lblDist;
    QLabel *label_7;
    QLabel *label_9;
    QLineEdit *edtCircLat;
    QLabel *label_8;
    QLineEdit *edtCircLong;
    QLabel *label_10;
    QLineEdit *edtCircR;
    QPushButton *btnAddCircle;
    QPushButton *btnDelCircles;
    QSpacerItem *verticalSpacer_5;
    QWidget *pgAffichage;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *chkReticule;
    QLabel *label_6;
    QComboBox *comboBox;
    QRadioButton *radioPhotos;
    QRadioButton *radioButton;
    QRadioButton *radioCarte;
    QRadioButton *radioButton_2;
    QLabel *lblZoomDisp;
    QSlider *sliderDisp;
    QPushButton *btnLoadGPX;
    QListWidget *gpxList;
    QPushButton *btnDelTraces;
    QSpacerItem *verticalSpacer_2;
    QWidget *pgNavigation;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_2;
    QLineEdit *edtLatitude;
    QLabel *label_3;
    QLineEdit *edtLongitude;
    QPushButton *btnLongLat;
    QLabel *lblAddress;
    QLineEdit *edtAddress;
    QPushButton *btnGotoAddress;
    QSpacerItem *verticalSpacer;
    QWidget *page;
    QVBoxLayout *verticalLayout_7;
    QLabel *lblZoomDown;
    QSlider *sliderDown;
    QCheckBox *chkSplit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *edtMaxWidth;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *edtMaxHeight;
    QSpacerItem *verticalSpacer_3;
    QCheckBox *chkTilesOnly;
    QPushButton *btnDownload;
    QPushButton *btnAtlas;
    QPushButton *btnAnnuler;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton;
    QCheckBox *chkAutoSave;
    QCheckBox *chkForceDL;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *vLayout;
    QHBoxLayout *horizontalLayout_6;
    MapWidget *mapWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(974, 684);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMouseTracking(false);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy);
        toolBox->setMinimumSize(QSize(150, 300));
        toolBox->setMaximumSize(QSize(120, 16777215));
        toolBox->setFrameShape(QFrame::Panel);
        pgMesures = new QWidget();
        pgMesures->setObjectName(QString::fromUtf8("pgMesures"));
        pgMesures->setGeometry(QRect(0, 0, 148, 462));
        verticalLayout = new QVBoxLayout(pgMesures);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(pgMesures);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QFont font;
        font.setBold(true);
        font.setUnderline(false);
        font.setWeight(75);
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_5);

        btnLigne = new QPushButton(pgMesures);
        btnLigne->setObjectName(QString::fromUtf8("btnLigne"));
        btnLigne->setCheckable(true);
        btnLigne->setChecked(true);
        btnLigne->setAutoExclusive(true);

        verticalLayout->addWidget(btnLigne);

        btnChemin = new QPushButton(pgMesures);
        btnChemin->setObjectName(QString::fromUtf8("btnChemin"));
        btnChemin->setCheckable(true);
        btnChemin->setAutoExclusive(true);

        verticalLayout->addWidget(btnChemin);

        lblDist = new QLabel(pgMesures);
        lblDist->setObjectName(QString::fromUtf8("lblDist"));

        verticalLayout->addWidget(lblDist);

        label_7 = new QLabel(pgMesures);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_7);

        label_9 = new QLabel(pgMesures);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout->addWidget(label_9);

        edtCircLat = new QLineEdit(pgMesures);
        edtCircLat->setObjectName(QString::fromUtf8("edtCircLat"));

        verticalLayout->addWidget(edtCircLat);

        label_8 = new QLabel(pgMesures);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout->addWidget(label_8);

        edtCircLong = new QLineEdit(pgMesures);
        edtCircLong->setObjectName(QString::fromUtf8("edtCircLong"));

        verticalLayout->addWidget(edtCircLong);

        label_10 = new QLabel(pgMesures);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout->addWidget(label_10);

        edtCircR = new QLineEdit(pgMesures);
        edtCircR->setObjectName(QString::fromUtf8("edtCircR"));

        verticalLayout->addWidget(edtCircR);

        btnAddCircle = new QPushButton(pgMesures);
        btnAddCircle->setObjectName(QString::fromUtf8("btnAddCircle"));

        verticalLayout->addWidget(btnAddCircle);

        btnDelCircles = new QPushButton(pgMesures);
        btnDelCircles->setObjectName(QString::fromUtf8("btnDelCircles"));

        verticalLayout->addWidget(btnDelCircles);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        toolBox->addItem(pgMesures, QString::fromUtf8("Mesures"));
        pgAffichage = new QWidget();
        pgAffichage->setObjectName(QString::fromUtf8("pgAffichage"));
        pgAffichage->setGeometry(QRect(0, 0, 118, 408));
        verticalLayout_2 = new QVBoxLayout(pgAffichage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        chkReticule = new QCheckBox(pgAffichage);
        chkReticule->setObjectName(QString::fromUtf8("chkReticule"));

        verticalLayout_2->addWidget(chkReticule);

        label_6 = new QLabel(pgAffichage);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QFont font1;
        font1.setPointSize(9);
        label_6->setFont(font1);

        verticalLayout_2->addWidget(label_6);

        comboBox = new QComboBox(pgAffichage);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        comboBox->setMinimumSize(QSize(100, 0));
        comboBox->setFont(font1);

        verticalLayout_2->addWidget(comboBox);

        radioPhotos = new QRadioButton(pgAffichage);
        radioPhotos->setObjectName(QString::fromUtf8("radioPhotos"));
        radioPhotos->setFont(font1);

        verticalLayout_2->addWidget(radioPhotos);

        radioButton = new QRadioButton(pgAffichage);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setFont(font1);

        verticalLayout_2->addWidget(radioButton);

        radioCarte = new QRadioButton(pgAffichage);
        radioCarte->setObjectName(QString::fromUtf8("radioCarte"));
        radioCarte->setFont(font1);
        radioCarte->setChecked(true);

        verticalLayout_2->addWidget(radioCarte);

        radioButton_2 = new QRadioButton(pgAffichage);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setFont(font1);

        verticalLayout_2->addWidget(radioButton_2);

        lblZoomDisp = new QLabel(pgAffichage);
        lblZoomDisp->setObjectName(QString::fromUtf8("lblZoomDisp"));
        lblZoomDisp->setLayoutDirection(Qt::LeftToRight);

        verticalLayout_2->addWidget(lblZoomDisp);

        sliderDisp = new QSlider(pgAffichage);
        sliderDisp->setObjectName(QString::fromUtf8("sliderDisp"));
        sliderDisp->setMinimum(4);
        sliderDisp->setMaximum(20);
        sliderDisp->setPageStep(1);
        sliderDisp->setValue(10);
        sliderDisp->setOrientation(Qt::Horizontal);
        sliderDisp->setInvertedAppearance(false);
        sliderDisp->setTickPosition(QSlider::TicksBelow);
        sliderDisp->setTickInterval(1);

        verticalLayout_2->addWidget(sliderDisp);

        btnLoadGPX = new QPushButton(pgAffichage);
        btnLoadGPX->setObjectName(QString::fromUtf8("btnLoadGPX"));

        verticalLayout_2->addWidget(btnLoadGPX);

        gpxList = new QListWidget(pgAffichage);
        gpxList->setObjectName(QString::fromUtf8("gpxList"));

        verticalLayout_2->addWidget(gpxList);

        btnDelTraces = new QPushButton(pgAffichage);
        btnDelTraces->setObjectName(QString::fromUtf8("btnDelTraces"));

        verticalLayout_2->addWidget(btnDelTraces);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        toolBox->addItem(pgAffichage, QString::fromUtf8("Affichage"));
        pgNavigation = new QWidget();
        pgNavigation->setObjectName(QString::fromUtf8("pgNavigation"));
        pgNavigation->setGeometry(QRect(0, 0, 118, 244));
        verticalLayout_8 = new QVBoxLayout(pgNavigation);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_2 = new QLabel(pgNavigation);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        verticalLayout_8->addWidget(label_2);

        edtLatitude = new QLineEdit(pgNavigation);
        edtLatitude->setObjectName(QString::fromUtf8("edtLatitude"));
        sizePolicy1.setHeightForWidth(edtLatitude->sizePolicy().hasHeightForWidth());
        edtLatitude->setSizePolicy(sizePolicy1);
        edtLatitude->setMaximumSize(QSize(100, 16777215));
        edtLatitude->setFont(font1);

        verticalLayout_8->addWidget(edtLatitude);

        label_3 = new QLabel(pgNavigation);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        verticalLayout_8->addWidget(label_3);

        edtLongitude = new QLineEdit(pgNavigation);
        edtLongitude->setObjectName(QString::fromUtf8("edtLongitude"));
        sizePolicy1.setHeightForWidth(edtLongitude->sizePolicy().hasHeightForWidth());
        edtLongitude->setSizePolicy(sizePolicy1);
        edtLongitude->setMinimumSize(QSize(100, 0));
        edtLongitude->setMaximumSize(QSize(100, 16777215));
        edtLongitude->setFont(font1);

        verticalLayout_8->addWidget(edtLongitude);

        btnLongLat = new QPushButton(pgNavigation);
        btnLongLat->setObjectName(QString::fromUtf8("btnLongLat"));
        btnLongLat->setFont(font1);

        verticalLayout_8->addWidget(btnLongLat);

        lblAddress = new QLabel(pgNavigation);
        lblAddress->setObjectName(QString::fromUtf8("lblAddress"));

        verticalLayout_8->addWidget(lblAddress);

        edtAddress = new QLineEdit(pgNavigation);
        edtAddress->setObjectName(QString::fromUtf8("edtAddress"));

        verticalLayout_8->addWidget(edtAddress);

        btnGotoAddress = new QPushButton(pgNavigation);
        btnGotoAddress->setObjectName(QString::fromUtf8("btnGotoAddress"));

        verticalLayout_8->addWidget(btnGotoAddress);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);

        toolBox->addItem(pgNavigation, QString::fromUtf8("Navigation"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 142, 288));
        verticalLayout_7 = new QVBoxLayout(page);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        lblZoomDown = new QLabel(page);
        lblZoomDown->setObjectName(QString::fromUtf8("lblZoomDown"));

        verticalLayout_7->addWidget(lblZoomDown);

        sliderDown = new QSlider(page);
        sliderDown->setObjectName(QString::fromUtf8("sliderDown"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sliderDown->sizePolicy().hasHeightForWidth());
        sliderDown->setSizePolicy(sizePolicy2);
        sliderDown->setMaximumSize(QSize(130, 16777215));
        sliderDown->setMinimum(4);
        sliderDown->setMaximum(20);
        sliderDown->setPageStep(1);
        sliderDown->setValue(7);
        sliderDown->setOrientation(Qt::Horizontal);
        sliderDown->setTickPosition(QSlider::TicksBelow);
        sliderDown->setTickInterval(1);

        verticalLayout_7->addWidget(sliderDown);

        chkSplit = new QCheckBox(page);
        chkSplit->setObjectName(QString::fromUtf8("chkSplit"));
        sizePolicy1.setHeightForWidth(chkSplit->sizePolicy().hasHeightForWidth());
        chkSplit->setSizePolicy(sizePolicy1);
        chkSplit->setFont(font1);

        verticalLayout_7->addWidget(chkSplit);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font1);

        horizontalLayout_3->addWidget(label);

        edtMaxWidth = new QLineEdit(page);
        edtMaxWidth->setObjectName(QString::fromUtf8("edtMaxWidth"));
        sizePolicy1.setHeightForWidth(edtMaxWidth->sizePolicy().hasHeightForWidth());
        edtMaxWidth->setSizePolicy(sizePolicy1);
        edtMaxWidth->setMinimumSize(QSize(40, 0));
        edtMaxWidth->setMaximumSize(QSize(60, 16777215));
        edtMaxWidth->setFont(font1);

        horizontalLayout_3->addWidget(edtMaxWidth);


        verticalLayout_7->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(page);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        horizontalLayout_2->addWidget(label_4);

        edtMaxHeight = new QLineEdit(page);
        edtMaxHeight->setObjectName(QString::fromUtf8("edtMaxHeight"));
        sizePolicy1.setHeightForWidth(edtMaxHeight->sizePolicy().hasHeightForWidth());
        edtMaxHeight->setSizePolicy(sizePolicy1);
        edtMaxHeight->setMaximumSize(QSize(60, 16777215));
        edtMaxHeight->setFont(font1);

        horizontalLayout_2->addWidget(edtMaxHeight);


        verticalLayout_7->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_3);

        chkTilesOnly = new QCheckBox(page);
        chkTilesOnly->setObjectName(QString::fromUtf8("chkTilesOnly"));
        QFont font2;
        font2.setPointSize(8);
        chkTilesOnly->setFont(font2);

        verticalLayout_7->addWidget(chkTilesOnly);

        btnDownload = new QPushButton(page);
        btnDownload->setObjectName(QString::fromUtf8("btnDownload"));
        btnDownload->setEnabled(false);

        verticalLayout_7->addWidget(btnDownload);

        btnAtlas = new QPushButton(page);
        btnAtlas->setObjectName(QString::fromUtf8("btnAtlas"));
        btnAtlas->setEnabled(false);

        verticalLayout_7->addWidget(btnAtlas);

        btnAnnuler = new QPushButton(page);
        btnAnnuler->setObjectName(QString::fromUtf8("btnAnnuler"));

        verticalLayout_7->addWidget(btnAnnuler);

        toolBox->addItem(page, QString::fromUtf8("T\303\251l\303\251chargement"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 171, 105));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton = new QPushButton(page_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setFont(font1);

        verticalLayout_3->addWidget(pushButton);

        chkAutoSave = new QCheckBox(page_2);
        chkAutoSave->setObjectName(QString::fromUtf8("chkAutoSave"));
        chkAutoSave->setFont(font1);
        chkAutoSave->setChecked(false);

        verticalLayout_3->addWidget(chkAutoSave);

        chkForceDL = new QCheckBox(page_2);
        chkForceDL->setObjectName(QString::fromUtf8("chkForceDL"));

        verticalLayout_3->addWidget(chkForceDL);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        toolBox->addItem(page_2, QString::fromUtf8("Options"));

        horizontalLayout->addWidget(toolBox);

        vLayout = new QVBoxLayout();
        vLayout->setObjectName(QString::fromUtf8("vLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, -1, -1);
        mapWidget = new MapWidget(centralwidget);
        mapWidget->setObjectName(QString::fromUtf8("mapWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(2);
        sizePolicy3.setHeightForWidth(mapWidget->sizePolicy().hasHeightForWidth());
        mapWidget->setSizePolicy(sizePolicy3);
        mapWidget->setMinimumSize(QSize(300, 300));
        mapWidget->setMouseTracking(true);

        horizontalLayout_6->addWidget(mapWidget);


        vLayout->addLayout(horizontalLayout_6);


        horizontalLayout->addLayout(vLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 974, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(edtAddress, SIGNAL(returnPressed()), btnGotoAddress, SLOT(click()));

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "G\303\251oportail", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Distance", 0, QApplication::UnicodeUTF8));
        btnLigne->setText(QApplication::translate("MainWindow", "Ligne", 0, QApplication::UnicodeUTF8));
        btnChemin->setText(QApplication::translate("MainWindow", "Chemin", 0, QApplication::UnicodeUTF8));
        lblDist->setText(QApplication::translate("MainWindow", "Dist : --", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Triangulation", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Latitude centre", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Longitude centre", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Rayon (m)", 0, QApplication::UnicodeUTF8));
        btnAddCircle->setText(QApplication::translate("MainWindow", "Ajouter cercle", 0, QApplication::UnicodeUTF8));
        btnDelCircles->setText(QApplication::translate("MainWindow", "Supprimer cercles", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pgMesures), QApplication::translate("MainWindow", "Mesures", 0, QApplication::UnicodeUTF8));
        chkReticule->setText(QApplication::translate("MainWindow", "R\303\251ticule", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Connect\303\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Local", 0, QApplication::UnicodeUTF8)
        );
        radioPhotos->setText(QApplication::translate("MainWindow", "Photos", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("MainWindow", "Cassini", 0, QApplication::UnicodeUTF8));
        radioCarte->setText(QApplication::translate("MainWindow", "Carte", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("MainWindow", "Cadastre", 0, QApplication::UnicodeUTF8));
        lblZoomDisp->setText(QApplication::translate("MainWindow", "Zoom : 10", 0, QApplication::UnicodeUTF8));
        btnLoadGPX->setText(QApplication::translate("MainWindow", "Charger gpx", 0, QApplication::UnicodeUTF8));
        btnDelTraces->setText(QApplication::translate("MainWindow", "Effacer traces", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pgAffichage), QApplication::translate("MainWindow", "Affichage", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "latitude", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "longitude", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnLongLat->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnLongLat->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        lblAddress->setText(QApplication::translate("MainWindow", "Adresse", 0, QApplication::UnicodeUTF8));
        btnGotoAddress->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pgNavigation), QApplication::translate("MainWindow", "Navigation", 0, QApplication::UnicodeUTF8));
        lblZoomDown->setText(QApplication::translate("MainWindow", "Zoom :  7", 0, QApplication::UnicodeUTF8));
        chkSplit->setText(QApplication::translate("MainWindow", "plusieurs fichiers", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Larg. Max", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Haut. Max", 0, QApplication::UnicodeUTF8));
        edtMaxHeight->setText(QString());
        chkTilesOnly->setText(QApplication::translate("MainWindow", "Uniquement tuiles", 0, QApplication::UnicodeUTF8));
        btnDownload->setText(QApplication::translate("MainWindow", "Telecharger", 0, QApplication::UnicodeUTF8));
        btnAtlas->setText(QApplication::translate("MainWindow", "Atlas", 0, QApplication::UnicodeUTF8));
        btnAnnuler->setText(QApplication::translate("MainWindow", "Annuler", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "T\303\251l\303\251chargement", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Save cache", 0, QApplication::UnicodeUTF8));
        chkAutoSave->setText(QApplication::translate("MainWindow", "autoSave", 0, QApplication::UnicodeUTF8));
        chkForceDL->setText(QApplication::translate("MainWindow", "toujours t\303\251l\303\251charger", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
