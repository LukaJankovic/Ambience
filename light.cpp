#include "light.h"

Light::Light(QObject *parent)
    : QObject{parent} {}

Light::Light(const QHostAddress &address, const QList<quint8> &serial) : address(address),
    serial(serial) {}
