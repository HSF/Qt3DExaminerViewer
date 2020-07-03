# Qt3DExaminerViewer
A viewer module which offers manipulation and visualization functionalities and tools similar to the Open Inventor / Coin Examiner viewer


## How to build

open `Qt3DExaminerViewer.pro` file with QtCreator, click `run` button on the left bottom corner then it will compile automatically cross platform.

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
| Left mouse button<br>Arrow keys | while long pressing left mouse button, mouse movement translate the camera position <br> and camera view center, the view direction keeps same.|
| Right mouse button<br>Control key + left mouse button<br>Alt/Option key + up/down Arrow keys | While long pressing right mouse button, mouse movement rotate the camera position <br> around the camera view center, the view center keeps same.|
| scroll wheel<br>Page up and page down keys<br>Fn + up/down Arrow keys | scroll wheel of mouse zooms the camera in and out without changing the view center <br> and view direction. |
## current UI
- control camera position and direction with slider/spinbox/keyboard arrow keys or touchpad/mouse(in view mode)

![spinbox](https://firebasestorage.googleapis.com/v0/b/steam-key-269816.appspot.com/o/Screenshot%202020-07-03%20at%2023.59.31.png?alt=media&token=195f9f81-8f37-4560-9854-7f50f6e00a81)

