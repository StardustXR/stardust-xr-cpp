#ifndef STARDUSTAPI_INPUT2D_H
#define STARDUSTAPI_INPUT2D_H

#include "input0d.h"
#include <QVector2D>
#include <math.h>

using namespace std;

namespace StardustAPI {
namespace Input {

class Input2D : public Input0D {
public:
    virtual bool isActive() {
        return abs(position.x()) > abs(deadzone.x()) && abs(position.y()) > abs(deadzone.y());
    }

    virtual QVector2D getPosition() {
        return position;
    }

protected:
    QVector2D deadzone = QVector2D(0,0);
    QVector2D position = QVector2D(0,0);
};

}
}

#endif // STARDUSTAPI_INPUT2D_H
