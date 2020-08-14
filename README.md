# Qt3DExaminerViewer
A viewer module which offers manipulation and visualization functionalities and tools similar to the Open Inventor / Coin Examiner viewer


## How to build
You need to firstly install [qt5.15, qmake](https://www.qt.io/download-open-source), [GeoModelIO, GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModel) as dependencies of this project.

From IDE QtCreator:


open `Qt3DExaminerViewer.pro` file with QtCreator, click `run` button on the left bottom corner then it will compile automatically cross platform.

From terminal:

```
git clone https://github.com/HSF/Qt3DExaminerViewer.git
cd Qt3DExaminerViewer
qmake -o build/Makefile Qt3DExaminerViewer.pro
cd build
make -j4 
open Qt3DExaminerViewer.app
```
Then select one of the database file from resourse/db folder when OpenFileDialog appears.

## current functionalities

### 1. about volume control
- info window to show information of volumes and tips
- change visibility of detector volume.
- switch between select and view mode for mouse:  disable moving camera by mouse after opening "select" mode, disable selecting volume by mouse after opening "view" mode 
- inaccurate picking support: left click volume to select, change its color to red after being selected.
- cancel selection and restore all volumes by click "revert original state", should be replaced by clicking empty space.
- open volume support: Cmd/Ctrl + left click to hide clicked volume and display its children.
- close volume support: Option/Alt + left click to hide clicked volume an display its parent.

### 2. about camera control
- switch between perspective and orthographic projection mode for camera.
- specify the camera position by radius, latitude and longitude (I built a spherical coordinate system centered on camera view center, which is detecter volume center by default) 
- Note, moving the camera forward and backward along radius is equivalent to zoom in and out.
- specify the camera direction by roll, yaw, pitch (I used Tait–Bryan angles convention).
- three predefined view: top, left, front view
- one predefined view tour

### 3. about input-action 
- control camera postion in terms of zoom in/out, tranlsation, rotation

| input | action |
| ---- | ---- |
| Left mouse button<br>Arrow keys | translate the camera position and camera view center,<br>the view direction keeps same.|
| Right mouse button<br>Control(on Mac) key + left mouse button<br>Alt/Option key + up/down keys | rotate the camera position around the camera view center,<br>the view center keeps same.|
| scroll wheel<br>Page up/Page down keys<br>Fn + up/down keys | zoom the camera in and out,<br>the view center and view direction keep same. |
## current UI
- control camera position and direction with slider/spinbox/keyboard arrow keys or touchpad/mouse(in view mode)

![spinbox](https://firebasestorage.googleapis.com/v0/b/steam-key-269816.appspot.com/o/Screenshot%202020-08-14%20at%2011.02.13.png?alt=media&token=9ff49d4e-7ffc-4022-9aaa-7f169262891c)

