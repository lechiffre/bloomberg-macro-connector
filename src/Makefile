CXXFLAGS = -Wall -g -std=c++14 -I ../include -fPIE
LIBNAME = libblpconn.a
OBJS = timeutils.o \
	logger.o \
	connection.o \
	eventhandler.o \
	subscription.o 

all: $(OBJS)
	ar rcs $(LIBNAME) $(OBJS)
	mv $(LIBNAME) ../lib

clean:
	rm -f $(OBJS)
