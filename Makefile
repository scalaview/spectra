
BIN_DIR = ./bin
OS_BIN = ./bin/os.bin
DISK_ORIGIN_IMG = ./disk.img
TMP_DIR = /mnt/d
FILES_DIR = ./files/data
KERNEL_PATH = ./kernel
GRUB_CONF_PATH = $(KERNEL_PATH)/src/grub.cfg
KERNEL_BIN	= $(KERNEL_PATH)/bin/kernel.elf
APPS_PATH	= ./programs/apps/
LIB_PATH	= ./programs/stdlib

APP_BIN_FILES = $(APPS_PATH)/start/bin/start.bin
LIB_FILES 	= $(LIB_PATH)/bin/stdlib.elf

dir:
	mkdir -p $(BIN_DIR)

all: dir $(KERNEL_BIN) user_apps disk

disk:
ifneq ("$(wildcard $(DISK_ORIGIN_IMG))","")
	@printf "\e[32;1mMaking $@\e[0m\n"
	sudo mkdir -p $(TMP_DIR)
	cp $(DISK_ORIGIN_IMG) $(OS_BIN)
	sudo mount -o loop,offset=1048576 $(OS_BIN) $(TMP_DIR)
	sudo mkdir -p $(TMP_DIR)/boot/grub
	sudo mkdir -p $(TMP_DIR)/usr/bin
	sudo mkdir -p $(TMP_DIR)/usr/lib
	sudo cp $(KERNEL_BIN) $(TMP_DIR)/boot
	sudo cp $(GRUB_CONF_PATH) $(TMP_DIR)/boot/grub
	sudo cp -r $(FILES_DIR) $(TMP_DIR)/
	sudo cp $(APP_BIN_FILES) $(TMP_DIR)/usr/bin
	sudo cp $(LIB_FILES) $(TMP_DIR)/usr/lib
	sync
	sudo umount $(TMP_DIR)
else
	echo "Making $(DISK_ORIGIN_IMG) file first."
endif

$(KERNEL_BIN):
	@printf "\e[32;1mMaking Kernel\e[0m $@\n"
	make -C $(KERNEL_PATH) clean
	make -C $(KERNEL_PATH) all

user_apps:
	@printf "\e[32;1mMaking apps\e[0m $@\n"
	make -C $(APPS_PATH)/start clean
	make -C $(APPS_PATH)/start all

clean:
	make -C $(KERNEL_PATH) clean
	make -C $(APPS_PATH)/start clean
	make -C $(LIB_PATH) clean
	rm -rf $(BIN_DIR)

