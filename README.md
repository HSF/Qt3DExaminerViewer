# Qt3DExaminerViewer
A viewer module which offers manipulation and visualization functionalities and tools similar to the Open Inventor / Coin Examiner viewer


## How to build

open `Qt3DExaminerViewer.pro` file with QtCreator, click `run` button then it will compile automatically cross platform.

## current functionalities

1.
- info window to show information of volumes and tips
- change visibility for detector volume.
2.
- specify the camera position by radius, latitude and longitude (I built a spherical coordinate system centered on dectecter) 
- Note, moving the camera forward and backward along radius is equivalent to zoom in and out.
- specify the camera direction by roll, yaw, pitch (I used Tait–Bryan angles convention).
3.
- three predefined view: top, left, front view
- cancel selection and restore all volumes by click "revert original state", should be replaced by clicking empty space.
4.
- switch between perspective and orthographic projection mode for camera.
- switch between select and view mode for mouse:  disable moving camera by mouse after opening "select" mode, disable selecting volume by mouse after opening "view" mode 
5.
- inaccurate picking support: left click volume to select, change its color to red after being selected.
- open volume support: Cmd/Ctrl + left click to hide clicked volume and display its children.
- close volume support: Option/Alt + left click to hide clicked volume an display its parent.
6.
- control camera position/direction, mapping relationpship as table below

| input | action |
| - | - |
| Left mouse button | While the left mouse button is pressed, mouse movement along x-axis moves the camera left and right and movement along y-axis moves it up and down.|
| Right mouse button | While the right mouse button is pressed, mouse movement along x-axis pans the camera around the camera view center and movement along y-axis tilts it around the camera view center. |
| Both left and right mouse button | While both the left and the right mouse button are pressed, mouse movement along y-axis zooms the camera in and out without changing the view center. |
| Arrow keys | Move the camera vertically and horizontally relative to camera viewport.|
| Page up and page down keys | Move the camera forwards and backwards. |
| Shift key | Changes the behavior of the up and down arrow keys to zoom the camera in and out without changing the view center. The other movement keys are disabled. | 
| Alt key | Changes the behovior of the arrow keys to pan and tilt the camera around the view center. Disables the page up and page down keys.|

## current UI
- control camera position and direction with slider/spinbox/mouse or touchpad(in view mode)

![spinbox](https://firebasestorage.googleapis.com/v0/b/steam-key-269816.appspot.com/o/Screenshot%202020-06-26%20at%2023.42.21.png?alt=media&token=1c0855ce-6e30-4441-9201-b7413b74dce2)

