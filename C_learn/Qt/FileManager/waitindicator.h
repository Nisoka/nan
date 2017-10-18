#ifndef WAITINDICATOR_H
#define WAITINDICATOR_H

#include <QWidget>


namespace Ui {
class WaitIndicator;
}

class WaitIndicator : public QWidget
{
    Q_OBJECT
    
public:
    explicit WaitIndicator(QWidget *parent = 0);
    ~WaitIndicator();

private:
    Ui::WaitIndicator *ui;

};

#endif // WAITINDICATOR_H
