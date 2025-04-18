#include "cabrillo.h"

Cabrillo::Cabrillo(QObject *parent)
    : QObject{parent}
{

}

void Cabrillo::init()
{

}

QString Cabrillo::getFrequency(const Frequency &_fr)
{
    if (_fr.toDouble(KHz) < 30000)
        return _fr.toQString(KHz);
    if (_f)

}
