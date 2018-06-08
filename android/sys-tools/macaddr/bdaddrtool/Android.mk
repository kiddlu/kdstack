LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= bd_addr_tool.c
LOCAL_MODULE:= bdaddrtool

LOCAL_SHARED_LIBRARIES := libc libcutils

include $(BUILD_EXECUTABLE)
