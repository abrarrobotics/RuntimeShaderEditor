
##############################################
#        3DBasicFramework for Android        #
#        date: 6/20/2016                     #
##############################################

LOCAL_PATH          := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE        := 3DBasicFramework
LOCAL_SRC_FILES     := ./native.cpp\
                    ../../src/Scene.cpp\
                    ../../src/Monkey.cpp\
                    ../../src/Utils/tga.cpp\
                    ../../src/Textures/Texture.cpp\
                    ../../src/ShaderProgram/ShaderProgram.cpp\
                    ../../src/ModelLoader/FormatDetector.cpp\
                    ../../src/ModelLoader/PLYLoader.cpp\
                    ../../src/ModelLoader/ModelLoader.cpp\
                    ../../src/InputManager/InputManager.cpp\
                    ../../src/Camera/Camera.cpp\
                    ../../../editor/server/RuntimeShaderEditor/RuntimeShaderEditor.cpp
LOCAL_C_INCLUDES    += $(LOCAL_PATH)/../../src
LOCAL_CFLAGS        += -I "$(CC_COMMON_LIBS)/glm"
LOCAL_CFLAGS        += -I "$(LOCAL_PATH)/../../../editor/server"
LOCAL_CFLAGS        += -DOS_ANDROID -DANDROID
LOCAL_CFLAGS        += -Wno-int-to-pointer-cast
LOCAL_CPPFLAGS      += -fno-rtti -fno-exceptions -std=c++11 -g
LOCAL_CFLAGS        += -fno-rtti -fno-exceptions -std=c++11 -g
LOCAL_LDLIBS        += -lGLESv3 -lEGL -llog
NDK_TOOLCHAIN_VERSION := clang
include $(BUILD_SHARED_LIBRARY)