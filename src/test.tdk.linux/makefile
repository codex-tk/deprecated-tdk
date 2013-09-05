CXX=g++
AR=ar
RANLIB=ranlib
RM=rm
MKDIR=mkdir -p
DEPEND_FILE=depend

ifeq ( $(RELEASE),1)
OBJS_DIR  = ../build/release
DBG_FLAGS = -DNDEBUG 
else
OBJS_DIR  = ../build/debug
DBG_FLAGS = -DDEBUG -g
endif

TARGET_NAME=test.tdk.linux
TARGET_OBJS_DIR=$(OBJS_DIR)/$(TARGET_NAME)
TARGET_CPPFLAGS=-std=c++11 -I./ -I./../ -I./../gtest/include $(DBG_FLAGS) 
TARGET_LDFLAGS= -lpthread -L./../gtest -lgtest -L$(OBJS_DIR)/tdk -ltdk -lrt
TARGET_DEPEND_FILE = $(TARGET_OBJS_DIR)/$(DEPEND_FILE)
TARGET_SRCS= main.cpp test_tdk_date_time.cpp \
        test_tdk_error.cpp \
        test_tdk_loop.cpp

TARGET_OBJS=$(TARGET_SRCS:%.cpp=$(TARGET_OBJS_DIR)/%.o)

TARGET=$(TARGET_OBJS_DIR)/$(TARGET_NAME).out

$(TARGET_OBJS_DIR)/%.o : %.cpp
	@echo "=====Compile=$(dir $@)"
	$`[ -d $(dir $@) ] || $(MKDIR) $(dir $@)
	$(CXX) $(TARGET_CPPFLAGS) -c $< -o $@


all: $(TARGET) $(OBJS_DIR)/tdk/libtdk.a

$(TARGET): $(TARGET_OBJS)
	@echo "=====Link=$(TARGET)"
	$(CXX) -o $@ $(TARGET_OBJS) $(TARGET_LDFLAGS) $(TARGET_CPPFLAGS)
	@echo ""

clean:
	rm -rf $(TARGET_OBJS) $(TARGET)   

depend:
	$'[ -d $(TARGET_OBJS_DIR) ] || $(MKDIR) $(TARGET_OBJS_DIR)
	$(RM) -f $(TARGET_DEPEND_FILE)
	@for FILE in $(TARGET_SRCS:%.cpp=%); do\
		$(CXX) -MM -MT $(TARGET_OBJS_DIR)/$$FILE.o $$FILE.cpp $(TARGET_CPPFLAGS) >> $(TARGET_DEPEND_FILE); \
	done



ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SRCS),)
-include $(TARGET_DEPEND_FILE) 
endif
endif
endif
