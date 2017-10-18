#ifndef RENAMEDLG_H
#define RENAMEDLG_H

#include <QDialog>
#include <QCopChannel>

//#define PC_DEBUG    1
namespace Ui {
class RenameDlg;
}

class RenameDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit RenameDlg(QWidget *parent = 0);
    ~RenameDlg();
    QString getFileName();
    void setFileName(QString name);

protected:
    virtual void showEvent (QShowEvent * event);
public slots:
    void okBtnClicked();
    void cancelBtnClicked();
private:
    Ui::RenameDlg *ui;
    QString m_newName;
};

#endif // RENAMEDLG_H
