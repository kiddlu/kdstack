/proc/kallsyms eq System.map

|      |                              |
| ---- | ---------------------------- |
| A    | 绝对                           |
| B或b  | 未初始化的数据段（BSS）                |
| D或d  | 初始化的数据段                      |
| G或g  | 小对象的初始化数据段（全局的）              |
| i    | DLL相关的段                      |
| N    | 调试用符号                        |
| p    | 堆栈的展开段（stack unwind section） |
| R或r  | 只读数据段                        |
| S或s  | 小对象的未初始化数据段                  |
| T或t  | 代码段                          |
| U    | 未定义                          |
| V或v  | 弱对象（weak object）             |
| W或w  | 没有标签的弱对象（weak object）        |
| -    | a.out目标文件中的桩符号（stabs symbol） |
| ?    | 未知符号                         |

