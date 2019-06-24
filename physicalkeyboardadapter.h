#ifndef PHYSICALKEYBOARDADAPTER_H
#define PHYSICALKEYBOARDADAPTER_H

#include <QObject>

class PhysicalKeyboardAdapter : public QObject
{
    Q_OBJECT
public:
    explicit PhysicalKeyboardAdapter(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PHYSICALKEYBOARDADAPTER_H
