# Qt3DExaminerViewer

A viewer module which offers manipulation and visualization functionalities and tools similar to the Open Inventor / Coin Examiner viewer. It reads geometry data from local database and render the shape for users. A strong focus is to simpify the effort to inspect a particle detector such as ATLAS.

This repository is reserved for Google Summer of Code 2020 project, hosted by CERN-HSF. Thanks for giving the chance for me. 
For more thorough documentation, please refer to my [GSoC report](https://docs.google.com/document/d/10I00Gz-JkWyfkBxf3iAYtOhVq7QwVbnep8SsJHSZdho/edit?usp=sharing). 

## How to build
You need to firstly install [qt5.15](https://www.qt.io/download-open-source), [GeoModelIO, GeoModelCore](https://gitlab.cern.ch/GeoModelDev/GeoModel) as dependencies of this project.

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
Then select one of the example database file from resourse/db folder.

## current functionalities

### 1. about volume inspection:
- info window to show information of volumes and tips
- change visibility of detector volume.
- switch between select and view mode for mouse:  disable moving camera by mouse after opening "select" mode, disable selecting volume by mouse after opening "view" mode 
- accurate picking/selection support: left click volume to select, the selected volume change its color to dard red and info window will display the information of this volume.
- cancel selection: click empty space or press "X" key
- open volume : Cmd/Ctrl + left click to hide clicked volume and display its child/children.
- close volume : Option/Alt + left click to hide clicked volume and display its parent.

### 2. about viewport:
- switch between perspective and orthographic projection.
- three predefined view and "ViewAll" button: top, left, front view
- bookmark user selected view to a list, quickly return to where user bookmarked before.
- [two automated tours](https://youtu.be/dYyYomakvLE)

### 3. about input-action mapping 
- control camera postion in terms of zoom in/out, tranlsation, rotation

| input | action |
| ---- | ---- |
| Left mouse button<br>Arrow keys | translate the camera position and camera view center,<br>the view direction keeps same.|
| Right mouse button<br>Control(on Mac) key + left mouse button<br>Alt/Option key + up/down keys | rotate the camera position around the camera view center,<br>the view center keeps same.|
| scroll wheel<br>Page up/Page down keys<br>Fn + up/down keys | Move the camera forwards and backwards. |
| clicking Both left and right mouse button and move mouse | zooms the camera in and out without changing the view center. |
## current UI

![spinbox](https://firebasestorage.googleapis.com/v0/b/steam-key-269816.appspot.com/o/Screenshot%202020-08-28%20at%2000.10.02.png?alt=media&token=29833747-f545-463d-8c91-e40f419f30d9)

