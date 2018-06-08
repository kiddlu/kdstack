##表格

| 802.11标准 |    WiFi联盟认证     | 认证方式                      | 加密方式                 | 加密算法 | 密钥生成 |
| :------: | :-------------: | :------------------------ | -------------------- | ---- | ---- |
|   传统方式   |                 | Open System<br> Share Key | WEP                  | RC4  | 静态   |
|          |  WPA-Personal   | WPA-PSK                   | TKIP                 | RC4  | 动态   |
|          | WPA-Enterprise  | 802.1X/EAP                | TKIP                 | RC4  | 动态   |
|   RSN    |  WPA2-Personal  | WPA2-PSK                  | CCMP(强制)<br>TKIP(可选) | AES  | 动态   |
|   RSN    | WPA2-Enterprise | 802.1X/EAP                | CCMP(强制)<br>TKIP(可选) | AES  | 动态   |


#WiFi：(CWNA学习指南，第13章）
###传统的安全机制：
####认证方式
> Open System Authentication: 不需要提供任何身份认证信息，通过简单协议交互就能获得接入权限。<br>
> Share Key Authentication: 要求WiFi设备支持WEP加密，AP与STA必须配置相同的静态WEP密钥。<br>

####静态WEP加密
>WEP(Wired Equivalent Privacy)加密是以RC4流加密为基础的二层加密机制，有64位和128位。数据完整性要求发送方在加密前要计算ICV(Integrity Check Value)，防止数据在传输过程中被篡改。

####MAC地址过滤
####SSID隐藏



###强健安全机制：
####RSN(Robust Security Network)
> 4-Way Handshke认证，建立关联和动态创建唯一的加密密钥。
> 802.1X/EAP 企业级认证，需要RADIUS服务器。 PSK SOHO级认证

####PSK认证
> WPA/WPA2-Person ASCII密码短语，8到63字节，映射成PSK。WPA和WPA2的区别是加密方式不同。

####802.1X认证
> Port-based Access Control，有三部分构成：supplicant，authenticator，authentication server。
> 实际认证时还需要配合认证协议EAP(Extensible Authentication Protocol)

####四次握手
> 经过801.1X/EAP或者PSK后，已经的到了PMK，通过四次握手，生成最终加密使用的PTK
> 过程如下：
> 1/4：Authenticator -> Supplicant 
> Authenticator把ANonce送给Supplicant。Supplicant收到1/4后，就有了生成PTK的所有元素。因为1/4里同时也包含了Authenticator的MAC地址。
>
> 2/4：Supplicant -> Authenticator
> Supplicant计算出PTK，把SNonce和自己的MAC地址送给Authenticator。同时，从2/4报文开始，后面的每个报文都会有MIC。1/4没有。
>
> 3/4：Authenticator -> Supplicant
> Authenticator向Supplicant证明自己有有效的，同样有MIC加入其中。
>
> 4/4：Supplicant -> Authenticator
> 仅是对3/4的一个ACK。说明PTK已经装好，后面的数据可以加密了。

####TKIP和CCMP
> TKIP，采用RC4流加密，由`128位临时密钥，48位IV，SRC MAC， DST MAC`通过复杂每包密钥混合生成，实质是WEP增强版本，升级软件即可。
> CCMP，采用AES块加密。



###强制Web门户：
> 企业内部，推广使用802.1X仍然非常困难。绝大多数酒店，机场，餐馆等设施采用了强制Web门户的方式，在二层认证结束后，通过Web认证授予客户端网络访问权限。