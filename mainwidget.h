#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QWidget>

#include <MyWidget.h>
#include <QtGui>
#include <QLayout>
#include <QSettings>

class MainWidget : public QWidget
{
    Q_OBJECT

    MyWidget* w=0;

    QWidget* Menu=0;
    QWidget* Options=0;
    QPushButton* Size;
    QPushButton* SizeDown;
    QPushButton* SizeUp;
    QPushButton* Type;
    QPushButton* OptionsBtnBack;


    QTimer* Delay;
    QTimer* SizeDelay;

    QPushButton* New;
    QPushButton* Continue;
    QPushButton* OptionsB;
    QPushButton* Exit;

    QList<QRect> btn_R_List;
    QList<QPushButton*> btnList;

    QBoxLayout* MenuLayout;

    QSettings MySett;
    bool ContinueEnable;
public:

    //explicit
    MainWidget(QWidget *parent = 0);


    void ReadSett();
    friend class MyApplication;
    friend void WriteSett();

    void keyPressEvent(QKeyEvent *key);
    void closeEvent(QCloseEvent*p){
        p->ignore();
    }
    virtual void resizeEvent(QResizeEvent* re){
        if ((this!=0)&&(Menu!=0)&&(Options!=0)&&(w!=0)&&(!SizeDelay->isActive())){
        Menu->setGeometry(QRect(0,0,this->geometry().width(),this->geometry().height()));
        Options->setGeometry(QRect(0,0,this->geometry().width(),this->geometry().height()));
        w->setGeometry(QRect(0,0,this->geometry().width(),this->geometry().height()));
        if (!w->isHidden()){
        w->SetCoord();
        ReadSett();
        }
        }
    }


signals:
    void GameOver();

public slots:
    void DelayOut();
    void SizeOut();

    void PushNewGame();
    void PushContinue();
    void PushOptionsBtn();
        void OptionsBack();
            void UpSizeArea();
            void DownSizeArea();
            void ChangeType();
    void PushExit();
    void rearrangeButtons();

    void GameOverSlot();
    void GameOver2Continue();
    void WriteSett();

public slots:
    void Save(){
        WriteSett();
    }

};

#endif // MAINWIDGET_H
