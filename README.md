# Linux Kernel Module Development
Dependencies, configurations and project files for Linux Kernel Module Development journey.

## Instructions
Kernel headers and build tools are needed to compile kernel modules. This guide will detail instructions for Arch Linux. You can also follow your distro-specific instructions to install these requirements:
```
pacman -Syu base-devel linux-headers
```

Although the headers are enough to successfuly build the module, it is recommended to configure and build your own kernel to help with our specific workflow needs. This repository includes reference to [Linus Torvalds's mainline Linux mirror](https://github.com/torvalds/linux) as submodule. I try to keep it as up-to-date as possible by bumping it to latest release tag. You can follow the instructions to build and install the mainline kernel.

Following packages are needed to build your own kernel:
```
pacman -Syu xmlto kmod inetutils bc libelf git cpio perl tar xz 
```

Clone this repository with `--recursive` argument to also clone aforementioned repository:
```
git clone --recursive git@github.com:Lephar/Kernel.git
```

Change into Linux directory:
```
cd Kernel/linux
```

Configure the kernel to your taste using one of the following with `make` command: `config`, `menuconfig`, `nconfig`, `gconfig`, `xconfig`. Or you can simply use the default configuration, which is sufficient for most use cases:
```
make defconfig
```

When you fetch the updated source code and some new entries are added to the configuration, you can preserve the old config and set the default values to new configuration options with the following command:
```
make olddefconfig
```

As an alternative way, 

Then compile the Linux kernel with all the available threads:
```
make -j$(nproc)
```

Congratulations! You have configured and built your own kernel! To install it to your system:
```
sudo make install
```


This kernel usually enough for us.  To 

For security updates and more compatibility with your distro you can also use [Greg Kroah-Hartman's Linux stable branch](https://github.com/gregkh/linux) and apply your 

Kernel source code can be downloaded from [The Linux Kernel Archives](https://kernel.org/)

Distro-specific patches can be acquired from their respective repositories, e.g. [Arch Linux kernel sources, with patches](https://github.com/archlinux/linux/releases)

To apply the patch, execute the following command in the source directory after copying the .patch file there (some modification of paths may be necessary on .patch file):
```
patch -ruN -p0 < linux-<VERSION>.patch
```

Current kernel config can be copied from the running system using the following command:
```
zcat /proc/config.gz > .config
```

If using Clang, compiler-specific config can be added to .config file using the following command:
```
make CC=clang olddefconfig
```

To compile the kernel on multiple threads with the created config using Clang compiler:
```
make CC=clang -j$(nproc)
```

To use code assist with `clangd` language server, `compile_commands.json` can be created using the following script:
```
scripts/clang-tools/gen_compile_commands.py
```

Copy the created `compile_commands.json` file into the desired project directory (e.g. [Hello World Example](HelloWorld)) and you are good to go!

Projects can be built using `Makefile` in the project folder:
```
make all
```

To insert compiled module into the kernel:
```
sudo insmod <MODULE_NAME>.ko
```

To remove inserted module from the kernel:
```
sudo rmmod <MODULE_NAME>
```

Kernel module output can be accessed on system logs using `journalctl`:
```
journalctl -f
```

Project files can be cleaned using `Makefile`:
```
make clean
```
