/********************************************************************************
** Form generated from reading UI file 'view.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_View
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_4;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_4;
    QLineEdit *attemptsText;
    QLineEdit *benchPathText;
    QLabel *label_2;
    QLabel *label;
    QGridLayout *gridLayout_3;
    QCheckBox *powersaveButton;
    QCheckBox *performanceButton;
    QCheckBox *conservativeButton;
    QCheckBox *schedutilButton;
    QCheckBox *ondemandButton;
    QCheckBox *userspaceButton;
    QLabel *label_3;
    QPushButton *compareButton;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *View)
    {
        if (View->objectName().isEmpty())
            View->setObjectName(QString::fromUtf8("View"));
        View->resize(2882, 1450);
        centralwidget = new QWidget(View);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 1000, 701));

        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(1050, 70, 1900, 1600));

        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);

        gridLayout_4 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";color:rgb(204, 0, 0);"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);


        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font: 18pt \"Ubuntu\";color:rgb(204, 0, 0);"));
        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        attemptsText = new QLineEdit(gridLayoutWidget);
        attemptsText->setObjectName(QString::fromUtf8("attemptsText"));

        gridLayout_2->addWidget(attemptsText, 3, 1, 1, 1);

        benchPathText = new QLineEdit(gridLayoutWidget);
        benchPathText->setObjectName(QString::fromUtf8("benchPathText"));

        gridLayout_2->addWidget(benchPathText, 2, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 3, 0, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        powersaveButton = new QCheckBox(gridLayoutWidget);
        powersaveButton->setObjectName(QString::fromUtf8("powersaveButton"));

        gridLayout_3->addWidget(powersaveButton, 0, 0, 1, 1);

        performanceButton = new QCheckBox(gridLayoutWidget);
        performanceButton->setObjectName(QString::fromUtf8("performanceButton"));

        gridLayout_3->addWidget(performanceButton, 1, 0, 1, 1);

        conservativeButton = new QCheckBox(gridLayoutWidget);
        conservativeButton->setObjectName(QString::fromUtf8("conservativeButton"));

        gridLayout_3->addWidget(conservativeButton, 3, 0, 1, 1);

        schedutilButton = new QCheckBox(gridLayoutWidget);
        schedutilButton->setObjectName(QString::fromUtf8("schedutilButton"));

        gridLayout_3->addWidget(schedutilButton, 2, 0, 1, 1);

        ondemandButton = new QCheckBox(gridLayoutWidget);
        ondemandButton->setObjectName(QString::fromUtf8("ondemandButton"));

        gridLayout_3->addWidget(ondemandButton, 4, 0, 1, 1);

        userspaceButton = new QCheckBox(gridLayoutWidget);
        userspaceButton->setObjectName(QString::fromUtf8("userspaceButton"));

        gridLayout_3->addWidget(userspaceButton, 5, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_3, 4, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 4, 0, 1, 1);

        compareButton = new QPushButton(gridLayoutWidget);
        compareButton->setObjectName(QString::fromUtf8("compareButton"));

        gridLayout_2->addWidget(compareButton, 5, 0, 1, 2);


        gridLayout->addLayout(gridLayout_2, 1, 0, 1, 1);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 1900, 1600));
        gridLayout_4->addWidget(tableWidget, 1, 0, 1, 1);

        View->setCentralWidget(centralwidget);
        menubar = new QMenuBar(View);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 2282, 39));
        View->setMenuBar(menubar);
        statusbar = new QStatusBar(View);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        View->setStatusBar(statusbar);

        retranslateUi(View);

        QMetaObject::connectSlotsByName(View);
    } // setupUi

    void retranslateUi(QMainWindow *View)
    {
        View->setWindowTitle(QApplication::translate("View", "View", nullptr));
        label_5->setText(QApplication::translate("View", "\320\230\321\201\321\205\320\276\320\264\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        label_4->setText(QApplication::translate("View", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        attemptsText->setText(QApplication::translate("View", "2", nullptr));
        attemptsText->setPlaceholderText(QString());
        label_2->setText(QApplication::translate("View", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\267\320\260\320\277\321\203\321\201\320\272\320\276\320\262 \320\277\321\200\320\276\321\206\320\265\321\201\321\201\320\260 \321\203\320\277\321\200\320\260\320\262\320\273\321\217\321\216\321\211\320\270\320\274 \321\207\320\260\321\201\321\202\320\276\321\202\320\260\320\274\320\270: ", nullptr));
        label->setText(QApplication::translate("View", "\320\237\321\203\321\202\321\214 \320\272 \320\270\321\201\320\277\320\276\320\273\320\275\321\217\320\265\320\274\320\276\320\274\321\203 \321\204\320\260\320\271\320\273\321\203 \320\277\321\200\320\276\321\206\320\265\321\201\321\201\320\260: ", nullptr));
        powersaveButton->setText(QApplication::translate("View", "powersave", nullptr));
        performanceButton->setText(QApplication::translate("View", "performance", nullptr));
        conservativeButton->setText(QApplication::translate("View", "conservative", nullptr));
        schedutilButton->setText(QApplication::translate("View", "schedutil", nullptr));
        ondemandButton->setText(QApplication::translate("View", "ondemand", nullptr));
        userspaceButton->setText(QApplication::translate("View", "mrpi-based (userspace)", nullptr));
        label_3->setText(QApplication::translate("View", "\320\243\320\277\321\200\320\260\320\262\320\273\321\217\321\216\321\211\320\270\320\265 \321\207\320\260\321\201\321\202\320\276\321\202\320\260\320\274\320\270:", nullptr));
        compareButton->setText(QApplication::translate("View", "\320\277\321\200\320\276\320\262\320\265\321\201\321\202\320\270 \321\201\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class View: public Ui_View {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_H
