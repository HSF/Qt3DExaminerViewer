#include "headers/CustomOrbitCameraController.h"
#include <QVector3D>
#include <QCamera>

CustomOrbitCameraController::CustomOrbitCameraController(Qt3DCore::QEntity *rootEntity):
super(rootEntity){

}

inline float clampInputs(float input1, float input2){
    float axisValue = input1 + input2;
    return (axisValue < -1) ? -1 : (axisValue > 1) ? 1 : axisValue;
}

inline float zoomDistance(QVector3D firstPoint, QVector3D secondPoint){
    return (secondPoint - firstPoint).lengthSquared();
}

void CustomOrbitCameraController::moveCamera(const QAbstractCameraController::InputState &state, float dt){
    Qt3DRender::QCamera *theCamera = camera();

    if (theCamera == nullptr)
        return;

    const QVector3D upVector(0.0f, 1.0f, 0.0f);

    // Mouse input
    if (state.leftMouseButtonActive && !state.shiftKeyActive) {
        // Translate
        theCamera->translate(QVector3D(clampInputs(state.rxAxisValue, state.txAxisValue) * linearSpeed(),
                                          clampInputs(state.ryAxisValue, state.tyAxisValue) * linearSpeed(),
                                          0) * dt);
    }
    else if (state.rightMouseButtonActive || (state.shiftKeyActive && state.leftMouseButtonActive)) {
        // Orbit
        theCamera->panAboutViewCenter((state.rxAxisValue * lookSpeed()) * dt, upVector);
        theCamera->tiltAboutViewCenter((state.ryAxisValue * lookSpeed()) * dt);
    }

    // Keyboard Input
    if (state.altKeyActive) {
        // Orbit
        theCamera->panAboutViewCenter((state.txAxisValue * lookSpeed()) * dt, upVector);
        theCamera->tiltAboutViewCenter((state.tyAxisValue * lookSpeed()) * dt);
    } else if (state.shiftKeyActive) {
        theCamera->translate(QVector3D(0, 0, -0.5), theCamera->DontTranslateViewCenter);
    } else {
        // Translate
        theCamera->translate(QVector3D(clampInputs(state.leftMouseButtonActive ? state.rxAxisValue : 0, state.txAxisValue) * linearSpeed(),
                                      clampInputs(state.leftMouseButtonActive ? state.ryAxisValue : 0, state.tyAxisValue) * linearSpeed(),
                                      state.tzAxisValue * linearSpeed()) * dt);
   }
}
