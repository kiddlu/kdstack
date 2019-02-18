在给android的apk签名的时候，需要用到一个扩展名为.pem和.pk8的文件，我第一反应，这啥啊，英文缩写？反正linux不用后缀名来区分文件，这到底是什么呢？ 



首先在密码学上，有两个概念PKCS和X.509，在我理解，这俩都是类似一套协议，标准的东西。标准是啥，就比如人的姓名，以姓氏开始，然后再是名，张三。标准就是为了让大家都去遵循，好形成一套好管理、易理解、大家都知道的东西。

基础知识：
密码学中将对称加密的密钥称为secret key,而将非对称加密的私钥和公钥分别称为private key 和 public key

## PKCS
PKCS全称为Public-Key Cryptography Standards (PKCS) ，公钥加密标准，很多标准^_^。维基上关于其的解释如下，

```
In cryptography, PKCS is a group of public-key cryptography standards devised and published by RSA Security Inc, starting in the early 1990s. The company published the standards to promote the use of the cryptography techniques to which they had patents, such as the RSA algorithm, the Schnorr signature algorithm and several others. Though not industry standards (because the company retained control over them), some of the standards in recent years[when?] have begun to move into the "standards-track" processes of relevant standards organizations such as the IETF and the PKIX working-group.
```

就是由某个公司发布的一组公钥加密标准，协议。目的是为了促进大家使用他们公司相关专利的技术，例如RSA算法等。由于这些专利是由RSA Security公司拥有，所以这些标准没有成为工业标准，但是最新这些年开始进入IETF 和PKIX的standards-track。 

而PKCS这套协议呢，是以#数字的方式进行命名。例如 
PKCS #1： RSA Cryptography Standard

|   标准名   |  名字    |   解释   |
| ---- | ---- | ---- |
|  PKCS #1    |   RSA Cryptography Standard   |  ee RFC 3447. Defines the mathematical properties and format of RSA public and private keys (ASN.1-encoded in clear-text), and the basic algorithms and encoding/padding schemes for performing RSA encryption, decryption, and producing and verifying signatures.    |
| PKCS #5 | Password-based Encryption Standard | See RFC 2898 and PBKDF2. |
| PKCS #7 | Cryptographic Message Syntax Standard | See RFC 2315. Used to sign and/or encrypt messages under a PKI. Used also for certificate dissemination (for instance as a response to a PKCS#10 message). Formed the basis for S/MIME, which is as of 2010 based on RFC 5652, an updated Cryptographic Message Syntax Standard (CMS). Often used for single sign-on. |
| PKCS #8 | Private-Key Information Syntax Standard | See RFC 5958. Used to carry private certificate keypairs (encrypted or unencrypted). |
| PKCS #12 | Personal Information Exchange Syntax Standard | See RFC 7292. Defines a file format commonly used to store private keys with accompanying public key certificates, protected with a password-based symmetric key. PFX is a predecessor to PKCS #12.This container format can contain multiple embedded objects, such as multiple certificates. Usually protected/encrypted with a password. Usable as a format for the Java key store and to establish client authentication certificates in Mozilla Firefox. Usable by Apache Tomcat. |



​	

## X.509
维基百科对X.509的解释如下，

```
In cryptography, X.509 is an ITU-T standard for a public key infrastructure (PKI) and Privilege Management Infrastructure (PMI). X.509 specifies, amongst other things, standard formats for public key certificates, certificate revocation lists, attribute certificates, and a certification path validation algorithm.
```

```
X.509 was initially issued on July 3, 1988 and was begun in association with the X.500 standard. It assumes a strict hierarchical system of certificate authorities (CAs) for issuing the certificates. This contrasts with web of trust models, like PGP, where anyone (not just special CAs) may sign and thus attest to the validity of others' key certificates. Version 3 of X.509 includes the flexibility to support other topologies like bridges and meshes.[1] It can be used in a peer-to-peer, OpenPGP-like web of trust[citation needed], but was rarely used that way as of 2004. The X.500 system has only ever been implemented by sovereign nations for state identity information sharing treaty fulfillment purposes, and the IETF's Public-Key Infrastructure (X.509), or PKIX, working group has adapted the standard to the more flexible organization of the Internet. In fact, the term X.509 certificate usually refers to the IETF's PKIX Certificate and CRL Profile of the X.509 v3 certificate standard, as specified in RFC 5280., commonly referred to as PKIX for Public Key Infrastructure (X.509).
```
```
In the X.509 system, a certification authority issues a certificate binding a public key to a particular distinguished name in the X.500 tradition, or to an alternative name such as an e-mail address or a DNS entry.
```
下面是维基百科上总结的X.509常用数字证书的扩展名：
```
.pem – (Privacy-enhanced Electronic Mail) Base64 encoded DER certificate, enclosed between "-----BEGIN CERTIFICATE-----" and "-----END CERTIFICATE-----"
.cer, .crt, .der – usually in binary DER form, but Base64-encoded certificates are common too (see .pem above)
.p7b, .p7c – PKCS#7 SignedData structure without data, just certificate(s) or CRL(s)
.p12 – PKCS#12, may contain certificate(s) (public) and private keys (password protected)
.pfx – PFX, predecessor of PKCS#12 (usually contains data in PKCS#12 format, e.g., with PFX files generated in IIS)
```



## .pem、.pk8和.x509.pem

```

.pem
通过上面的分析，很明显，在android对apk签名的时候，.pem这种文件就是一个X.509的数字证书，里面有用户的公钥等信息，是用来解密的。但是由上文可知，这种文件格式里面不仅可以存储数字证书，还能存各种key。

.pk8 
上文没有提过以.pk8为扩展名的文件，应该和PKCS #8是对应的，用来保存private key，才有DER格式存储。


.x509.pem
ITU-T X.509 证书
```

