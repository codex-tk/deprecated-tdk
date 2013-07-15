CXX=g++
AR=ar
RANLIB=ranlib
RM=rm
MKDIR=mkdir -p

ifeq ( $(RELEASE),1)
OBJS_DIR  = build/release
DBG_FLAGS = -DNDEBUG
else
OBJS_DIR  = build/debug
DBG_FLAGS =-DDEBUG
endif

DEPEND_FILE = $(OBJS_DIR)/depend_file

TARGET_SRCS=tdk.cpp \
    alloc.cpp \
    time/ctime.cpp \
    time/time_span.cpp \
    time/tick.cpp \
    time/date_time.cpp \
    \
    threading/thread_local.cpp \
    threading/thread_data.cpp  \
    threading/spin_lock.cpp \
    \
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
    \
    io/ip/address.cpp\
    io/ip/socket.cpp\
    \
    io/eventfd.cpp \
    io/engine_epoll.cpp \
    io/operation.cpp \
    io/ip/tcp/socket_epoll.cpp \
    io/ip/tcp/operation/connect_operation_epoll.cpp


TARGET_OBJS=$(TARGET_SRCS:%.cpp=$(OBJS_DIR)/%.o)
CPPFLAGS=-std=c++11 -I./../ -I./
TARGET=$(OBJS_DIR)/libtdk.a

#.SUFFIXES: .cpp .o

$(OBJS_DIR)/%.o : %.cpp
	@echo "=====Compile=$(dir $@)"
	$`[ -d $(dir $@) ] || $(MKDIR) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@
	@echo ""

all: $(TARGET)

$(TARGET): $(TARGET_OBJS)
	$(AR) rvs $@ $(TARGET_OBJS)
	$(RANLIB) $@

clean:
	rm -rf $(TARGET_OBJS) $(TARGET)

depend:
	$'[ -d $(OBJS_DIR) ] || $(MKDIR) $(OBJS_DIR)
	$(RM) -f $(DEPEND_FILE)
	@for FILE in $(TARGET_SRCS:%.cpp=%); do\
		$(CXX) -MM -MT $(OBJS_DIR)/$$FILE.o $$FILE.cpp $(CPPFLAGS) >> $(DEPEND_FILE); \
	done

echo:
	echo $(OBJS)



ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SRCS),)
-include @(DEPEND_FILE) 
endif
endif
endif
