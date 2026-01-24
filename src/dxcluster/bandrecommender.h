#ifndef BANDRECOMMENDER_H
#define BANDRECOMMENDER_H

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class BandRecommender : public QWidget
{
    Q_OBJECT
public:
    explicit BandRecommender(QWidget *parent = nullptr);
    void createUI();

signals:

private:
    QPushButton *button1, *button2, *button3, *button4, *button5;
};

#endif // BANDRECOMMENDER_H
