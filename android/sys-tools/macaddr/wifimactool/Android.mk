LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= wifi_mac_tool.c
LOCAL_MODULE:= wifimactool

LOCAL_SHARED_LIBRARIES := libc libcutils

include $(BUILD_EXECUTABLE)
