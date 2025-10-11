cd build
cmake ..
cmake --build .
cmake --install .
cd ../swig
make
cd ../go
rm bin/*
sh install.sh
cd ..
