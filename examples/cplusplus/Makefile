CXXFLAGS = -std=c++14 -g -Wall -Werror -I ../../include
LDLIBS = -lblpapi3_64

all: bpipe-connection

bpipe-connection: eventhandler.o

clean:
	rm -f bpipe-connection
	rm -f *.o
