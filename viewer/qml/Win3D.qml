import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12

Entity {
    components: [
        // output, camera
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: "black"
                camera: Camera {
                    id: camera
                    projectionType: CameraLens.PerspectiveProjection
                    fieldOfView: 45
                    nearPlane: 0.1
                    farPlane: 1000.0
                    position: Qt.vector3d(0.0, 40.0, -40.0)
                    upVector: Qt.vector3d(0.0, 1.0, 0.0)
                    viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
                }
            }
            pickingSettings.pickMethod: PickingSettings.TrianglePicking
        },
        // input
        InputSettings {}
    ]
    // controller of camera (input-output mapping)
    OrbitCameraController {
        camera: camera
    }
    // light
    Entity {
        components: [
            DirectionalLight {
                color: Qt.rgba(1,1,1)
                intensity: 1
                worldDirection: Qt.vector3d(0, -1, 0)
            }
        ]
    }
    // model
    Scene {
    }
}
