#!/bin/sh

#这个脚本的目标是，将UNC的路径，转换为Windows默认路径格式

#其中 \\192.168.1.1\flash 已经被映射到了Y盘
#所以，\\192.168.1.1\flash\daily\helloworld 这个UNC地址
#对应的Window路径为：Y:\daily\helloworld 
#其实Windows本身的路径，是不区分斜杠和反斜杠的
#所以，为了方便，假设我们已经通过一堆的shell脚本得到了
#下面三个使用了斜杠代替反斜杠的字符串

drive='Y:'
map_point='//192.168.1.1/flash'
unc_path='//192.168.1.1/flash/daily/nidaye'

#然后转化为正则表达式，方便之后使用sed替换
map_point_regex=$(echo $map_point | sed "s/\//\\\\\//g")
drive_regex=$(echo $drive | sed "s/\:/\\\:/g")
echo "$map_point_regex"  #\/\/192.168.1.1\/flash
echo "$drive_regex"      #Y\:

#执行替换
echo "$unc_path" | sed "s/$map_point_regex/$drive_regex/g"
#这样就得到了一个cmd可以正常访问的路径 Y:/daily/nidaye

#我还真被自己的实现恶心到了....
