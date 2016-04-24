#include "MyWidget.h"
#include "QGraphicsView"


MyWidget::MyWidget(QWidget*parent) : QWidget(parent)
{
    Segment::MaxX = QApplication::desktop()->screenGeometry().bottomRight().rx(); // Сохраняем значение ширины
    Segment::MaxY = QApplication::desktop()->screenGeometry().bottomRight().ry();// Сохраняем значение высоты

    TimerWin=new QTimer(this);

    connect(TimerWin,SIGNAL(timeout()),SLOT(TimerWinOut()));


	Action = CROSS;
	Last[0] = 0;
	Last[1] = 0;
	Turn = 0;
    Segment::PointP1 = 0;
    Segment::PointP2 = 0;
    Pic[0] = QPixmap(":/myresources/CROSS.png");
    Pic[1] = QPixmap(":/myresources/ZERO.png");
    Pic[2] = QPixmap(":/myresources/VOID.png");
    Pic[3] = QPixmap(":/myresources/CROSSWIN.png");
    Pic[4] = QPixmap(":/myresources/ZEROWIN.png");
    Pic[5] = QPixmap(":/myresources/CROSSLAST.png");
    Pic[6] = QPixmap(":/myresources/ZEROLAST.png");


    StrPoint1 = new QString;
    StrPoint2 = new QString;
    BTNPoint1 = new QPushButton(this);
    BTNPoint2 = new QPushButton(this);
    BTNPoint1->setDisabled(true);
    BTNPoint2->setDisabled(true);


}


MyWidget::~MyWidget()
{
}

void MyWidget::SetCoord(){

    Segment::MaxX = QApplication::desktop()->screenGeometry().bottomRight().rx(); // Сохраняем значение ширины
    Segment::MaxY = QApplication::desktop()->screenGeometry().bottomRight().ry();// Сохраняем значение высоты
    if (Segment::MaxX<Segment::MaxY) Segment::MinXY=Segment::MaxX;
    else{
        Segment::MaxY-=35;
        Segment::MinXY=Segment::MaxY;
    }



    Segment::SizeR = Segment::MinXY / Segment::ConstX; // Вычисляем размер Сегмента

    // Выщитывается для смещения от центра
    float factorX = (Segment::MaxX - Segment::SizeR*Segment::ConstX) / 2;
    float factorY = (Segment::MaxY - Segment::SizeR*Segment::ConstX) / 2;

    if (Segment::ConstX % 2){
        factorX += 0.5;
        factorY +=0.5;
    }

    //QWidget::move(QPoint(Segment::MaxX / 2 - factor * Segment::SizeR, 0));
    Segment::MaxX = QApplication::desktop()->screenGeometry().bottomRight().rx();
    //Segment::MaxX =  Segment::SizeR * Segment::ConstX;// Сохраняем значение ширины
    Segment::MaxY = Segment::SizeR * Segment::ConstX;// Сохраняем значение высоты

    if (this->isHidden()){
        for(int i=0; i<Segment::ConstX;i++){
           QVector<Segment> A;
           for (int j=0; j<Segment::ConstX;j++){
               Segment B;
               A.append(B);
           }
           Box.append(A);
        }
    }

    for (int i = 0; i < Segment::ConstX; i++){
        for (int j = 0; j < Segment::ConstX; j++){
            Box[i][j].Rect = QRect(factorX + j*Segment::SizeR, i*Segment::SizeR+factorY, Segment::SizeR, Segment::SizeR);
        }
    }
}

void MyWidget::paintEvent(QPaintEvent*p){
	QPainter Painter(this);
    for (int i = 0; i < Segment::ConstX; i++){
        for (int j = 0; j < Segment::ConstX; j++){
			Painter.drawPixmap(Box[i][j].Rect, Pic[Box[i][j].value]);
		}
	}
    for (int i = 1; i < Segment::ConstX; i++){
        Painter.drawLine(QLine(Box[0][i].Rect.topLeft(), Box[Segment::ConstX-1][i].Rect.bottomLeft()));
        Painter.drawLine(QLine(Box[i][0].Rect.topLeft(), Box[i][Segment::ConstX-1].Rect.topRight()));
	}


    StrPoint1->setNum(Segment::PointP1,10);
    StrPoint2->setNum(Segment::PointP2,10);
    BTNPoint1->setText(*StrPoint1);
    BTNPoint1->setGeometry(QRect(25,25,100,75));
    BTNPoint2->setText(*StrPoint2);
    BTNPoint2->setGeometry(QRect(Segment::MaxX-125,25,100,75));

    if (Action==CROSS){
            Painter.setPen(QPen(Qt::green,5,Qt::SolidLine));
            Painter.drawRect(QRect(24,24,102,77));
    }
    else{
        Painter.setPen(QPen(Qt::green,7,Qt::SolidLine));
        Painter.drawRect(QRect(Segment::MaxX-126,24,102,77));
    }
}


void MyWidget::mousePressEvent(QMouseEvent*p){
        oldPos=p->pos();
}


void MyWidget::mouseReleaseEvent(QMouseEvent *p){
    QPoint Offset=p->pos()-oldPos;
    if (TimerWin->isActive()){
        return;
    }
    if (Segment::SizeR/4>(sqrt(Offset.x()*Offset.x()+Offset.y()*Offset.y()))){
        for (int i = 0; i < Segment::ConstX; i++){
            for (int j = 0; j < Segment::ConstX; j++){
                if (Box[i][j].Rect .contains(p->pos())&&(Box[i][j].value==VOIDs)){
                    Box[i][j].value = Action;
                    if (Action) Action = CROSS;
                    else Action = ZERO;
                    Last[0] = i;
                    Last[1] = j;
                    Turn++;
                    int win;
                    if ((Box[Last[0]][Last[1]].value != VOIDs) && (Turn >= (Segment::Point * 2 - 1)) && (win = Win())){
                        if (Action == CROSS){
                            Segment::PointP2 += win;
                            if (!ListIter){
                            for (int i=0;i<Segment::Point;i++){
                                if (WinArrays.at(ListIter).at(i)->Rect==Box[Last[0]][Last[1]].Rect) WinArrays.at(ListIter).at(i)->value=ZEROLAST;
                                else WinArrays.at(ListIter).at(i)->value=ZEROWIN;
                            }
                            TimerWin->start(300);
                            }
                        }
                        else{
                            Segment::PointP1 += win;
                            if (!ListIter){
                            for (int i=0;i<Segment::Point;i++){
                                if (WinArrays.at(ListIter).at(i)->Rect==Box[Last[0]][Last[1]].Rect) WinArrays.at(ListIter).at(i)->value=CROSSLAST;
                                else WinArrays.at(ListIter).at(i)->value=CROSSWIN;
                            }
                            TimerWin->start(300);
                            }
                        }
                    }
                    break;
                }
            }
        }
        update();
        if (!CheckWin())
            emit Save();
    }
}


int MyWidget::Win(){
    int WinIter=0;
    ListIter=0;
    int PoinT = 1;
    int win = 0;

    QVector<Segment*> WinRects;


    // Снизу вверх
    while(WinIter<Segment::Point){
        if((Last[0]+WinIter<Segment::ConstX)&&(Box[Last[0]+WinIter][Last[1]].value==Box[Last[0]][Last[1]].value)){
            PoinT=1;
            WinRects.append(&Box[Last[0]+WinIter][Last[1]]);
            for (int i = Last[0]+WinIter - 1; ((i >= 0) && (i > Last[0]+WinIter - Segment::Point)); i--){ // Проверяем вверх, начиная с элемента, первого сверху, пока не упрёмся в конец поля или пока не проверим Point элементов
                if (Box[Last[0]+WinIter][Last[1]].value == Box[i][Last[1]].value){ // Проверяемый совпадает с искомым?
                    PoinT++;
                    WinRects.append(&Box[i][Last[1]]);
                    if (PoinT == Segment::Point){
                        win++;
                        WinArrays.append(WinRects);
                        while(WinRects.size()){
                            WinRects.removeFirst();
                        }
                    }
                }
                else{
                    while(WinRects.size()){
                        WinRects.removeFirst();
                    }
                    break;
                    }// Если нет, поиск вверх окончен
            }
        }
        WinIter++;
    }




    //Справа налево

    PoinT=1;
    WinIter=0;

    while(WinIter<Segment::Point){
        if ((Last[1] + WinIter < Segment::ConstX) && (Box[Last[0]][Last[1]+WinIter].value == Box[Last[0]][Last[1]].value)){
            PoinT = 1;
            WinRects.append(&Box[Last[0]][Last[1]+WinIter]);
            for (int i = Last[1]+WinIter - 1; ((i >= 0) && (i>Last[1]+WinIter - Segment::Point)); i--){
                if (Box[Last[0]][Last[1]+WinIter].value == Box[Last[0]][i].value){
                    PoinT++;
                    WinRects.append(&Box[Last[0]][i]);
                    if (PoinT == Segment::Point){
                        win++;
                        WinArrays.append(WinRects);
                        while(WinRects.size()){
                            WinRects.removeFirst();
                        }
                    }
                }
                else{
                    while(WinRects.size()){
                        WinRects.removeFirst();
                    }
                    break;
                    }// Если нет, поиск вверх окончен
            }
        }
        WinIter++;
    }

    //По диагонали Вправо Вверх

    PoinT=1;
    WinIter=0;

    while(WinIter<Segment::Point){
        if ((Last[1] - WinIter >= 0) && (Last[0] + WinIter < Segment::ConstX) && (Box[Last[0] + WinIter][Last[1] - WinIter].value == Box[Last[0]][Last[1]].value)){
            PoinT = 1;
            WinRects.append(&Box[Last[0]+WinIter][Last[1]-WinIter]);



            int j = Last[1]-WinIter+1;
            int i =Last[0]+WinIter - 1;

            for(i; (( i >= 0 ) && (j < Segment::ConstX) && (i > Last[0]+WinIter - Segment::Point) && (j > Last[1]-WinIter-Segment::Point)); i--, j++){
                if (Box[Last[0]+WinIter][Last[1]-WinIter].value == Box[i][j].value){
                    PoinT++;
                    WinRects.append(&Box[i][j]);
                    if (PoinT == Segment::Point){
                        win++;
                        WinArrays.append(WinRects);
                        while(WinRects.size()){
                            WinRects.removeFirst();
                        }
                    }

                }
                else{
                    while(WinRects.size()){
                        WinRects.removeFirst();
                    }
                    break;
                    }// Если нет, поиск вверх окончен
            }
        }
        WinIter++;
    }


    //По диагонали Вправо Вниз



    PoinT=1;
    WinIter=0;

    while (WinIter<Segment::Point){
         if ((Last[1] - WinIter >= 0) && (Last[0] - WinIter >= 0) && (Box[Last[0] - WinIter][Last[1] - WinIter].value == Box[Last[0]][Last[1]].value)){
            PoinT = 1;
            WinRects.append(&Box[Last[0]-WinIter][Last[1]-WinIter]);
            int j = Last[1]-WinIter+1;
            int i = Last[0]-WinIter+1;


            for (i; ((i < Segment::ConstX) && (j < Segment::ConstX) && (i > Last[0] - WinIter - Segment::Point) && (j > Last[1] - WinIter-Segment::Point )); i++, j++){
                if (Box[Last[0]-WinIter][Last[1]-WinIter].value == Box[i][j].value){
                    PoinT++;
                    WinRects.append(&Box[i][j]);
                    if (PoinT == Segment::Point){
                        win++;
                        WinArrays.append(WinRects);
                        while(WinRects.size()){
                            WinRects.removeFirst();
                        }
                    }
                }
                else{
                    while(WinRects.size()){
                        WinRects.removeFirst();
                    }
                    break;
                    }// Если нет, поиск вверх окончен
            }
         }
         WinIter++;
    }


	return win;
}


void MyWidget::TimerWinOut(){
    QVector<Segment*> WinRects=WinArrays.at(ListIter);
    for (int i=0;i<Segment::Point;i++){
        switch (WinRects[i]->value){
        case ZEROWIN: WinRects[i]->value=ZERO;
            break;
        case ZEROLAST: WinRects[i]->value=ZERO;
            break;
        case CROSSWIN: WinRects[i]->value=CROSS;
            break;
        case CROSSLAST: WinRects[i]->value=CROSS;
            break;
        }
    }
    ListIter++;

    if (ListIter==(WinArrays.size())){
        TimerWin->stop();
        while(WinArrays.size()){       
            WinArrays.removeFirst();
        }
        emit Save();
        update();
        return;
    }

    WinRects=WinArrays.at(ListIter);
    for (int i=0;i<Segment::Point;i++){
        switch (WinRects[i]->value){
        case ZERO: if (WinRects[i]->Rect==Box[Last[0]][Last[1]].Rect) WinRects[i]->value=ZEROLAST;
                   else WinRects[i]->value=ZEROWIN;
            break;
        case CROSS: if (WinRects[i]->Rect==Box[Last[0]][Last[1]].Rect) WinRects[i]->value=CROSSLAST;
                    else WinRects[i]->value=CROSSWIN;
            break;
        }
    }
    emit Save();
    update();
}

bool MyWidget::CheckWin(){
    switch (Segment::TypeOfGame){
    case 0: if (Segment::PointP1>=Segment::PointWin||Segment::PointP2>=Segment::PointWin||((Segment::ConstX*Segment::ConstX)==Turn)) { emit GameOver(); return true; }
        break;
    case 1: if (Segment::PointP1>=Segment::PointWin||Segment::PointP2>=Segment::PointWin||((Segment::ConstX*Segment::ConstX)==Turn)) { emit GameOver(); return true; }
        break;
    case 2: if ((Segment::ConstX*Segment::ConstX)==Turn) { emit GameOver(); return true; }
    }
}
