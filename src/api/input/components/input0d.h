#ifndef STARDUSTAPI_INPUT0D_H
#define STARDUSTAPI_INPUT0D_H


namespace StardustAPI {

class Input0D {
public:
    virtual bool isActive() {
        return active;
    }

protected:
    bool active = false;
};

}

#endif // STARDUSTAPI_INPUT0D_H
