#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.1)

set(APP_NAME BattleField_RTS)

project(${APP_NAME})

#!if(WIN32)
#!set(COCOS2DX_ROOT_PATH $ENV{COCOS_X_ROOT})
#!elseif(MACOSX)
#cocos2d-x-3.17

set(COCOS2DX_ROOT_PATH $ENV{COCOS_X_ROOT}/cocos2d-x-3.17)

#set(COCOS2DX_ROOT_PATH $ENV{COCOS_X_ROOT})

#!set(COCOS2DX_ROOT_PATH $ENV{COCOS_X_ROOT}/)
#!endif()
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
if(NOT USE_COCOS_PREBUILT)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

include_directories(
        Classes
        ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
	Classes/include
)
#LINK_DIRECTORIES(
#	Classes/lib
#)
# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
       Classes/AppDelegate.cpp
     Classes/HelloWorldScene.cpp
     Classes/MenuScene.cpp
     Classes/SettingLayer.cpp
     Classes/GameMessage.pb.cc
     Classes/NetworkLayer.cpp
     Classes/SocketClient.cpp
     Classes/SocketServer.cpp
     Classes/GameScene.cpp
     Classes/MyResources.cpp
     Classes/UnitCreate.cpp
	 Classes/TiledMap.cpp
	 Classes/UnitManager.cpp
	 Classes/BuildingBase.cpp
	 Classes/FighterUnitBase.cpp
	 Classes/Unit.cpp
	 Classes/MouseRect.cpp
	 Classes/PathArithmetic.cpp
	 Classes/GameMessageInterface.cpp
	 Classes/LoadingScene.cpp
     )
list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/HelloWorldScene.h
     Classes/MenuScene.h
     Classes/SettingLayer.h
     Classes/GameMessage.pb.h
     Classes/NetworkLayer.h
     Classes/SocketClient.h
     Classes/SocketServer.h
	 Classes/SocketMessage.h
     Classes/GameScene.h
     Classes/Grid.h
     Classes/MyResources.h
     Classes/UnitCreate.h
	Classes/TiledMap.h
	Classes/UnitManager.h
	Classes/BuildingBase.h
	Classes/FighterUnitBase.h
	Classes/Unit.h
	Classes/MouseRect.h
	Classes/PathArithmetic.h
	Classes/BattleField_RTS.h
	Classes/GameMessageInterface.h
	Classes/LoadingScene.h
	Classes/GameMessageOperation.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(APP_SRC
    ${GAME_SOURCE}
    ${GAME_HEADER}
    )


set(PROTOBUFF_LIB_PATH Classes/lib/Debug/Protobuf)
set(_protobuff_prefix protobuff)
set(protobuff_INCLUDE_DIRS Classes/include/google/protobuf)
set(protobuff_LIBRARIES protobuff)
add_library(protobuff STATIC IMPORTED)
if(MACOSX)
set_target_properties(protobuff
			PROPERTIES IMPORTED_LOCATION
			${PROTOBUFF_LIB_PATH}/libprotobuf.a
)
elseif(WINDOWS)
set_target_properties(protobuff
		PROPERTIES IMPORTED_LOCATION
		"C:/github/BattleField_RTS/Classes/lib/Debug/Protobuf/libprotobuf.lib"
)

#LINK_DIRECTORIES(${PROTOBUFF_LIB_PATH})
endif()

set(BOOST_LIB_PATH Classes/lib/Debug/Boost)
set(_boost_prefix boost)
set(boost_INCLUDE_DIRS Classes/include/boost)
set(boost_LIBRARIES boost)
add_library(boost STATIC IMPORTED)
if(MACOSX)
set_target_properties(boost
			PROPERTIES IMPORTED_LOCATION
			${BOOST_LIB_PATH}/libboost_system.a
)
elseif(WINDOWS)
#set_target_properties(boost
#			PROPERTIES IMPORTED_LOCATION
#			C:\github\BattleField_RTS\Classes\lib\Debug\Boost\libboost_date_time-vc141-mt-gd-x32-1_67.lib
#)
link_directories(${BOOST_LIB_PATH})
endif()

if(MACOSX)
cocos_build_app(${APP_NAME}
                APP_SRC "${APP_SRC}"
				DEPEND_COMMON_LIBS "cocos2d" "protobuff" "boost"
                DEPEND_ANDROID_LIBS "cocos2d_android"
                )
elseif(WINDOWS)
cocos_build_app(${APP_NAME}
                APP_SRC "${APP_SRC}"
				DEPEND_COMMON_LIBS "cocos2d" "protobuff"	
                DEPEND_ANDROID_LIBS "cocos2d_android"
                )
endif()


if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME} COPY_TO ${APP_BIN_DIR})
endif()

if(LINUX OR WINDOWS)
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()