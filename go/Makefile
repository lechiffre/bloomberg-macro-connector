CXXFLAGS = -Wall -g -std=c++14 -fPIE
INC = -Wall -I../include -I../blpapi3_64/blpapi_cpp_3.25.3.1/include/

all:
	swig $(INC) -c++ -go -cgo -intgosize 64 blpconn.i
	$(CXX) -c $(CXXFLAGS) $(INC) blpconn_wrap.cxx	 
	ar rcs libblpconngo.a blpconn_wrap.o
	go doc -all > API.txt

clean:
	rm -f *.o *.a
	rm -f *.go
	rm -f *.cxx



