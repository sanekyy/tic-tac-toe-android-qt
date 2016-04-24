#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H
#include <MyWidget.h>
#include <mainwidget.h>
#include <QApplication>
#include <QSpinBox>
#include <QWindow>

class MyApplication: public QApplication
{
    MainWidget* Main=0;
    QBoxLayout* MainLayout;
public:
    MyApplication(int argc,char**argv):QApplication(argc,argv){
        QApplication::desktop()->showFullScreen();
        //setWindowIcon(QIcon(QPixmap(":/myresources/CROSS.png")));
        Main=new MainWidget();
        Main->Delay->start(1000);
        Main->SizeDelay->start(3000);
    }


    virtual ~MyApplication(){}
    void closeEvent(QCloseEvent*p){
        p->ignore();
    }
    friend class MainWidget;

};

#endif // MYAPPLICATION_H
