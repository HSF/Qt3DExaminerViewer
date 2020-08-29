# Qt3DExaminerViewer

**GSoC participant**: Huajian Qiu (EPFL, Lausanne, Switzerland)

**CERN-HSF mentor**: Dr. Riccardo Maria Bianchi (ATLAS, CERN & University of Pittsburgh)

This repository is reserved for Google Summer of Code 2020 project, hosted by CERN-HSF. Thanks for giving the chance for me. 
For more thorough documentation, please refer to my [final GSoC report](https://docs.google.com/document/d/10I00Gz-JkWyfkBxf3iAYtOhVq7QwVbnep8SsJHSZdho/edit?usp=sharing), [presentation slides](https://docs.google.com/presentation/d/10ut0qlqdP9e3BUP_aFZ-F3chzL1kFoLcUwCfs8jjQ3k/edit?usp=sharing), [application proposal](https://docs.google.com/document/d/1PuiWyJuALC7vUnVg-wExRN6eULU-tJmclNQYqueXAWI/edit?usp=sharing). 

## Introduction 
GeoExaminerViewer is a general viewer GUI module that offers manipulation and visualization functionalities similar to the Open Inventor / Coin Examiner viewer. 

- It reads geometry data and its associated information from local SQL databases and builds the basic geometry component shapes (like boxes, toruses, cones, and so forth) according to geometry parameters.
- It offers a flexible way to inspect huge amounts of geometry components and the parent-children hierarchy between them. 
- It lets users select geometry components and see the pieces of data associated with them.

This viewer is particularly useful for inspecting the geometry structure of large, complex, scientific apparatuses,  such as the high energy physics detector ATLAS running at CERN. 

## Motivation
The ATLAS  community already built some Athena visualization tools (VP1/VP1Light/GMEX), which use a stack of different software libraries: 
```
Qt → GUI
Coin/OpenInventor → 3D graphics engine
SoQt → Glue package between Coin and the Qt GUI
```
The above show several drawbacks:
- Coin/OpenInventor, even if recently revamped by a small but quite active open source community (mainly from the robotics R&D field), is old-ish technology and lacks modern graphics features
- SoQt is the weakest link: it handles the low-level graphics calls to the native windowing system, which easily breaks when new OS or compiler versions are out.

In this GSoC project, my work is to replace the usage of Coin/OpenInventor and SoQt by Qt3D, which is a newly released, well-supported 3D engine. Being Qt3D part of the Qt framework–which is used for the GUI anyway–, the use of that instead of Coin+SoQt would help to simplify the software stack too, by eliminating a layer of complexity.

However many high-level functionalities are missed in Qt3D, compared to Coin3D.  Therefore, before using Qt3D to effectively render geometry data and interact with that, many software tools had to be designed and implemented.  Thus, I had to reimplement many of the most used inspection tools based on the Qt3D API and classes; I also  designed some new features after discussions between my mentor and me.  



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

