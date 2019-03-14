# https://github.com/python-cmake-buildsystem/python-cmake-buildsystem

cd ~/scratch
git clone git://github.com/python-cmake-buildsystem/python-cmake-buildsystem

cd ~/scratch
mkdir -p python-build && mkdir -p python-install
cd python-build
cmake -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/scratch/python-install ../python-cmake-buildsystem
make -j10
make install
