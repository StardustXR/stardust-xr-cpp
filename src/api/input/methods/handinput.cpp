#include "handinput.h"

namespace StardustAPI {
namespace Input {

HandInput::HandInput(QQuick3DNode *parent) : Input(parent, InputType::Hand) {}

float HandInput::distanceToField(StardustAPI::Fields::Field *field) {
    return field->distance(this->mapPositionToScene(localInteractPoint));
}

HandInputBone::HandInputBone(HandInputBone *parent) : QQuick3DNode(parent), parentBone(parent) {

}

float HandInputBone::length() {
    if(childBones.isEmpty())
        return 0.0f;

    return childBones[0]->scenePosition().distanceToPoint(scenePosition());
}

QVector3D HandInputBone::direction() {
    return (childBones[0]->scenePosition() - scenePosition()).normalized();
}

QQmlListProperty<HandInputBone> HandInputBone::qmlChildBones() {
    return {this, &childBones,
             &HandInputBone::appendChildBone,
             &HandInputBone::childBoneCount,
             &HandInputBone::childBone,
                &HandInputBone::clearChildBones};
}

void HandInputBone::appendChildBone(QQmlListProperty<HandInputBone> *property, HandInputBone *p) {
    HandInputBone *bone = qobject_cast<HandInputBone *>(property->object);
    bone->childBones.append(p);
}

int HandInputBone::childBoneCount(QQmlListProperty<HandInputBone> *property) {
    HandInputBone *bone = qobject_cast<HandInputBone *>(property->object);
    return bone->childBones.length();
}

HandInputBone *HandInputBone::childBone(QQmlListProperty<HandInputBone> *property, int index) {
    HandInputBone *bone = qobject_cast<HandInputBone *>(property->object);
    return bone->childBones.at(index);
}

void HandInputBone::clearChildBones(QQmlListProperty<HandInputBone> *property) {
    HandInputBone *bone = qobject_cast<HandInputBone *>(property->object);
    bone->childBones.clear();
}

}
}
