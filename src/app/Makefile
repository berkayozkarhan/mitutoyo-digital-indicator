MODULE_NAME = app

PROJECT_ROOT_DIR = ../..
PROJECT_BUILD_DIR = $(PROJECT_ROOT_DIR)/build

MODULE_DIR = $(PROJECT_BUILD_DIR)/$(MODULE_NAME)
MODULE_OBJ_DIR = $(MODULE_DIR)/obj
MODULE_LIB_DIR = $(MODULE_DIR)/lib/static


TARGET = $(MODULE_LIB_DIR)/lib$(MODULE_NAME).a
MODULE_OBJ_1 = $(MODULE_OBJ_DIR)/$(MODULE_NAME).o
MODULE_SRC_1 = $(MODULE_NAME).c

MODULE_INCLUDE_DIR = include

# libusb static library
LIBUSB_OBJ = /usr/lib/x86_64-linux-gnu/libusb-1.0.a

CFLAGS = -Wall -Werror
INCLUDE_FLAGS = -I$(MODULE_INCLUDE_DIR)
LINKER_FLAGS = -lusb-1.0
CC = /usr/bin/gcc
AR = /usr/bin/ar
AR_PARAMETERS = rcs

module: prepare $(TARGET)

$(TARGET): $(MODULE_OBJ_1)
	$(AR) $(AR_PARAMETERS) $@ $<

$(MODULE_OBJ_1): $(MODULE_SRC_1)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@


prepare:
	mkdir -p $(MODULE_DIR) $(MODULE_OBJ_DIR) $(MODULE_LIB_DIR)

clean:
	rm -rf $(MODULE_DIR)