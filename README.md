# Linux Kernel Module Development
Dependencies, configurations and project files for Linux Kernel Module Development journey.

## Development Environment
### Basic Setup
Kernel headers and build tools are needed to compile kernel modules. This guide will detail instructions for Arch Linux. You can also follow your distro-specific instructions to install these requirements:
```
pacman -Syu base-devel linux-headers
```
That's it! You are ready to compile your own kernel modules!

### Mainline Setup
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

Then compile the Linux kernel with all the available threads:
```
make -j$(nproc)
```

Compile the necessary kernel modules against the newly built kernel:
```
make -j$(nproc) modules
```

Now install the modules to your system, you need admin rights to do that:
```
sudo make modules_install
```

And you can install the kernel at last:
```
sudo make install
```

Congratulations! You have configured, built and installed your own kernel. You are now ready to build your own kernel modules.

### Distro-Patched Stable-Branch Setup
Instead of Linus' mainline branch, you can acquire [Greg Kroah-Hartman's stable-branch mirror](https://github.com/gregkh/linux). This branch has minor version tags which gets the security updates on top of mainline-branch.

Better yet, you can get to your distro's kernel repository, which are usually based on stable-branch kernel. These branches get distro-specific patches optimized to your distro's use case, on top of official security updates. This setup will offer more security and more compatibility with your current system. We will use latest release tag of [Arch Linux kernel sources, with patches](https://github.com/archlinux/linux).

Almost every distro also customizes the kernel configuration depending on their use case. So, we can skip all the hassle of configuration and get the current kernel config using `procfs` from running system and write it into `.config` file with following command. Just make sure to run it inside cloned kernel repository directory:
```
zcat /proc/config.gz > .config
```

If config changed between versions, it's still necessary to manually configure or run `olddefconfig` over previous command. Other than that, it's all a similar process.

## Compiler Support

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
