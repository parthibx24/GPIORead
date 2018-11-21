LOCAL_PATH := $(call my-dir) 

include $(CLEAR_VARS)
LOCAL_MODULE := GPIORead
LOCAL_SRC_FILES := GPIORead.c meta_gpio.c
include $(BUILD_EXECUTABLE)
