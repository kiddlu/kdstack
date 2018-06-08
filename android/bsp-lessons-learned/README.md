# Android BSP Lessons Learned

## Index

- [Kickoff](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#kickoff)
- [Boot](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#boot)
- [Linux init](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#linux-init)
- [Android init](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#android-init)
- [Stability](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#stability)
- [Optimize](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#optimize)
- [Security](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#security)
- [Recovery](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#recovery)
- [Branch](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#branch)
- [Windows](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#Windows)
- [Hardware](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#hardware)
- [Business](https://github.com/kiddlu/oh-my-stack/tree/master/android/bsp-lessons-learned#business)

## Kickoff

首先就是project的setup，先与CM合作布置好整个project，建好分支，要跟vendor相似，但又独立与vendor。

- Android与OEM的repo布置，各个子系统的编译，build error fix，在参考板上先把软件跑起来。
- 在参考版上验证vendor的烧录工具，刷机流程，能否支持命令行脚本，adb，fastboot等命令是否兼容。
- UFS需要先进行provision，EMMC不需要。
- device/vendor 下建立对应的项目目录，比如msm8974->sfo，这里坑特别多，但是没有什么好方法避免。<br>
  比如一份配置文件复制时，从ASIC格式变成UTF-8，多出来一些不可见字符，导致编译出错，这只能细心+经验。<br>
  如果一个平台对应的proj很多（US版本，高低端，CDMA版本），则需要建立common性的架构，再由各个proj来include。
- 在partition加入factory/alterable/security/slash这些自己的分区，调整partition.xml，原则是不改动的分区尽量往上提，如NV，fac，改动的分区往后，最后是userdata可变大小分区。这个也可以放到原生系统稳定后才做。
- 根据项目和硬件批次建立结构化的dts，尽量使用include来做分离和复用，分离的子模块尽量有自己的dtsi。
- 添加项目的defconfig，check vendor defconfig与android base的差异。
- lunch new-proj，build all，flash。

## Boot

这个阶段一般是芯片ROM读取Flash上的bootloader，bootloader再去load lk，lk引导kernel的过程。

- 许多vendor有secure boot的概念，与PM确定是否需要。一般项目初期都不需要这个，后期需要加上。
- bootloader会有初始化ddr，初始化charger，存在线dump，离线dump和传board-id等功能，前期不要改动。
- lk在引导成功后中要加入许多功能，前期除了DTS选择外，同样不改动。<br>
  `1.LCD，Charge和LED等功能`<br>
  `2.添加menu，进入DL和Fastboot模式，UART/Headset切换，是否禁用SELinux，控制kernel loglevel`<br>
  `3.从GPIO引脚状态，或者分区变量，确定cmdline的传参`<br>
  `4.user版本中干掉fastboot flash erase等功能`<br>
  `5.引导选dts时可能会出现选不上的情况，看经验了`<br>
- boot阶段的调试，基本只能靠串口
- 无法刷机时，可能是DDR问题，首先考虑降频还有更换成lite看是否能够刷机。

## Linux init

这个阶段可以看作kernel初始化，一直到init进程拉起adb，直到zygote启动前的阶段，init进程很关键，挂点会直接导致dump。

- on fs阶段，init会解析fstab的内容，和init.rc脚本，挂载上所有分区（system/userdata/zram/modem...）。<br>
  部分分区有SELinux的要求，要加上`context=u:object_r:xxx_file:s0`
- adb启动了，但adb shell返回/system/bin/sh没法执行，就是system分区没能挂载起来。<br>
  这时你可能需要Android-Platform-Knife解析下system分区没有挂载上的原因。
- SELinux同样可能导致系统无法启动，首板时可以关闭SE，`BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive`。
- init拉service时，会检查SE domain，没有规则会报错，开始时可以关闭。
- init进程会把kernel cmdline中androidboot.xxx转换成ro.boot.xxx设置到property当中。
  `main -> process_kernel_cmdline -> import_kernel_cmdline -> import_kernel_nv`

## Android init

这个阶段是从zygote启动到界面显示launcher的阶段。这是属于Framework层的事，但在原生版本上，还是需要带起来。

- zygote起不来，很有可能是库文件，资源文件不匹配或者缺失。
- 首次开机，起到launcher之前，需要把没有编译的apk都编译一遍，过程很慢，发热，掉核。
- FTM版本在射频校准之前不会启动Android Framework，否则误测会比较高，用cmdline控制。

## Stability

### Kernel

这个主要是解dump和修复内存泄漏问题，不黑屏，不死机，不无故重启，不突然掉电，不出现内存溢出，可以正常休眠。

- 有时候不完全是软件问题，可能是硬件原因。
- 一般一个新平台，都要更新一版ramdump parser。这个可以从高通release出来的包中拿到，也可以从 https://source.codeaurora.org/quic/la 下载。
- dump尽量先给高通提case，最好提前做好dump解析工具，事半功倍。
- 内存泄露问题比较难跟，kmemleak，frace，/proc等都用上，最好找到复现方法，写好复现脚本。监视内存的方法，可以看我的脚本
- suspend and resume逻辑问题，中断重入问题导致系统多拿一个锁，无法休眠。
- no_console_suspend可以在系统休眠是仍然开启串口中断。
- 内存泄漏问题可以通过kmemleak和adb-checkmem监控和调试，泄漏可能导致内存碎片话严重，而且无法回收释放，直到枯竭。

### Native

这个也出过不少问题

- netd多线程互锁导致了system_server restart。<br>
- fingerprintd在zygote重启后，出现fetal error，但是却没有重启，导致指纹无法使用。<br>
- daemon没有SE权限导致一些功能不能用或者反复重启，<br>
- logcat起不来，或者起来之后莫名其妙挂掉。后来查明是往userdata分区存离线log，导致verify出错，把校验去掉。<br>
- 段错误，系统调用不返回，没有办法reboot，服务不响应，unix权限不对，多线程互锁，大量僵尸类似这种问题。<br>
- 有些daemon还是vendor二进制文件，所以调试起来会很困难，一起解决。

## Optimize

### boot speed up

- 使用bootgraph分析kernel启动过程，一般耗时在serial driver上。
- 使用bootchart分析init启动过程，耗时可能在camera与media_server的互等上。
- 开启bootchart需要init支持，还需要添加命令行参数`BOARD_KERNEL_CMDLINE += androidboot.bootchart=120`，之前有个bug就是init解析参数出了问题，导致无法开启bootchart。
- bootgraph的开启参数，`BOARD_KERNEL_CMDLINE += printk.time=1 initcall_debug`
- 我做好了[解析脚本封装](https://github.com/kiddlu/oh-my-tools/blob/master/windows/android/bootanalyse.sh)
- 这里有一个很好的参考 https://blog.feabhas.com/2016/04/boot-times-in-linuxandroid/

### app launch lentency

- 使用systrace可以分析
- 写一个简单的demo app
- 可优化的空间，较少log输出，降低logcat的优先级，把非关键的daemon迁移小核心上

### remove useless module

- 使用我的[adk.sh](https://github.com/kiddlu/android-platform-knife/wiki/Android-Debug-Kit%E7%9A%84%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E)分析
- vendor自带的module，一般有宏控制，注释掉就可以
- vendor的测试apk，需要去Android.mk中注释掉
- 去除哪些部分，需要看经验，把进度放慢一点，不要一次都踢完，保证系统稳定性优先

### IO

- 移植sdcardfs，优化IO性能。`sdcard进程中做钩子，内核加入sdcardfs的选项`
- 更换data分区文件系统为f2fs，`TARGET_USERIMAGES_USE_F2FS := true`，内核加入f2fs选项fstab中更改参数
- 调试readahead

### Net

- 优化TCP/IP协议栈，默认参数()

### Memory

- Swap to ZRAM （fstab）
- Kernel Same-page Merging (KSM)
- Reduced system memory ()

## Security

https://source.android.com/security/index.html

### Boot

- 反fastboot unlock，进行fastboot flash刷机

### Kernel

- 主要是反root，这个比较困难，一般是对root软件进行反汇编，看它动了哪些节点，静态分析动态分析都需要用上。

### SELinux

- 为我们自己的东西建立domain
- 彻底了解整套体系，需要系统性学习，甚至阅读[源代码](https://github.com/SELinuxProject)
- ps -Z / ls -Z 能看到进程和文件的SELinux属性
- system/bin/下的可执行文件，需要编译systemimage才会生效。

### Filesystem Permissions

- 主要是Linux文件系统的权限管理，user，group，others权限的问题，报错一般是`Operation not permitted`或者`Permission denied`。但程序中有可能没有检查返回值，出现其他衍生报错，这时需要strace来定位。

### Secure boot

- efuse熔丝，不可逆
- SE的手机，9008不能刷非SE版本的软件

### Fingerprint

- 指纹支付

### dm-verity

- 当前我们是不强制的，在fstab中把`forceencrypt改成了encryptable`
- 不过后续产品在user版本中已经变成一致的了

### anti-root

- 反root的cace，我主要遇到过两单。一单是发现从内核WiFi驱动的momcpy没有做数组边界检查，直接拷贝过去导致的root。
- 另一单详细看这里：[https://github.com/kiddlu/anti-root-analyse/](https://github.com/kiddlu/anti-root-analyse/)

## Recovery

- OTA升级包的制作
- adb shell可执行的命令
- sdcard功能模拟
- log导出

## Feature

### 主要是检查vendor默认的是不是有些配置项没选上，经常会出现VPN用不了，流量统计用不了，都是因为kernel config没配置好。<br>

还要在hwinfo中加入对应的inode，给user space使用。还需要一些宏判断当前平台。<br>

### Offline log

离线的log，需要起logcat和klogd这两个daemon，user版本开启的机制。

### Offile Dump

高通的dump是，xbl中发现了重启标志位后，进入到了dump模式，一般通过USB从Windows电脑中抓取出来，但是经常出现这样的问题，就是测试过程中没能及时抓取，导致手机先没有电了。
这时我们需要的是，使用离线dump机制，在xbl中往/data/分区存储内存dump数据，需要xbl支持ext4文件系统，可以把linux内核的src porting过去，再做一些裁剪。

### USB Func

OTG，MTP，CDROM，Diag，Adb等功能都需要加上，驱动需要根据Fun MI来做匹配。<br>
具体check [https://github.com/kiddlu/windows-android-driver](https://github.com/kiddlu/windows-android-driver)

### Storage

刷机不刷userdata分区，启动后动态识别分区大小，直接格式化成对应大小的对应分区，改造fs_mgr

### odb 

OS debug bridge，主要是加入user版本的鉴权，用于在user版本下刷写IMEI，MAC等数据使用的，具体设计可以参看
for qcom: [OS Debug bridge](https://github.com/kiddlu/oh-my-stack/tree/master/android/os-debug-bridge/qcom.md)

### Android Platform tools

我维护的开源安卓平台工具刀，有诸多的小工具方便调试优化。在GitHub上已经超过了10个stars和10个forks。[https://github.com/kiddlu/android-platform-knife](https://github.com/kiddlu/android-platform-knife)。
使用方法可以阅读[Wiki](https://github.com/kiddlu/android-platform-knife/wiki)

## Branch

### Android-orig

- 原生线，项目初期进行bringup，调试硬件时用的，后期仅仅用作对比调试用，很少维护。

### Main Line

- 日常开发主线 

### MOL

- 主线在一段时间拉出来的版本，只修复bug，不进新feature，稳定后推给用户。

### Factory Line

- 会有很多工厂需求要进: 
- 比如干掉锁屏和输入法
- 干掉所有第三方App，干掉多余APK
- 极速开机，加入WITH_DEXPREOPT优化
- eng版本，root权限，关闭SElinux
- Diag功能扩展
- Windows驱动，IgnoreHWSerNum开关，一拖多需求， CDROM有坑，有些功能只能置于fun 0驱动才能装上
- adb offline问题，可能需要下载最新的adb或者`make USE_MINGW=y adb`自己动手编译一个，Andoid N后直接`mmma system/core/adb/ -j8`<br>diag口导致USB host shutdown问题，更新高通最新驱动后解决。<br>adb nodaemon server问题，是System32和SysWOW64的adb和dll版本不匹配<br>

## Windows

偶尔需要Windows的开发工作，如写工厂工具或者些测试脚本。
默认编译的adb是x86的，sideload > 3GB的镜像时会出问题，这时需要x64的adb，需要自己编译Windows版本。看这里[adb-win64](https://github.com/kiddlu/adb-win64)。

Windows脚本编程，可以看这里：[Windows Batch Scripting](https://github.com/kiddlu/oh-my-stack/tree/master/windows/bat)

## Hardware

辅助硬件定位问题，手机系统复杂性，很多问题可能并不是软件原因导致的，但需要软件辅助硬件，帮他们的检查问题。

例子:

- 硬件画原理图的时候，某个点和线没有连接起来，导致校准异常，最后是软件先定位，硬件将原理图放大数百倍才发现的问题
- 硬件设计问题导致了GPS定位不准，电池无法充满，这些都需要一起协同定位问题
- 刷机出现比特位反转，最后查明是电脑USB口的电压和线材问题
- 硬件干扰波拉死RPM导致dump的问题，电压不稳，突然下降导致的死机
- 虽然不是自己遇到的，但是一个很经典，软件定位硬件问题的例子，出现在Playstation经典游戏《古惑狼》上<br>
  [http://blog.jobbole.com/50995/](http://blog.jobbole.com/50995/)

## Business

有时候即使技术上没问题，也还会出很多问题。
Android设备是个极度复杂的硬件项目，供应链繁多复杂。供应商随时有可能变更，倒闭，或者不跟你合作了。
之前我们遇到的一个问题就是升级 Android 大版本时，没有得到某家无线巨头供应商的支持，原因很多，导致我们的大版本升级浪费了很多人力和时间，so，不想太多展开。
