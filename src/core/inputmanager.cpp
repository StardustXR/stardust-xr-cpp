#include "inputmanager.h"


namespace Stardust {

InputManager::InputManager(QObject *parent) : QObject(parent) {

}

void InputManager::registerInputDevice(InputDevice *device) {
    if(inputDevices.contains(device))
        return;
    inputDevices.append(device);
    inputs.append(device->inputs);
}

void InputManager::registerInputHandler(InputHandler *handler) {
    if(!inputHandlers.contains(handler))
        inputHandlers.append(handler);
}

void InputManager::processInputs() {
    //Refresh the rejection of actions

    foreach(InputHandler *handler, inputHandlers)
        handler->rejectAction = false;
    foreach(Input *input, inputs)
        input->rejectAction = false;

    //Create representations of input interaction with input handlers
    float maxDistance = 0.0f;
    QList<InputHandlerLink> distanceLinks;
    foreach(InputHandler *handler, inputHandlers) {
        maxDistance = std::fmaxf(maxDistance, handler->maxDistance);
        foreach(Input *input, inputs) {
            if(!input->enabled)
                continue;
            distanceLinks.append(InputHandlerLink{});
            InputHandlerLink *link = &distanceLinks.last();
            link->input = input;
            link->handler = handler;

            if(input->type == InputType::Global)
                link->distance = handler->maxDistance - 0.001f;
            else
                link->distance = input->distanceToField(handler->field);
        }
    }

    QList<InputHandlerLink *> processedLinks;
    float processedDistance = 0.0f;
    //Repeat enough times so that all links are covered
    for(int i=0; i<distanceLinks.length(); ++i) {
        InputHandlerLink *linkToProcess = nullptr;
        float minDistance = maxDistance;

        //Look for next link to process
        for(int j=distanceLinks.length()-1; j>-1; --j) {
            InputHandlerLink *link = &distanceLinks[i];
            if(processedLinks.contains(link)) {
                continue;
            }
            if(link->distance < minDistance && link->distance >= processedDistance) {
                minDistance = link->distance;
                linkToProcess = link;
            }
        }

        if(!linkToProcess->input->rejectAction && !linkToProcess->handler->rejectAction) {
            QList<ActionTrigger *> actionTriggers = linkToProcess->handler->actionTriggers;
            foreach(ActionTrigger *actionTrigger, actionTriggers) {
                if(linkToProcess->input->type == actionTrigger->type) {
                    actionTrigger->testAction(linkToProcess->input, linkToProcess->distance);
                }
            }
        }
    }
}

}
