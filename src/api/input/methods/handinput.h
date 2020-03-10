#ifndef STARDUSTAPI_HANDINPUT_H
#define STARDUSTAPI_HANDINPUT_H

#include "input.h"

namespace StardustAPI {
namespace Input {

class HandInputBone : public QQuick3DNode {
    Q_OBJECT
    Q_PROPERTY(float width MEMBER width)
    Q_PROPERTY(float length READ length)
    Q_PROPERTY(QVector3D sceneDirection READ forward)

    Q_PROPERTY(HandInputBone *parentBone MEMBER parentBone)
    Q_PROPERTY(QQmlListProperty<StardustAPI::Input::HandInputBone> childBones READ qmlChildBones)
    Q_CLASSINFO("DefaultProperty", "childBones")
public:
    explicit HandInputBone(HandInputBone *parent = nullptr);

    float width;
    float length();

    QVector3D direction();

    HandInputBone *parentBone;
    QList<HandInputBone *> childBones;
    QQmlListProperty<HandInputBone> qmlChildBones();

private:
    static void appendChildBone(QQmlListProperty<HandInputBone>*, HandInputBone *p);
    static int childBoneCount(QQmlListProperty<HandInputBone>*);
    static HandInputBone *childBone(QQmlListProperty<HandInputBone>*, int index);
    static void clearChildBones(QQmlListProperty<HandInputBone>*);
};

class HandInput : public Input {
    Q_OBJECT
    Q_PROPERTY(HandInputBone *arm MEMBER arm)
    Q_PROPERTY(HandInputBone *wrist MEMBER wrist)

public:
    explicit HandInput(QQuick3DNode *parent = nullptr);

    HandInputBone *arm;
    HandInputBone *wrist;

    QVector3D localInteractPoint;
    virtual float distanceToField(StardustAPI::Fields::Field *field);
};

}
}

#endif // STARDUSTAPI_HANDINPUT_H
