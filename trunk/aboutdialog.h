#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H


class QEvent;

#include <QDialog>
#include <QTabWidget>


class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(const QString tversion, QWidget *parent = 0);

    bool event(QEvent *event);

private:
    QTabWidget *tabw;
    QWidget *tab1, *tab2, *tab3, *tab4;

};



#endif // ABOUTDIALOG_H
