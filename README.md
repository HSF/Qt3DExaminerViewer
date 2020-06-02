# Qt3DExaminerViewer
A viewer module which offers manipulation and visualization functionalities and tools similar to the Open Inventor / Coin Examiner viewer


## How to build

open `Qt3DExaminerViewer.pro` file with QtCreator, click `run` button then it will compile automatically cross platform.

## current functionalities

- switch between perspective and orthographic camera projection mode
- change visibility of entire volume
-  rotate and scale volume 
- control camera/the view of observation. mapping relationpship as table below

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
- perspective projection mode
![perspective](https://firebasestorage.googleapis.com/v0/b/steam-key-269816.appspot.com/o/Screenshot%202020-06-02%20at%2010.09.32.png?alt=media&token=f6a1147d-fcff-43e2-a37d-86fc55c767c1)


- orthographic projection mode
![orthographic](https://firebasestorage.googleapis.com/v0/b/steam-key-269816.appspot.com/o/Screenshot%202020-06-02%20at%2010.13.19.png?alt=media&token=e7cbd7c3-8d56-43d4-82c0-9a9bd65f4352)
