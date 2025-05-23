CXXFLAGS = -Wall -g -std=c++14 -fPIE -DDEBUG
INC = -Wall -I../include -I../blpapi3_64/blpapi_cpp_3.25.3.1/include/
NAME = blpconngo

all:
	swig $(INC) -c++ -go -cgo -intgosize 64 $(NAME).i
	$(CXX) -c $(CXXFLAGS) $(INC) $(NAME)_wrap.cxx	 
	ar rcs lib$(NAME).a $(NAME)_wrap.o
	go doc -all > ../docs/go-api.txt
	mv lib$(NAME).a ../lib
	mv $(NAME).go ../go

clean:
	rm -f *.o *.a
	rm -f *.go
	rm -f *.cxx



