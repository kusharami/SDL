LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := my_malloc

LOCAL_SRC_FILES := my_malloc.c

include $(BUILD_SHARED_LIBRARY)