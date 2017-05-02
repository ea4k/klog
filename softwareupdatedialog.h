#ifndef SOFTWAREUPDATEDIALOG_H
#define SOFTWAREUPDATEDIALOG_H
#include <QtWidgets>
#include <QDialog>
#include <QTextBrowser>

class SoftwareUpdateDialog: public QDialog
{
    Q_OBJECT
public:
    SoftwareUpdateDialog();
    ~SoftwareUpdateDialog();
    void setVersion(const QString tversion);

private slots:
    void slotAcceptButtonClicked();

private:
    void keyPressEvent(QKeyEvent *event);
     QTextBrowser *textBrowser;
     QString text;
     QString _version;

};

#endif // SOFTWAREUPDATEDIALOG_H


