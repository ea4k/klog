#ifndef HELPABOUTDIALOG_H
#define HELPABOUTDIALOG_H
//#include <QtWidgets>
#include <QtWidgets>
#include <QDialog>
 #include <QTextBrowser>


class HelpAboutDialog : public QDialog
{
    Q_OBJECT

public:
    HelpAboutDialog(const QString tversion);
    ~HelpAboutDialog();

private slots:
    void slotAcceptButtonClicked();
    //void slotCancelButtonClicked();



private:
    void keyPressEvent(QKeyEvent *event);

     QTextBrowser *textBrowser;
};

#endif // HELPABOUTDIALOG_H
