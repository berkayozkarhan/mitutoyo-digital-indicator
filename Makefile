
PROJECT_ROOT_DIR  =	$(shell pwd)
PROJECT_SRC_DIR   = $(PROJECT_ROOT_DIR)/src
PROJECT_APP_DIR   = $(PROJECT_ROOT_DIR)/app

PROJECT_BUILD_DIR = $(PROJECT_ROOT_DIR)/build


all: prepare
	cd $(PROJECT_SRC_DIR) && make all
	cd $(PROJECT_APP_DIR) && make all

prepare:
	mkdir -p $(PROJECT_BUILD_DIR)

clean:
	rm -rf $(PROJECT_BUILD_DIR)