#ifndef INPUTPOINTER_H
#define INPUTPOINTER_H

#include <QObject>
#include <QQmlParserStatus>

#include <QNode>
#include <QRayCaster>
#include <QPickEvent>
#include <QPickEventPtr>

#include <QCoreApplication>

class InputPointer : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(Qt3DRender::QRayCaster *rayCaster MEMBER m_ray)
    Q_PROPERTY(Qt3DRender::QRayCasterHit *hit MEMBER m_hit)
    Q_PROPERTY(bool hasHit READ hasHit)
    Q_PROPERTY(uint hitOffset READ offset WRITE setOffset)
public:
    explicit InputPointer(QObject *parent = nullptr);
    explicit InputPointer(QObject *parent = nullptr, Qt3DRender::QRayCaster *caster = nullptr);

    virtual void leftClick() = 0;
    virtual void middleClick() = 0;
    virtual void rightClick() = 0;
    virtual void back() = 0;

    void sendEvent(Qt3DCore::QNode *target, QString eventType, Qt3DRender::QPickEvent *event);

private slots:
    void rayUpdated(const Qt3DRender::QAbstractRayCaster::Hits &hits);

private:
    Qt3DRender::QRayCaster *m_ray;
    Qt3DRender::QRayCasterHit *m_hit;

    uint m_offset = 0;
    bool m_hasHit = false;

    void initialize();

    // QQmlParserStatus interface
public:
    void classBegin() {}
    void componentComplete();
    uint offset() const;
    void setOffset(const uint &offset);
    bool hasHit() const;
};

#endif // INPUTPOINTER_H
