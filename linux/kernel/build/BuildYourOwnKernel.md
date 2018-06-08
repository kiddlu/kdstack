## [BuildYourOwnKernel](https://wiki.ubuntu.com/Kernel/BuildYourOwnKernel)

This page will describe how to easily build the kernel.

The majority of users that are interested in building their own kernel are doing so because they have installed Ubuntu on their system and they wish to make a small change to the kernel for that system. In many cases the user just wants to make a kernel configuration change.

The purpose of this page is to give that user a minimum amount of information for them to meet the goal of making a simple change to the kernel, building it and installing their kernel. It is not intended to be the definitive guide to doing Ubuntu kernel development.

## Obtaining the source for an Ubuntu release

There are a number of different ways of getting the kernel sources. The two main ways will be documented here.

If you have installed a version of Ubuntu and you want to make changes to the kernel that is installed on your system, use the **apt-get** method (described below) to obtain the sources.

However, if you wish to get the most up to date sources for the Ubuntu release you are running and make changes to that, use the **git** method (described below) to obtain the sources.

### apt-get

The source code which generated a specific binary package may be obtained using the `apt-get source <package>` command. For example to obtain the source for the currently running kernel you can use the command:

```
apt-get source linux-image-$(uname -r)
```

### git

All of the Ubuntu Kernel source is maintained under `git`. The source for each release is maintained in its own git repository on `kernel.ubuntu.com`. To obtain a local copy you can simply git clone the repository for the release you are interested in as shown below.

```
git clone git://kernel.ubuntu.com/ubuntu/ubuntu-<release codename>.git
```

For example to obtain the precise tree:

```
git clone git://kernel.ubuntu.com/ubuntu/ubuntu-precise.git
```

## Build Environment

If you've not built a kernel on your system before, there are some packages needed before you can successfully build. You can get these installed with:

```
sudo apt-get build-dep linux-image-$(uname -r)
```

## Modifying the configuration

This step can be skipped if no configuration changes are wanted. The build process will use a configuration that is put together from various sub-config files. The simplest way to modify anything here is to run:

```
chmod a+x debian/rules
chmod a+x debian/scripts/*
chmod a+x debian/scripts/misc/*
fakeroot debian/rules clean
fakeroot debian/rules editconfigs # you need to go through each (Y, Exit, Y, Exit..) or get a complaint about config later
```

This takes the current configuration for each architecture/flavour supported and calls menuconfig to edit its config file. The chmod is needed because the way the source package is created, it loses the executable bits on the scripts.

In order to make your kernel "newer" than the stock Ubuntu kernel from which you are based you should add a local version modifier. Add something like "+test1" to the end of the first version number in the `debian.master/changelog` file, before building. This will help identify your kernel when running as it also appears in `uname -a`. Note that when a new Ubuntu kernel is released that will be newer than your kernel (which needs regenerating), so care is needed when upgrading. NOTE: do not attempt to use CONFIG_LOCALVERSION as this _will_ break the build.

## Building the kernel

Building the kernel is quite easy. Change your working directory to the root of the kernel source tree and then type the following commands:

```
fakeroot debian/rules clean
# quicker build:
fakeroot debian/rules binary-headers binary-generic binary-perarch
# if you need linux-tools or lowlatency kernel, run instead:
fakeroot debian/rules binary
```

If the build is successful, a set of three .deb binary package files will be produced in the directory above the build root directory. For example after building a kernel with version "4.8.0-17.19" on an amd64 system, these three (or four) .deb packages would be produced

```
cd ..
ls *.deb
    linux-headers-4.8.0-17_4.8.0-17.19_all.deb
    linux-headers-4.8.0-17-generic_4.8.0-17.19_amd64.deb
    linux-image-4.8.0-17-generic_4.8.0-17.19_amd64.deb
```

on later releases you will also find a linux-extra- package which you should also install if present.

## Testing the new kernel

Install the three-package set (on your build system, or on a different target system) with dpkg -i and then reboot:

```
sudo dpkg -i linux*4.8.0-17.19*.deb
sudo reboot
```

## Debug Symbols

Sometimes it is useful to have debug symbols built as well. Two additional steps are needed. First pkg-config-dbgsym needs to be installed. Second when executing the binary-* targets you need to add 'skipdbg=false'.

```
sudo apt-get install pkg-config-dbgsym
fakeroot debian/rules clean
fakeroot debian/rules binary-headers binary-generic binary-perarch skipdbg=false
```

## See also

The above instructions provide a very simple recipe for obtaining the sources and then building them. If you are going to be doing more kernel development than simple configuration changes you may want to look at:

- | [Kernel Git Guide](https://wiki.ubuntu.com/Kernel/Dev/KernelGitGuide) | More information about using git to pull down the kernel sources. |
  | ---------------------------------------- | ---------------------------------------- |
  | [ARM Cross Compile](https://wiki.ubuntu.com/KernelTeam/ARMKernelCrossCompile) | For more info about ARM and cross compilation. |

