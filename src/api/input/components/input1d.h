#ifndef STARDUSTAPI_INPUT1D_H
#define STARDUSTAPI_INPUT1D_H

#include "input0d.h"

namespace StardustAPI {
namespace Input {

class Input1D : public Input0D {
public:
    virtual bool isActive() {
        return value > activeThreshold;
    }

    virtual float getValue() {
        return value;
    }

protected:
    float activeThreshold = 0.0f;
    float value = 1.0f;
};

}
}

#endif // STARDUSTAPI_INPUT1D_H
