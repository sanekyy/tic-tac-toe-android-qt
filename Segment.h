#pragma once

#include <QtGui>
#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <QMouseEvent>
#include <qrect.h>
#include <qtimer.h>
#include <QTimerEvent>
#include <qdesktopwidget.h>
#include <QkeyEvent>
#include <qmessagebox.h>
#include <qfont.h>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QList>
#include <QPalette>
#include <QIcon>
#include <QPushButton>

enum MyEnum{CROSS,ZERO,VOIDs,CROSSWIN,ZEROWIN,CROSSLAST,ZEROLAST};


class Segment
{
	static int MaxX;
	static int MaxY;
    static int MinXY;
	static int ConstX;
    static int SaveConstX;
	static int SizeR;
    static int Point; // очков для очка
    static int PointWin; // очков для победы
	static int PointP1;
	static int PointP2;
    static int TypeOfGame;


	MyEnum value;
	QRect Rect;

public:
	Segment();
	~Segment();
	friend class MyWidget;
    friend class MainWidget;
};

