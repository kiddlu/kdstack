# OS Debug Bridge



## Android Side(Service Daemon)

- key：RSA签名的pub/priv key，pub key存放到Android端，priv key存放到smartdog中。

- mbedtls：原名polarssl，类似于openssl的加密库，但是更加简洁，API也更容易使用，后来被ARM买了并更名为mbedtls，用于签名和验证。写了Android.mk移植到Android上用来验证Client。

- odbd：Android端的Service Daemon，原来实现Android平台debug的解锁功能

### odbd的源码结构
- main.c： main入口函数，通过socket保证单实例运行，并根据平台选择启动对应的调试子系统(Qcom/MTK/Others)
- odb_diag.c：Qcom Diag 子系统的注册封装和数据传输，用来解锁所用的Diag调试命令
- random_opt.c：生成随机数的工具函数
- rsa_opt.c：RSA算法的封装函数，向下对接的实现是mbedtls




## Windows Side(Client)

- librsa_opt：RSA算法的封装函数，向下对接mbedtls，从smartdog读取priv key并签名
- mbedtls：同Android Side，基础的加密算法库
- odb：Client端，根据平台选择对应的传输方式进行Debug解锁
- smartdog_tools：smartdog的读写工具，用来读取和写入priv key

### odb的源码结构
- lib/hasp_windows_115013.dll：smartdog操作的函数，没有源码，供应商提供
- lib/libmbedcrypto.dll：mbedtls编译出来的算法库，odb用来签名
- lib/librsa_opt.dll：封装了libmbedcrypto和hasp_windows_115013，往上为C#代码暴露更简单的API，使得C#代码可以不需要直接操作smartdog
- lib/QMSL_MSVC10R.dll：Qcom Diag操作的封装库，用来维持底层通信
- src/main.cs：main函数，根据不同平台选择入口
- src/diag.cs：基于Qcom Diag的签名封装
- src/dll-MbedTLS.cs：librsa_opt.dll的C#封装
- src/dll-QMSLPhone.cs：QMSL_MSVC10R.dll的C#封装





## 通信过程
|              odbd（Android）              |         odb（Windows）         |
| :-------------------------------------: | :--------------------------: |
|                                         |       <-- 字符串 "unlock"       |
|              512字节随机数 -->               |                              |
|                                         | <-- 256字节的签名（使用priv签名收到的随机数） |
| 验证签名，返回字符串 "Unlock success/ failed" --> |                              |
|                                         |       验证成功，正常发送平台调试命令        |



## 基于Qcom Diag的数据包结构
|       数据包格式        |                                   |            |
| :----------------: | :-------------------------------: | :--------: |
| subsys-ID(4 Bytes) | payload-lenth(2 Bytes，little-end) | payload... |

不同平台数据包格式可能有区别，Qcom Diag的数据包要求前4个字节是子系统ID，后面的数据由各个子系统自己解析，odb_diag在数据包`<5，6字节>`要求放一个`ushort`类型的长度，代表payload的长度，从第7字节开始，就是payload的内容。

举个例子，odb在申请解锁时，需要往odbd发送一个unlock的字符串，odbd的子系统ID是`{0x4b, 0xfe, 0x00, 0x00}`，payload`unlock`长度是ushort 6 小端, `{0x06, 0x00}`，字符串对应的ascii码是`{0x75, 0x6e, 0x6c, 0x6f, 0x63, 0x6a}`

所以，解锁申请的数据包是
`{0x4b, 0xfe, 0x00, 0x00, 0x06, 0x00, 0x75, 0x6e, 0x6c, 0x6f, 0x63, 0x6a}`



## 简单的RSA介绍
- 非对称加密算法，常用来做签名

- pub的由N和E组成，priv由N D组成，[还有必须保密的P Q]，虽然不参与计算，但也是私钥的一部分

- EPQ是随机生成的，但有要求，生成的PQ互质，E与(P-1)(Q-1)互质，且 1 < E < (P-1)×(Q-1)，P != Q

- N = P×Q,  E×D=1 mod (P-1)×(Q-1)

- D P Q任意一个都不能外漏，否则就等于priv泄漏

- 在没有 D Q P，情况下要得到D，只能从N=PQ这条公式暴力破解P，Q（...当前，只要N足够大，还没发现这样的黑科技）

- http://www.ruanyifeng.com/blog/2013/07/rsa_algorithm_part_two.html