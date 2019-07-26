#ifndef WAYLANDKEYBOARDHANDLER_H
#define WAYLANDKEYBOARDHANDLER_H

#include <QObject>
#include <QQmlParserStatus>
#include "keyboardhandler.h"
#include <QWaylandSeat>
#include <QWaylandSurface>

class WaylandKeyboardHandler : public KeyboardHandler, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QWaylandSurface *surf READ surf WRITE setSurf)
public:
    explicit WaylandKeyboardHandler(QObject *parent = nullptr);

    QWaylandSurface * surf();
    void setSurf(QWaylandSurface *surf);

    void classBegin() override {}
    void componentComplete() override;

public slots:
    void fullKeyEvent(QObject *keyEvent) override;
    void keyEvent(int key, bool pressed) override;
private:
    QWaylandSurface *m_waylandSurface = nullptr;
    QWaylandSeat *m_waylandSeat = nullptr;
};

#endif // WAYLANDKEYBOARDHANDLER_H
