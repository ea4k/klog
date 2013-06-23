#ifndef HELPABOUTDIALOG_H
#define HELPABOUTDIALOG_H
#include <QtGui>
#include <QDialog>
#include <QTextBrowser>


class HelpAboutDialog : public QDialog
{
    Q_OBJECT

public:
    HelpAboutDialog();
    ~HelpAboutDialog();

private slots:
    void slotAcceptButtonClicked();
    //void slotCancelButtonClicked();



private:
     QTextBrowser *textBrowser;
};

#endif // HELPABOUTDIALOG_H
