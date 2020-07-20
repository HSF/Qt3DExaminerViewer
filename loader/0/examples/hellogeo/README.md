

# The 'hellogeo' GeoModel example

The `hellogeo` example shows you how to read persistified GeoModel data in a standalone program.

This example program reads in GeoModel data from a `.db` file, it prints out data entries, it builds the GeoModel tree, it access the RootVolume, and it loops over all its child volumes, printing the name of the GeoLogVol associated with them.

It uses many of the GeoModel packages.

## Build the dependencies

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

#### Build GeoModelIO::GeoModelRead

```bash
git clone ssh://git@gitlab.cern.ch:7999/GeoModelDev/GeoModelIO/GeoModelRead.git
mkdir build_gmread
cd build_gmread
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../GeoModelRead
make -j 4
make install
```


## Build 'hellogeo'

From your work folder:

```bash
cp -r GeoModelRead/examples/hellogeo .
cd hellogeo
mv CMakeLists.txt.example CMakeLists.txt
mv main.cpp.example main.cpp
cd ../
mkdir build_hellogeo
cd build_hellogeo
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../hellogeo
make -j4
make install
```

Then, get sample geometry data to play with:

```bash
wget https://atlas-vp1.web.cern.ch/atlas-vp1/doc_new/sample_datafiles/geometry/geometry-ATLAS-R2-2015-03-01-00.db
ln -s $PWD/geometry-ATLAS-R2-2015-03-01-00.db ../geometry.db
```

Now, you can run the example by typing:

```bash
./hellogeo
```

The example program:

 1. loads the geometry from the `.db` file
 2. prints all GeoMaterial entries found in the `.db`
 3. builds the GeoModel tree, storing it in memory
 4. gets the RootVolume of the GeoModel tree and it prints out the number of its children
 5. loops over all the RootVolume's children volumes (GeoPhysVol and GeoFullPhysVol instances), printing the name of the GeoLogVol associated to them
