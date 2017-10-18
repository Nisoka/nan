#ifndef NEWFOLDERDLG_H
#define NEWFOLDERDLG_H

#include <QDialog>
#include <QCopChannel>

//#define PC_DEBUG    1

namespace Ui {
class NewFolderDlg;
}

class NewFolderDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewFolderDlg(QWidget *parent = 0);
    ~NewFolderDlg();
    QString getFolderName();
protected:
    virtual void showEvent (QShowEvent * event);
public slots:
    void okBtnClicked();
    void cancelBtnClicked();
    
private:
    Ui::NewFolderDlg *ui;
    QString m_folderName;
};

#endif // NEWFOLDERDLG_H
