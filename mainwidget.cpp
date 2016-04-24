#include "mainwidget.h"
#include "MyWidget.h"
#include "QString"
MainWidget::MainWidget(QWidget *parent) : QWidget(parent),MySett("aQm","MyPr")
{
    showMaximized();
    showFullScreen();

    Menu=new QWidget(this);

    New=new QPushButton("New");
    Continue=new QPushButton("Continue");
    OptionsB=new QPushButton("Options");
    Exit=new QPushButton("Exit");

    btnList.append(New);
    btnList.append(Continue);
    btnList.append(OptionsB);
    btnList.append(Exit);


    connect(New,SIGNAL(clicked()),SLOT(PushNewGame()));
    connect(Continue,SIGNAL(clicked()),SLOT(PushContinue()));
    connect(OptionsB,SIGNAL(clicked()),SLOT(PushOptionsBtn()));
    connect(Exit,SIGNAL(clicked()),SLOT(PushExit()));



    ContinueEnable=MySett.value("/Settings/ContinueEnable","0").toBool();
    if (!ContinueEnable) Continue->setDisabled(true);

    MenuLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    MenuLayout->addWidget(New);
    MenuLayout->addWidget(Continue);
    MenuLayout->addWidget(OptionsB);
    MenuLayout->addWidget(Exit);


    Menu->setLayout(MenuLayout);
    Menu->resize(QApplication::desktop()->screenGeometry().bottomRight().rx(),QApplication::desktop()->screenGeometry().bottomRight().ry());






    Delay = new QTimer(this);
    SizeDelay = new QTimer(this);
    connect(Delay,SIGNAL(timeout()),SLOT(DelayOut()));
    connect(Delay,SIGNAL(timeout()),SLOT(SizeOut()));


//GameWidget
    w=new MyWidget(this);
    connect(w,SIGNAL(WriteSett()),SLOT(WriteSett()));
    connect(w,SIGNAL(Save()),this,SLOT(Save()));
    connect(w,SIGNAL(GameOver()),this,SLOT(GameOverSlot()));

    connect(this,SIGNAL(GameOver()),SLOT(GameOver2Continue()));


    w->resize(QApplication::desktop()->screenGeometry().bottomRight().rx(),QApplication::desktop()->screenGeometry().bottomRight().ry());
    w->hide();


//OptionWidget
    Options=new QWidget(this);

    SizeDown=new QPushButton("-");
    Segment::ConstX=MySett.value("/Settings/ConstX","3").toInt();
    Size=new QPushButton(QString::number(Segment::ConstX));
    SizeUp=new QPushButton("+");
    Type = new QPushButton(this);
    OptionsBtnBack = new QPushButton("Back");
    Segment::TypeOfGame=MySett.value("/Settings/TypeOfGame","0").toInt();
    switch (Segment::TypeOfGame){
    case 0: Type->setText("Classic");
        break;
    case 1: Type->setText("Five-in-a-row");
        break;
    case 2: Type->setText("Tic-tac-toe");
    }




    Size->setDisabled(true);



    QBoxLayout* OptionsLayout =new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout* SizeLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout* TypeLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout* BackLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    SizeLayout->addWidget(SizeDown);
    SizeLayout->addWidget(Size);
    SizeLayout->addWidget(SizeUp);
    TypeLayout->addWidget(Type);
    BackLayout->addWidget(OptionsBtnBack);
    OptionsLayout->addLayout(SizeLayout);
    OptionsLayout->addLayout(TypeLayout);
    OptionsLayout->addLayout(BackLayout);

    connect(SizeDown,SIGNAL(clicked()),SLOT(DownSizeArea()));
    connect(SizeUp,SIGNAL(clicked()),SLOT(UpSizeArea()));
    connect(Type,SIGNAL(clicked()),SLOT(ChangeType()));
    connect(OptionsBtnBack,SIGNAL(clicked()),SLOT(OptionsBack()));

    Options->setLayout(OptionsLayout);


    Options->resize(QApplication::desktop()->screenGeometry().bottomRight().rx(),QApplication::desktop()->screenGeometry().bottomRight().ry());

    Options->hide();

    QPalette pal;
    pal.setBrush(this->backgroundRole(),QBrush(Qt::white));
    pal.setBrush(Menu->backgroundRole(),QBrush(Qt::white));
    pal.setBrush(w->backgroundRole(),QBrush(Qt::white));
    pal.setBrush(Options->backgroundRole(),QBrush(Qt::white));

    this->setPalette(pal);
    Menu->setPalette(pal);
    w->setPalette(pal);
    Options->setPalette(pal);

    this->setAutoFillBackground(true);
    Menu->setAutoFillBackground(true);
    w->setAutoFillBackground(true);
    Options->setAutoFillBackground(true);




}

void MainWidget::PushNewGame(){
    Menu->hide();

    Segment::ConstX=MySett.value("/Settings/ConstX","3").toInt();

    Segment::TypeOfGame=MySett.value("/Settings/TypeOfGame","0").toInt();
    switch (Segment::TypeOfGame){
    case 0: Segment::Point=3;
            Segment::ConstX=3;
            Segment::PointWin=1;
        break;
    case 1: Segment::Point=5;
            Segment::PointWin=1;
        break;
    case 2: Segment::Point=3;
            Segment::PointWin=Segment::ConstX*Segment::ConstX;
        break;
    }



    w->SetCoord();

    for(int i=0;i<Segment::ConstX;i++){
        for(int j=0;j<Segment::ConstX;j++){
            w->Box[i][j].value=VOIDs;
        }
    }
    w->Action=CROSS;
    w->Last[0]=0;
    w->Last[1]=0;
    w->Turn=0;
    Segment::PointP1 = 0;
    Segment::PointP2 = 0;

    emit w->Save();
    w->show();
}

void MainWidget::PushContinue(){
    Menu->hide();
    Segment::ConstX=MySett.value("/Settings/SaveConstX","3").toInt();
    Segment::TypeOfGame=MySett.value("/Settings/SaveTypeOfGame","0").toInt();
    w->SetCoord();
    ReadSett();
    w->show();
}

void MainWidget::PushOptionsBtn(){
    Menu->hide();

     Segment::ConstX=MySett.value("/Settings/ConstX","3").toInt();
     Segment::TypeOfGame=MySett.value("/Settings/TypeOfGame","0").toInt();
    Options->show();
}

void MainWidget::OptionsBack(){
    Options->hide();
    Menu->show();
}

void MainWidget::PushExit(){
    QApplication::quit();
}

void MainWidget::GameOverSlot(){


    ContinueEnable=false;
    MySett.setValue("/Settings/ContinueEnable",ContinueEnable);
    Continue->setDisabled(true);

    if (Segment::PointP1>Segment::PointP2)
        QMessageBox::information(0, "GAME OVER", "<H2> Player 1 WIN!!! </H2>");
    else
        if (Segment::PointP1<Segment::PointP2)
            QMessageBox::information(0, "GAME OVER", "<H2> Player 2 WIN!!! </H2>");
        else
            QMessageBox::information(0, "GAME OVER", "<H2> DRAW!!! </H2>");

    w->hide();
    if (w->Box.size()){
        while(w->Box.size()){
            while(w->Box.first().size()){
                w->Box.first().removeFirst();
            }
            w->Box.removeFirst();
        }
    }
    Menu->show();
}

void MainWidget::GameOver2Continue(){
    ContinueEnable=true;
    MySett.setValue("/Settings/ContinueEnable",ContinueEnable);
    Continue->setEnabled(true);
    w->hide();
    if (w->Box.size()){
        while(w->Box.size()){
            while(w->Box.first().size()){
                w->Box.first().removeFirst();
            }
            w->Box.removeFirst();
        }
    }
    Menu->show();
}

void MainWidget::WriteSett(){
    MySett.beginGroup("/Settings");

    MySett.setValue("/PointP1",Segment::PointP1);
    MySett.setValue("/PointP2",Segment::PointP2);

    MySett.setValue("/SaveConstX",Segment::ConstX);
    MySett.setValue("/SaveTypeOfGame",Segment::TypeOfGame);

    MySett.setValue("/Action",w->Action);
    MySett.setValue("/Turn",w->Turn);
    MySett.setValue("/Last0",w->Last[0]);
    MySett.setValue("/Last1",w->Last[1]);

    ContinueEnable=true;
    MySett.setValue("/ContinueEnable",ContinueEnable);

    QString Str;


    for (int i=0;i<Segment::ConstX;i++){
        for (int j=0;j<Segment::ConstX;j++){
            Str=QString("/");
            QString str;
            Str+=str.setNum(i);
            Str+=str.setNum(j);
            MySett.setValue(Str,w->Box[i][j].value);
            if (w->Box[i][j].value==ZEROWIN) MySett.setValue(Str,ZEROWIN);
            if (w->Box[i][j].value==ZEROWIN) MySett.setValue(Str,CROSSWIN);
        }
    }

    MySett.endGroup();
}

void MainWidget::keyPressEvent(QKeyEvent *p){
    if ((p->key()==Qt::Key_Back)||(p->key()==Qt::Key_Escape)){
        if (!Menu->isHidden()){
            emit Exit->clicked();
            return;
        }
        if (!Options->isHidden()){
            OptionsBack();
            return;
        }
        if (!w->isHidden()){
            emit GameOver();
        }
    }
}



void MainWidget::ReadSett(){
    MySett.beginGroup("/Settings");

    QString Str;

    Segment::PointP1=MySett.value("/PointP1","0").toInt();
    Segment::PointP2=MySett.value("/PointP2","0").toInt();
    Segment::ConstX=MySett.value("/SaveConstX","3").toInt();
    Segment::TypeOfGame=MySett.value("SaveTypeOfGame","0").toInt();
    Size->setText(QString::number(MySett.value("/ConstX","3").toInt()));
    w->Turn=MySett.value("/Turn","0").toInt();
    w->Last[0]=MySett.value("/Last0","0").toInt();
    w->Last[1]=MySett.value("/Last1","0").toInt();
    ContinueEnable=MySett.value("/ContinueEnable","0").toBool();




    switch (MySett.value("/Action","0").toInt()){
    case 0: w->Action=CROSS;
        break;
    case 1: w->Action=ZERO;
        break;
    }


    for (int i=0;i<Segment::ConstX;i++){
        for (int j=0;j<Segment::ConstX;j++){
            Str=QString("/");
            QString str;
            Str+=str.setNum(i);
            Str+=str.setNum(j);
            int X=MySett.value(Str,"-1").toInt();
            switch (X){
            case 0: w->Box[i][j].value=CROSS;
                break;
            case 1: w->Box[i][j].value=ZERO;
                break;
            case 2: w->Box[i][j].value=VOIDs;
                break;
            }
        }
    }



    MySett.endGroup();
}

void MainWidget::UpSizeArea(){
    int Max=10;
    if (Segment::TypeOfGame==1) Max=13;
    if(Segment::ConstX<Max){
        Segment::ConstX++;
        Size->setText(QString::number(Segment::ConstX));
        MySett.setValue("/Settings/ConstX",Segment::ConstX);
    }
}

void MainWidget::DownSizeArea(){
    int Min=4;
    if (Segment::TypeOfGame==1) Min=10;
    if (Segment::ConstX>Min){
        Segment::ConstX--;
        Size->setText(QString::number(Segment::ConstX));
        MySett.setValue("/Settings/ConstX",Segment::ConstX);
    }
}

void MainWidget::ChangeType(){
    Segment::TypeOfGame=(Segment::TypeOfGame+1)%3;
    switch (Segment::TypeOfGame){
    case 0: Type->setText("Classic");
            SizeDown->setDisabled(true);
            SizeUp->setDisabled(true);
            Size->setText(QString::number(3));
            MySett.setValue("/Settings/ConstX",3);
            Segment::ConstX=3;
        break;
    case 1: Type->setText("Five-in-a-row");
            SizeDown->setEnabled(true);
            SizeUp->setEnabled(true);
            if (Segment::ConstX<10){
                Size->setText(QString::number(10));
                MySett.setValue("/Settings/ConstX",10);
                Segment::ConstX=10;
            }
        break;
    case 2: Type->setText("Tic-tac-toe");
            SizeDown->setEnabled(true);
            SizeUp->setEnabled(true);
            if (Segment::ConstX>10){
                Size->setText(QString::number(10));
                MySett.setValue("/Settings/ConstX",10);
                Segment::ConstX=10;
            }
        break;
    }
    MySett.setValue("/Settings/TypeOfGame",Segment::TypeOfGame);
}


void MainWidget::rearrangeButtons(){

    int btnCount=btnList.size();
    for(int i=0;i<btnCount;i++){
        QPushButton* btn=btnList.at(i);
        QPropertyAnimation* animation = new QPropertyAnimation(btn, "geometry");
        QRect Rect= btn->geometry();
        animation->setDuration(3000);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        btn->setGeometry(QRect(0,0,btn->geometry().width(),btn->geometry().height()));
        animation->setEndValue(Rect);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWidget::DelayOut(){
    this->show();
    Menu->show();
    rearrangeButtons();
    Delay->stop();
}
void MainWidget::SizeOut(){
    SizeDelay->stop();
}



