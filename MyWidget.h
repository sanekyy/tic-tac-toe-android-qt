#pragma once
#include <Segment.h>

class MyWidget:public QWidget
{
	Q_OBJECT

    QVector<QVector<Segment> > Box;
    QPixmap Pic[7];
    QTimer* Timer1;
    QTimer* Timer2;
    QTimer* TimerWin;
    int ListIter;
    QVector<QVector<Segment*> > WinArrays;
	MyEnum Action;
    int Last[2];
	int Turn;
    QPoint oldPos;

    QString* StrPoint1;
    QString* StrPoint2;
    QPoint Point1;
    QPoint Point2;
    QPushButton* BTNPoint1;
    QPushButton* BTNPoint2;

public:
	MyWidget(QWidget*parent=0);
	~MyWidget();
	void paintEvent(QPaintEvent*p);
    void mousePressEvent(QMouseEvent*p);
    void mouseReleaseEvent(QMouseEvent*p);
    void closeEvent(QCloseEvent*p){
        p->ignore();
    }
	int Win();
    void SetCoord();
    bool CheckWin();
    friend class MainWidget;
    friend class MyApplication;
signals:
    void GameOver();
    void WriteSett();
    void Save();

public slots:
    void TimerWinOut();
};

