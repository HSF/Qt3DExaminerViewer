#ifndef CUSTOMORBITCAMERACONTROLLER_H
#define CUSTOMORBITCAMERACONTROLLER_H
#include <Qt3DExtras/qorbitcameracontroller.h>

class CustomOrbitCameraController: public Qt3DExtras::QOrbitCameraController
{
public :
   typedef Qt3DExtras::QOrbitCameraController super;
public:
    CustomOrbitCameraController(Qt3DCore::QEntity *rootEntity);
    void moveCamera(const QAbstractCameraController::InputState &state, float dt) override;
};

#endif // CUSTOMORBITCAMERACONTROLLER_H
