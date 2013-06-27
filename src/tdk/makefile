CXX=g++ 
INC=-I/home/tk/develop/tdk/src
CPPFLAGS=-std=c++11 $(INC)
OBJECTS=util/rtti.o threading/atomic/atomic_linux.o
OBJS_DIR=obj

TARGET=libtdk.a

#.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rvs $@ $(OBJECTS)

clean:
	rm -rf $(OBJECTS)
