#arm-linux-gnueabi-gcc -o shutdown shutdown.c -static

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= shutdown.c
LOCAL_MODULE:= shutdown 

include $(BUILD_EXECUTABLE)
