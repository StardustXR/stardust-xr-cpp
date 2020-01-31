#include "inputpointer.h"

InputPointer::InputPointer(QObject *parent) : QObject(parent) {}

InputPointer::InputPointer(QObject *parent, Qt3DRender::QRayCaster *caster) : QObject(parent) {
    m_ray = caster;
    initialize();
}

void InputPointer::sendEvent(Qt3DCore::QNode *target, QString eventType, Qt3DRender::QPickEvent *event) {
    Qt3DRender::QPickEventPtr eventPtr(event);
    if(hasHit()) {
        target->sendCommand(eventType, QVariant::fromValue(eventPtr));
    }
}


void InputPointer::initialize() {
    connect(m_ray, &Qt3DRender::QRayCaster::hitsChanged, this, &InputPointer::rayUpdated);
}

void InputPointer::componentComplete() {
    initialize();
}

void InputPointer::rayUpdated(const Qt3DRender::QAbstractRayCaster::Hits &hits) {
    m_hasHit = (hits.length() > 0);
    Qt3DRender::QRayCasterHit hit(hits.at(m_offset));

    if(m_hasHit) {
        delete m_hit;
        m_hit = new Qt3DRender::QRayCasterHit(hit);
    } else {
        delete m_hit;
    }
}

bool InputPointer::hasHit() const {
    return m_hasHit;
}

uint InputPointer::offset() const {
    return m_offset;
}

void InputPointer::setOffset(const uint &offset) {
    m_offset = offset;
}
