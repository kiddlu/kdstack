#!/bin/bash
echo 初始 OPTIND: $OPTIND

while getopts "a:b:c" arg #选项后面的冒号表示该选项需要参数
do
    case $arg in
        a)
			echo "a's arg:$OPTARG" #参数存在$OPTARG中
			;;
        b)
			echo "b's arg:$OPTARG"
			;;
        c)
			echo "c's arg:$OPTARG"
			;;
        ?)  #当有不认识的选项的时候arg为?
			echo "unkonw argument"
			exit 1
		;;
    esac
done

echo 处理完参数后的 OPTIND：$OPTIND
echo 移除已处理参数个数：$((OPTIND-1))
shift $((OPTIND-1))
echo 参数索引位置：$OPTIND
echo 准备处理余下的参数：
echo "Other Params: $@"