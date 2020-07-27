import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12

Entity{
    id: scenceRoot
    Entity{
        components: [
            CuboidMesh {},
            DiffuseSpecularMaterial { ambient: "#444"; shininess: 100;},
            Transform { scale: 10 },
            ObjectPicker {
                hoverEnabled: true
                onClicked: {
                    console.log("clicked");
                }
                onContainsMouseChanged: {
                    console.log("contains mouse?", containsMouse);
                }
            }
        ]
    }
    LogicalDevice {
        axes: [
            Axis {
                inputs: [
                    AnalogAxisInput {
                        sourceDevice: MouseDevice
                        axis: MouseDevice.X
                    }
                ]
                onValueChanged: {
                    console.log("mouse axis value: ", value);
                }
            },
            Axis {
                inputs: [
                    ButtonAxisInput {
                        sourceDevice: keyboardDevice
                        buttons: [Qt.Key_Left]
                        scale: -1.0
                    },
                    ButtonAxisInput {
                        sourceDevice: keyboardDevice
                        buttons: [Qt.Key_Right]
                        scale: 1.0
                    }
                ]
                onValueChanged: {
                    console.log("keyboard axis value", value);
                }
            }
        ]
    }
}
