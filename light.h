#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>

class Light : public QObject
{
    Q_OBJECT

public:
    explicit Light(QObject *parent = nullptr);

private:
    QString label;
    QList<quint8> mac;

signals:

};

#endif // LIGHT_H
