ifdef DEBUG
STRIP=-strip-all
endif

HOST=example_host
LIBARCHIVE=libhirt.a
INC_PATH=include
SRC_PATH=src
EXAMPLE_PATH=example
OUT_DIR=output
#OPTIMIZE=-O3

INCLUDES=\
	-I$(INC_PATH) \
	-I$(SRC_PATH)

CC = $(CROSS_COMPILE)gcc
CX = $(CROSS_COMPILE)g++
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)ld
NM = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

CCFLAGS=$(OPTIMIZE) $(INCLUDES)
CXFLAGS=$(OPTIMIZE) $(INCLUDES)
ASFLAGS=$(INCLUDES)
#LDFLAGS=-nostartfiles -nostdlib -T $(LDS_HIKL)
ARFLAGS=-rcs

HOST_OBJS=\
	$(OUT_DIR)/example_host.o

OBJS=\
	$(OUT_DIR)/example1.o \
	$(OUT_DIR)/example2.o

all: dir $(HOST)

rebuild: clean all

dir:
	mkdir -p $(OUT_DIR)

$(HOST): $(HOST_OBJS) $(LIBARCHIVE)
	$(LD) $(HOST_OBJS) $(LIBARCHIVE) -o $(HOST)

$(LIBARCHIVE): $(OBJS)
	$(AR) $(ARFLAGS) $(LIBARCHIVE) $^

#example rules
$(OUT_DIR)/example_host.o: $(EXAMPLE_PATH)/example_host.c
	$(CC) -c $(CCFLAGS) $< -o $@ 

$(OUT_DIR)/libhirt.o: $(SRC_PATH)/libhirt.c
	$(CC) -c $(CCFLAGS) $< -o $@ 

$(OUT_DIR)/libhirt_cmdqueue.o: $(SRC_PATH)/libhirt_cmdqueue.cpp
	$(CX) -c $(CXFLAGS) $< -o $@

$(OUT_DIR)/libhirt_port_os.o: $(SRC_PATH)/libhirt_port_os.c
	$(CC) -c $(CCFLAGS) $< -o $@ 

$(OUT_DIR)/libhirt_scheduler.o: $(SRC_PATH)/libhirt_scheduler.cpp
	$(CX) -c $(CXFLAGS) $< -o $@


clean:
	rm -rf $(OUT_DIR)/*

distclean:
	rm -rf $(OUT_DIR)/*
	rm -rf $(OUT_DIR)
