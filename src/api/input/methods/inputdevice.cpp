#include "inputdevice.h"

namespace StardustAPI {
namespace Input {

QVariantList InputDevice::variantInputs() {
    QVariantList variantList;
    foreach (Input *input, inputs) {
        variantList.append(QVariant::fromValue(input));
    }
    return variantList;
}

void InputDevice::setInputsVariant(QVariantList variantInputs) {
    inputs.clear();
    foreach(QVariant variant, variantInputs) {
        Input *input = qobject_cast<Input *>(qvariant_cast<QObject *>(variant));
        inputs.append(input);
    }
}



}
}
