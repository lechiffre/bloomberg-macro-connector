cd build
cmake3 ..
cmake3 --build .
cmake3 --install .
cd ../swig
make
cd ../go
rm bin/*
sh install.sh
cd ..
