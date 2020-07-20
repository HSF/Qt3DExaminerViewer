
# Building the GeoModelIO::GeoModelRead library

## Dependencies

### Install Qt5

See: <https://doc.qt.io/qt-5/gettingstarted.html>

On macOS, you can use `brew`:

```bash
brew install qt5
```

After the installation, pay attention to add the Qt folder to your PATH (replace <path-to-qt> with your Qt installation folder and 'clang_64' with ):

```bash
PATHQT=/<path-to-qt>/Qt5.12.0/5.12.0/clang_64;
export PATH=$PATHQT/bin:$PATH;
```

On some platforms, you might need to explicitly set the QMAKESPEC and the QT variables:

```bash
export QMAKESPEC=$PATHQT/mkspecs/macx-clang;
export QTDIR=$PATHQT;
export QTINC=$PATHQT/include;
export QTLIB=$PATHQT/lib
```


### Build GeoModelIO dependencies

#### Build GeoModelKernel

**NOTE!** Here, we are currently using a development branch.

```bash
git clone --recurse-submodules ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelKernel.git
cd GeoModelKernel
git checkout master-geomodel-standalone-cmake
cd ../
mkdir build_gmk
cd build_gmk
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelKernel
make -j 4
make install
```

#### Build GeoModelIO::GeoModelDBManager

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO/GeoModelDBManager.git
mkdir build_gmdb
cd build_gmdb
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelDBManager/
make -j 4
make install
```

#### Build GeoModelIO::TFPersistification

```bash
git clone
mkdir TFPersistification_build
cd TFPersistification_build
cmake ../TFPersistification -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=../install
make -j10
make install
```

## Build GeoModelIO::GeoModelRead

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO/GeoModelRead.git
mkdir build_gmread
cd build_gmread
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelRead
make -j 4
make install
```
