SUB_DIRS=tdk test.tdk.linux

MAKE = make

subdirs:
	@for dir in $(SUB_DIRS); do\
        $(MAKE) -C $$dir all;\
	    if [ $$? != 0 ]; then exit 1; fi;\
	done
   
all: subdirs

dependall:
	@for dir in $(SUB_DIRS); do\
        $(MAKE) -C $$dir depend;\
	    if [ $$? != 0 ]; then exit 1; fi;\
	done

cleanall:
	@for dir in $(SUB_DIRS); do\
        $(MAKE) -C $$dir clean;\
	    if [ $$? != 0 ]; then exit 1; fi;\
	done

