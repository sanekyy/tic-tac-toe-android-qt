#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QObject>
#include <QWidget>

class MySettings : public QApplication
{
    Q_OBJECT
public:
    explicit MySettings(QWidget *parent = 0);

signals:

public slots:
};

#endif // MYSETTINGS_H
