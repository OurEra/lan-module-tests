LOCAL_PATH:= $(call my-dir)

# build package
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under,src) \
                   ../server/src/com/srw/demo_server/IServiceTest.aidl

LOCAL_RESOURCE_DIR := $(addprefix $(LOCAL_PATH)/,res)

LOCAL_STATIC_JAVA_LIBRARIES := \
    lib-constrolapi

LOCAL_PACKAGE_NAME := Demoapp
LOCAL_CERTIFICATE := platform
include $(BUILD_PACKAGE)

# prebuilt jar
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := \
   lib-constrolapi:libs/javalib.jar

include $(BUILD_MULTI_PREBUILT)
