#ifndef NINFOBOX_H
#define NINFOBOX_H

#include "../QtHeads.h"

class NInfoBox : public QWidget
{
    Q_OBJECT
public:
    explicit NInfoBox(QWidget *parent = 0, int Length = 300, int Width = 100, int Mergen = 5);

signals:

public slots:

public:
    QWidget *m_pWidget;
};

int easyMessageBox(QString info);


#endif // NINFOBOX_H
