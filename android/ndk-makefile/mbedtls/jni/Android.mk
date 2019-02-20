LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libmbedcrypto

LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -fPIC -fpic

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../mbedtls/include
LOCAL_SRC_FILES := $(LOCAL_PATH)/../mbedtls/library/aes.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/aesni.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/arc4.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/aria.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/asn1parse.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/asn1write.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/base64.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/bignum.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/blowfish.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/camellia.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ccm.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/chacha20.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/chachapoly.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/cipher.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/cipher_wrap.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/cmac.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ctr_drbg.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/des.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/dhm.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ecdh.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ecdsa.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ecjpake.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ecp.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ecp_curves.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/entropy.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/entropy_poll.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/error.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/gcm.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/havege.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/hkdf.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/hmac_drbg.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/md.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/md2.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/md4.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/md5.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/md_wrap.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/memory_buffer_alloc.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/nist_kw.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/oid.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/padlock.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pem.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pk.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pk_wrap.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pkcs12.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pkcs5.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pkparse.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/pkwrite.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/platform.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/platform_util.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/poly1305.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/ripemd160.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/rsa_internal.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/rsa.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/sha1.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/sha256.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/sha512.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/threading.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/timing.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/version.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/version_features.c
LOCAL_SRC_FILES += $(LOCAL_PATH)/../mbedtls/library/xtea.c
include $(BUILD_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE := pk_sign
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../mbedtls/include
LOCAL_SRC_FILES := $(LOCAL_PATH)/../mbedtls/programs/pkey/pk_sign.c
LOCAL_STATIC_LIBRARIES := libmbedcrypto
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := pk_verify
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../mbedtls/include
LOCAL_SRC_FILES := $(LOCAL_PATH)/../mbedtls/programs/pkey/pk_verify.c
LOCAL_STATIC_LIBRARIES := libmbedcrypto
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_MODULE := pk_sign_mem
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../mbedtls/include
#LOCAL_SRC_FILES := $(LOCAL_PATH)/../mbedtls/programs/pkey/pk_sign_mem.c
#LOCAL_STATIC_LIBRARIES := libmbedcrypto
#include $(BUILD_EXECUTABLE)
