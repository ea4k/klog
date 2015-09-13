#ifndef HELPHELPDIALOG_H
#define HELPHELPDIALOG_H
//#include <QtWidgets>
#include <QtGui>
#include <QDialog>
#include <QTextBrowser>


class HelpHelpDialog : public QDialog
{
    Q_OBJECT

public:
    HelpHelpDialog(const QString tversion);
    ~HelpHelpDialog();

private slots:
    void slotAcceptButtonClicked();
    //void slotCancelButtonClicked();



private:
     //void keyPressEvent(QKeyEvent *event);
     QTextBrowser *textBrowser;

};
#endif // HELPHELPDIALOG_H
