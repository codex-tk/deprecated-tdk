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

TARGET_NAME=tdk
TARGET_OBJS_DIR=$(OBJS_DIR)/$(TARGET_NAME)
TARGET_CPPFLAGS=-std=c++11 -I./ -I./../  $(DBG_FLAGS) 
TARGET_DEPEND_FILE = $(TARGET_OBJS_DIR)/$(DEPEND_FILE)
TARGET_SRCS=tdk.cpp \
alloc.cpp \
task/thread_timer.cpp \
time/ctime.cpp \
time/time_span.cpp \
time/tick.cpp \
time/date_time.cpp \
\
threading/thread_local.cpp \
threading/thread_data.cpp  \
threading/spin_lock.cpp \
\
error/error_category_epoll.cpp \
error/error_category_platform.cpp \
error/error_category_tdk.cpp \
error/error_platform.cpp \
error/error_tdk.cpp \
error/last_error.cpp \
\
log/logger.cpp \
log/record.cpp \
log/category.cpp \
log/formatter/formatter.cpp \
log/formatter/string_formatter.cpp \
log/writer/writer.cpp \
log/writer/stderr_writer.cpp  \
log/writer/file_writer.cpp \
\
buffer/allocator.cpp \
buffer/buffer_base.cpp \
buffer/marker.cpp \
buffer/memory_block.cpp \
\
crypto/sha1.cpp\
crypto/base64.cpp\
crypto/md5.cpp\
crypto/crc32.cpp\
crypto/seed.cpp\
crypto/seed/SEED_KISA.cpp\
crypto/padding.cpp\
\
io/ip/address.cpp\
io/ip/socket.cpp\
\
io/eventfd.cpp \
io/timerfd.cpp \
io/buffer_adapter.cpp \
\
event_loop/task.cpp\
event_loop/io_task.cpp\
event_loop/timer_task.cpp\
event_loop/scheduler.cpp\

TARGET_OBJS=$(TARGET_SRCS:%.cpp=$(TARGET_OBJS_DIR)/%.o)

TARGET=$(TARGET_OBJS_DIR)/lib$(TARGET_NAME).a

$(TARGET_OBJS_DIR)/%.o : %.cpp
	@echo "=====Compile=$(dir $@)"
	$`[ -d $(dir $@) ] || $(MKDIR) $(dir $@)
	$(CXX) $(TARGET_CPPFLAGS) -c $< -o $@


all: $(TARGET)

$(TARGET): $(TARGET_OBJS)
	@echo "=====archive=$(TARGET)"
	$(AR) rvs $@ $(TARGET_OBJS)
	$(RANLIB) $@
	@echo ""

clean:
	rm -rf $(TARGET_OBJS) $(TARGET)   

depend:
	$'[ -d $(TARGET_OBJS_DIR) ] || $(MKDIR) $(TARGET_OBJS_DIR)
	$(RM) -f $(TARGET_DEPEND_FILE)
	@for FILE in $(TARGET_SRCS:%.cpp=%); do\
		$(CXX) -MM -MT $(TARGET_OBJS_DIR)/$$FILE.o $$FILE.cpp $(TARGET_CPPFLAGS) >> $(TARGET_DEPEND_FILE); \
	done

echo:
	echo $(OBJS)


ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SRCS),)
-include $(TARGET_DEPEND_FILE) 
endif
endif
endif
