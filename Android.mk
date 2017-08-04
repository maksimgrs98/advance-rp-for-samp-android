LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := samp
LOCAL_LDLIBS += -llog -lEGL -lGLESv2
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/util/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/RakNet/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/RakNet/SAMP/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/game/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/net/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/gui/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/gui/imgui/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/RakNet/SAMP

LOCAL_CPPFLAGS += -D_ARM_ -DRAKSAMP_CLIENT -D_RAKNET_THREADSAFE
LOCAL_CPPFLAGS += -w -pthread -fpack-struct=1 -fvisibility=hidden -fpermissive
include $(BUILD_SHARED_LIBRARY)