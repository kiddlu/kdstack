| Due date | Task                                     |
| -------- | ---------------------------------------- |
| 11.10    | GO和O的编译参数与system properties对比，明确影响的模块，以及确定安卓系统如何识别自身是GO还是O。 |
| 11.13    | 参考《Android Go Edition Device Configuration Guide》，编译出基于R8符合Google默认定义的GO版本和O版本，如果R8没有办法编译出O版本，使用X572作为O版本参照，确定GO与O的memory和storage使用情况，分开对比OS系统和第三方应用。(当前R8编译选项跟Google默认GO编译选项有出入) |
| 11.14    | 对比GO和O以下模块的行为差异，具体参数对系统的影响：<br>lmkd:：影响系统杀进行的门限和频繁程度<br>jemalloc：GO时malloc时只使用一个区域 <br>system_server：GO使用speed-profile减少系统服务的内存 |
| 11.15    | 对比GO和O以下模块的行为差异，具体参数对系统的影响：<br>pm.dexopt：install apk和shared apk的编译的影响<br>dalvik.vm：art runtime的堆栈使用限制 |
| 11.16    | GO与O的Native和Benchmark App test，从目前得到的信息看，GO并不影响native的性能，对上层的性能影响需要benchmark测试对比得到相应数据。测试模块包括CPU/GPU/IO/MEM等。 |
| 11.17    | GO在节约内存和存储空间，是以更大的CPU占用和功耗为代价的。设置一些简单测试场景如”连续播放视频4小时“对比GO和O的耗电情况。整理之前的数据，输出性能评估报告。 |
