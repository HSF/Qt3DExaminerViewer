import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12

Entity{
    id: scenceRoot
    Entity{
        components: [
            //CuboidMesh {},
            Mesh {
                id: logoMesh
                source: "qrc:/mesh/left_part.obj"
            },
            DiffuseSpecularMaterial { ambient: "#444"; shininess: 100;},
            Transform { scale: 2 },
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
}
