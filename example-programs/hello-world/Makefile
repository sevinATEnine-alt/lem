current_dir := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

.PHONY: all build install

build:
	gcc ./hello-world.c -o hello-world

install:
	sudo mkdir -p /usr/local/bin
	sudo ln -s $(current_dir)/hello-world /usr/local/bin/hello-world
