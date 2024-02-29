LOCAL_PATH:= $(call my-dir)

# build package
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under,src) \
                   src/com/srw/demo_server/IServiceTest.aidl

LOCAL_RESOURCE_DIR := $(addprefix $(LOCAL_PATH)/,res)

LOCAL_PACKAGE_NAME := Demoserver
LOCAL_CERTIFICATE := platform
include $(BUILD_PACKAGE)
