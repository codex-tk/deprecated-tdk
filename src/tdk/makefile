CXX=g++
SRCS=time/ctime.cpp time/time_span.cpp time/tick.cpp time/date_time.cpp tdk.cpp \
    threading/thread_local.cpp threading/thread_data.cpp \
    error/error_category_platform.cpp error/error_category_tdk.cpp \
    error/error_platform.cpp error/error_tdk.cpp error/last_error.cpp \

OBJS=$(SRCS:%.cpp=%.o)
INC=-I./../ -I./
AR=ar
RANLIB=ranlib

CPPFLAGS=-std=c++11 $(INC)

TARGET=libtdk.a

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $(OBJS)
	$(RANLIB) $@

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
