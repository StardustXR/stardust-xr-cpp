#ifndef MODULE_H
#define MODULE_H

#include <QObject>

class Module : public QObject
{
    Q_OBJECT
public:
    explicit Module(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MODULE_H
