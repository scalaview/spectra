
BIN_DIR = ./bin
BUILD_DIR = ./build
OS_BIN = ./bin/os.bin
DISK_ORIGIN_IMG = ./disk.img
TMP_DIR = /mnt/d
FILES_DIR = ./files/data
KERNEL_PATH = ./kernel
GRUB_CONF_PATH = $(KERNEL_PATH)/src/grub.cfg
KERNEL_BIN	= $(KERNEL_PATH)/bin/kernel.elf
APPS_PATH	= ./programs/apps
LIB_PATH	= ./programs/stdlib
APP_BIN_PATH = $(BUILD_DIR)/apps
LIB_BUILD_PATH = $(BUILD_DIR)/lib

dir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)/lib
	mkdir -p $(BUILD_DIR)/apps

all: dir $(KERNEL_BIN) user_apps disk

disk:
ifneq ("$(wildcard $(DISK_ORIGIN_IMG))","")
	@printf "\e[32;1mMaking $@\e[0m\n"
	sudo mkdir -p $(TMP_DIR)
	sudo umount -q $(TMP_DIR) || /bin/true
	cp $(DISK_ORIGIN_IMG) $(OS_BIN)
	sudo mount -o loop,offset=1048576 $(OS_BIN) $(TMP_DIR)
	sudo mkdir -p $(TMP_DIR)/boot/grub
	sudo mkdir -p $(TMP_DIR)/usr/bin
	sudo mkdir -p $(TMP_DIR)/usr/lib
	sudo cp $(KERNEL_BIN) $(TMP_DIR)/boot
	sudo cp $(GRUB_CONF_PATH) $(TMP_DIR)/boot/grub
	sudo cp -r $(FILES_DIR) $(TMP_DIR)/
	sudo cp $(APP_BIN_PATH)/* $(TMP_DIR)/usr/bin
	sudo cp $(LIB_BUILD_PATH)/* $(TMP_DIR)/usr/lib
	sync
	sudo umount $(TMP_DIR)
else
	echo "Making $(DISK_ORIGIN_IMG) file first."
endif

$(KERNEL_BIN):
	@printf "\e[32;1mMaking Kernel\e[0m $@\n"
	$(MAKE) -C $(KERNEL_PATH) clean
	$(MAKE) -C $(KERNEL_PATH) all

user_apps:
	@printf "\e[32;1mMaking apps\e[0m $@\n"
	$(MAKE) -C $(APPS_PATH) clean all install
	$(MAKE) -C $(LIB_PATH) clean all install

clean:
	$(MAKE) -C $(KERNEL_PATH) clean
	$(MAKE) -C $(APPS_PATH) clean
	$(MAKE) -C $(LIB_PATH) clean
	rm -rf $(BIN_DIR)
	rm -rf $(BUILD_DIR)

