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

LIB_TDK_OBJS_DIR=$(OBJS_DIR)/tdk
LIB_TDK_CPPFLAGS=-std=c++11 -I./tdk/ -I./ -g
LIB_TDK_DEPEND_FILE = $(LIB_TDK_OBJS_DIR)/depend_file
LIB_TDK_SRCS=tdk/tdk.cpp \
tdk/alloc.cpp \
tdk/time/ctime.cpp \
tdk/time/time_span.cpp \
tdk/time/tick.cpp \
tdk/time/date_time.cpp \
\
tdk/threading/thread_local.cpp \
tdk/threading/thread_data.cpp  \
tdk/threading/spin_lock.cpp \
\
tdk/error/error_category_platform.cpp \
tdk/error/error_category_tdk.cpp \
tdk/error/error_platform.cpp \
tdk/error/error_tdk.cpp \
tdk/error/last_error.cpp \
\
tdk/log/logger.cpp \
tdk/log/record.cpp \
tdk/log/category.cpp \
tdk/log/formatter/formatter.cpp \
tdk/log/formatter/string_formatter.cpp \
tdk/log/writer/writer.cpp \
tdk/log/writer/stderr_writer.cpp  \
\
tdk/buffer/allocator.cpp \
tdk/buffer/buffer_base.cpp \
tdk/buffer/marker.cpp \
tdk/buffer/memory_block.cpp \
\
tdk/crypto/sha1.cpp\
tdk/crypto/base64.cpp\
tdk/crypto/md5.cpp\
tdk/crypto/crc32.cpp\
tdk/crypto/seed.cpp\
tdk/crypto/seed/SEED_KISA.cpp\
\
tdk/io/ip/address.cpp\
tdk/io/ip/socket.cpp\
\
tdk/io/eventfd.cpp \
tdk/io/buffer_adapter.cpp \
tdk/io/engine_epoll.cpp \
tdk/io/operation.cpp \
tdk/io/ip/tcp/socket_epoll.cpp \
tdk/io/ip/tcp/operation/connect_operation_epoll.cpp \
tdk/io/ip/tcp/operation/send_operation_epoll.cpp \
tdk/io/ip/tcp/operation/recv_operation_epoll.cpp




LIB_TDK_OBJS=$(LIB_TDK_SRCS:%.cpp=$(LIB_TDK_OBJS_DIR)/%.o)
LIB_TDK_TARGET=$(LIB_TDK_OBJS_DIR)/libtdk.a

$(LIB_TDK_OBJS_DIR)/%.o : %.cpp
	@echo "=====Compile=$(dir $@)"
	$`[ -d $(dir $@) ] || $(MKDIR) $(dir $@)
	$(CXX) $(LIB_TDK_CPPFLAGS) -c $< -o $@
	@echo ""


TEST_TDK_LINUX_OBJS_DIR = $(OBJS_DIR)/test.tdk.linux
TEST_TDK_LINUX_CPPFLAGS = -std=c++11 -I./test.tdk.linux/ -I./ -I/home/tk/develop/gtest-1.6.0/include -g
TEST_TDK_LINUX_LDFLAGS  = -lpthread -L./gtest -lgtest -L$(LIB_TDK_OBJS_DIR) -ltdk -lrt
TEST_TDK_LINUX_DEPEND_FILE    = $(TEST_TDK_LINUX_OBJS_DIR)/depend_file
TEST_TDK_LINUX_SRCS = test.tdk.linux/main.cpp \
test.tdk.linux/test_tdk_date_time.cpp \
test.tdk.linux/test_tdk_error.cpp \
test.tdk.linux/test_tdk_io_ip_address.cpp \
test.tdk.linux/test_tdk_io_socket.cpp \
test.tdk.linux/test_tdk_io_engine_epoll.cpp \
test.tdk.linux/test_epoll_multithread.cpp 
                      
TEST_TDK_LINUX_OBJS = $(TEST_TDK_LINUX_SRCS:%.cpp=$(TEST_TDK_LINUX_OBJS_DIR)/%.o)                      
TEST_TDK_LINUX      = $(TEST_TDK_LINUX_OBJS_DIR)/test.tdk.linux.app
    
$(TEST_TDK_LINUX_OBJS_DIR)/%.o : %.cpp
	@echo "=====Compile=$(dir $@)"
	$`[ -d $(dir $@) ] || $(MKDIR) $(dir $@)
	$(CXX) $(TEST_TDK_LINUX_CPPFLAGS) -c $< -o $@
	@echo ""

#.SUFFIXES: .cpp .o


all: tdk test

tdk: $(LIB_TDK_TARGET)

test: $(TEST_TDK_LINUX)

$(TEST_TDK_LINUX): tdk  $(TEST_TDK_LINUX_OBJS) $(LIB_TDK_TARGET)
	$(CXX) -g -o $@ $(TEST_TDK_LINUX_OBJS) $(TEST_TDK_LINUX_LDFLAGS)
   

$(LIB_TDK_TARGET): $(LIB_TDK_OBJS)
	$(AR) rvs $@ $(LIB_TDK_OBJS)
	$(RANLIB) $@

clean:
	rm -rf $(LIB_TDK_OBJS) $(LIB_TDK_TARGET) $(TEST_TDK_LINUX_OBJS) $(TEST_TDK_LINUX)
   

depend:
	$'[ -d $(LIB_TDK_OBJS_DIR) ] || $(MKDIR) $(LIB_TDK_OBJS_DIR)
	$(RM) -f $(LIB_TDK_DEPEND_FILE)
	@for FILE in $(LIB_TDK_SRCS:%.cpp=%); do\
		$(CXX) -MM -MT $(LIB_TDK_OBJS_DIR)/$$FILE.o $$FILE.cpp $(LIB_TDK_CPPFLAGS) >> $(LIB_TDK_DEPEND_FILE); \
	done
	$'[ -d $(TEST_TDK_LINUX_OBJS_DIR) ] || $(MKDIR) $(TEST_TDK_LINUX_OBJS_DIR)
	$(RM) -f $(TEST_TDK_LINUX_DEPEND_FILE)
	@for FILE in $(TEST_TDK_LINUX_SRCS:%.cpp=%); do\
		$(CXX) -MM -MT $(TEST_TDK_LINUX_OBJS_DIR)/$$FILE.o $$FILE.cpp $(TEST_TDK_LINUX_CPPFLAGS) >> $(TEST_TDK_LINUX_DEPEND_FILE); \
	done

echo:
	echo $(OBJS)



ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SRCS),)
-include $(LIB_TDK_DEPEND_FILE) 
-include $(TEST_TDK_LINUX_DEPEND_FILE)    
endif
endif
endif
