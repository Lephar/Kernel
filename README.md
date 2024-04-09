# Linux Kernel Module Development
Dependencies, configurations and project files for Linux Kernel Module Development journey.

## Build Dependencies
### Basic Setup
Kernel headers and build tools are needed to compile kernel modules. This guide will detail instructions for Arch Linux. You can also follow your distro-specific instructions to install these requirements:
```
sudo pacman -Syu base-devel linux-headers
```

Clone this repository with the following command:
```
git clone git@github.com:Lephar/Kernel.git
```

Now, change into the repository directory:
```
cd Kernel
```

That's it! You are now ready to compile the example kernel modules!

### Mainline Setup
Although the headers are enough to successfully build the module, it is recommended to configure and build your own kernel to help with our specific workflow needs. We can use latest release tag of [Linus Torvalds's mainline Linux mirror](https://github.com/torvalds/linux) for that purpose. Follow these instructions to build and install the mainline kernel.

Following packages are needed to build your own kernel:
```
sudo pacman -Syu xmlto kmod inetutils bc libelf git cpio perl tar xz
```

Clone mainline kernel source into this repository directory with the following command:
```
git clone git@github.com:torvalds/linux.git
```

Change into this directory:
```
cd linux
```

Configure the kernel to your taste using one of the following with `make` command: `config`, `menuconfig`, `nconfig`, `gconfig`, `xconfig`. Or you can simply use the default configuration, which is sufficient for most use cases:
```
make defconfig
```

When you fetch the updated source code and some new entries are added to the configuration, you can preserve the old config and set the default values to new configuration options with the following command:
```
make olddefconfig
```

Then compile the Linux kernel and configured modules with all the available threads. This is equivalent to calling `make` with `vmlinux`, `modules` and `bzImage` separately:
```
make -j$(nproc) all
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
Instead of Linus' mainline-branch, you can acquire [Greg Kroah-Hartman's stable-branch mirror](https://github.com/gregkh/linux). This branch has minor version tags which gets the security updates on top of mainline-branch.

Better yet, you can get to your distro's kernel repository, which are usually based on stable-branch kernel. These branches get distro-specific patches optimized to your distro's use case, on top of official security updates. This setup will offer more security and more compatibility with your current system. This repository includes reference to [Arch Linux kernel sources, with patches](https://github.com/archlinux/linux) as submodule. I try to keep it as up-to-date as possible by bumping it to latest release tag. You can clone this repository with the `--recursive` flag to also clone patched kernel source.

Almost every distro also customizes the kernel configuration depending on their use case. So, we can skip all the hassle of configuration and get the current kernel config using `procfs` from running system and write it into `.config` file with following command. Just make sure to run it inside cloned kernel repository directory:
```
zcat /proc/config.gz > .config
```

If the config has changed between versions, it's still necessary to manually configure or run `olddefconfig` over previous command. Other than that, it's all the similar process.

## Compiler Support

When it comes to kernel compilation, the default choice is `gcc`. All of the above `make` commands make use of it unless set otherwise. It's important to use the same compiler version when compiling the kernel and the modules, or you will get "version magic mismatch".

Even though it's not necessary to set one, a language server makes a great difference while writing the code. When using `gcc`, `ccls` offers great compatibility while also supporting realtive paths.

In order to use language server, it needs to know locations of headers and other compilation tokens for indexing. All these information is kept inside `compile_commands.json` file. It can be generated using an existing script in source directory after the compilation of the kernel:
```
scripts/clang-tools/gen_compile_commands.py
```

Note that, this script was originally written to generate compilation databse for `clangd` language server, but can also be consumed by `ccls`.
`clangd` can also be used with `gcc` but some `gcc` extensions and compiler arguments may be incompatible and unsupported.

Even though it is not without issues, `clang` compiler and `LLVM` toolchain can also be used to compile kernel and modules.

To switch compilers:
```
make CC=clang -j$(nproc) all
```

To switch whole toolchain:
```
make LLVM=1 -j$(nproc) all
```

Note that it is important to specify the same toolchain parameter with ALL above `make` commands, including config generation lines.

While using the `LLVM` toolchain, `clangd` is the obvious language server choice. Its compilation database is generated with the exact same script.

You must then copy the created `compile_commands.json` file into root of your workspace directory and you are good to go!

## Example Modules
Every individual module in this repository can be built using `Makefile` in their respective project folders:
```
make all
```

To show info on the compiled module:
```
modinfo <MODULE_NAME>.ko
```

To insert compiled module into the kernel:
```
sudo insmod <MODULE_NAME>.ko
```

To list all the inserted modules in the system:
```
lsmod
```

To remove inserted module from the kernel:
```
sudo rmmod <MODULE_NAME>
```

Meanwhile, kernel module output can be viewed on system logs using `journalctl`:
```
journalctl -f -k
```

Project files can be cleaned using `Makefile`:
```
make clean
```

That's all that is necessary, happy kernel hacking!