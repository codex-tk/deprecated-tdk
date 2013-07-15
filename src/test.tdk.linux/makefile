CXX=g++
SRCS=   main.cpp test_tdk_date_time.cpp \
        test_tdk_error.cpp \
        test_tdk_io_ip_address.cpp \
        test_tdk_io_socket.cpp \
        test_tdk_io_engine_epoll.cpp \
        test_epoll_multithread.cpp 

OBJS=$(SRCS:%.cpp=%.o)
INCLUDES=-I./../ -I./ -I/home/tk/develop/gtest-1.6.0/include
LDFLAGS=-lpthread -L./../gtest -lgtest -L./../tdk/build/debug -ltdk -lrt
CPPFLAGS=-std=c++11 $(INCLUDES) -g
TARGET=test.tdk.linux

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS) ./../tdk/build/debug/libtdk.a
	$(CXX) -g -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS) $(TARGET) depend_file

depend:
	$(CXX) -M $(CPPFLAGS) $(SRCS) > depend_file


ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SRCS),)
-include depend_file
endif
endif
endif
