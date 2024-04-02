# Linux Kernel Module Development
Dependencies, configurations and project files for Linux Kernel Module Development journey.

## Instructions
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
