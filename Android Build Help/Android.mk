LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(call all-subdir-makefiles)
#LOCAL_MODULE := libprotobuf_static
#LOCAL_MODULE_FILENAME := libprotobuf
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/protobuf-3.5.1/src/
#LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += pseudoproto.cpp
#include $(BUILD_SHARED_LIBRARY)

LOCAL_MODULE := protobuf_static

LOCAL_MODULE_FILENAME := libprotobuf

LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := \
src/google/protobuf/io/coded_stream.cc                \
src/google/protobuf/stubs/common.cc                   \
src/google/protobuf/descriptor.cc                     \
src/google/protobuf/descriptor.pb.cc                  \
src/google/protobuf/descriptor_database.cc            \
src/google/protobuf/dynamic_message.cc                \
src/google/protobuf/extension_set.cc                  \
src/google/protobuf/extension_set_heavy.cc            \
src/google/protobuf/generated_message_reflection.cc   \
src/google/protobuf/generated_message_util.cc         \
src/google/protobuf/io/gzip_stream.cc                 \
src/google/protobuf/compiler/importer.cc              \
src/google/protobuf/message.cc                        \
src/google/protobuf/message_lite.cc                   \
src/google/protobuf/stubs/once.cc                     \
src/google/protobuf/compiler/parser.cc                \
src/google/protobuf/io/printer.cc                     \
src/google/protobuf/reflection_ops.cc                 \
src/google/protobuf/repeated_field.cc                 \
src/google/protobuf/service.cc                        \
src/google/protobuf/stubs/structurally_valid.cc       \
src/google/protobuf/stubs/strutil.cc                  \
src/google/protobuf/stubs/substitute.cc               \
src/google/protobuf/stubs/stringprintf.cc               \
src/google/protobuf/text_format.cc                    \
src/google/protobuf/io/tokenizer.cc                   \
src/google/protobuf/unknown_field_set.cc              \
src/google/protobuf/wire_format.cc                    \
src/google/protobuf/wire_format_lite.cc               \
src/google/protobuf/io/zero_copy_stream.cc            \
src/google/protobuf/io/zero_copy_stream_impl.cc       \
src/google/protobuf/io/zero_copy_stream_impl_lite.cc \
src/google/protobuf/arena.cc                       \
src/google/protobuf/stubs/stringpiece.cc  \
src/google/protobuf/io/strtod.cc \
src/google/protobuf/map_field.cc \
src/google/protobuf/any.cc
#src/google/protobuf/util/json_util.cc



    # C++ lite library
    # =======================================================

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
#LOCAL_MODULE := libboost_system_static
#LOCAL_MODULE_FILENAME := libprotobuf
#LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../lib/Debug/libboost_system-clang-darwin-mt-a32-1_67.a
#include $(BUILD_STATIC_LIBRARY)

include $(call all-subdir-makefiles)

LOCAL_MODULE    := libpseudo
#LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H
LOCAL_CPPFLAGS := -std=c++11 -pthread -frtti -fexceptions -w -DBOOST_ERROR_CODE_HEADER_ONLY -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_SYSTEM_NO_LIB -DBOOST_DATE_TIME_NO_LIB -DBOOST_REGEX_NO_LIB
#-fvisibility=hidden
LOCAL_C_INCLUDES := $(LOCAL_PATH)/boost_1_67_0/
#LOCAL_LDFLAGS += -llog

LOCAL_SRC_FILES := pseudo.cpp

include $(BUILD_SHARED_LIBRARY)

#$(call import-module,.)


include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

AL_CPP_EXTENSION := .cc .cpp

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/BuildingBase.cpp \
                   $(LOCAL_PATH)/../../../Classes/FighterUnitBase.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameMessage.pb.cc \
                   $(LOCAL_PATH)/../../../Classes/GameMessageInterface.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/LoadingScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/MenuScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/MouseRect.cpp \
                   $(LOCAL_PATH)/../../../Classes/MyResources.cpp \
                   $(LOCAL_PATH)/../../../Classes/NetworkLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/PathArithmetic.cpp \
                   $(LOCAL_PATH)/../../../Classes/SettingLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/SocketClient.cpp \
                   $(LOCAL_PATH)/../../../Classes/SocketServer.cpp \
                   $(LOCAL_PATH)/../../../Classes/TiledMap.cpp \
                   $(LOCAL_PATH)/../../../Classes/Unit.cpp \
                   $(LOCAL_PATH)/../../../Classes/UnitCreate.cpp \
                   $(LOCAL_PATH)/../../../Classes/UnitManager.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../include \
                    $(LOCAL_PATH)/../../../include/boost \
                    $(LOCAL_PATH)/../../../

LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../include
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../../../include/boost

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += libpseudo
LOCAL_WHOLE_STATIC_LIBRARIES := protobuf_static
#LOCAL_WHOLE_STATIC_LIBRARIES := libprotobuf_static
#LOCAL_WHOLE_STATIC_LIBRARIES += libboost_system_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
