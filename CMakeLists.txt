cmake_minimum_required(VERSION 3.0)
project(OpenBlox)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(OpenBloxSources
	src/easywsclient/easywsclient.cpp
	src/enet/callbacks.c
	src/enet/compress.c
	src/enet/host.c
	src/enet/list.c
	src/enet/packet.c
	src/enet/peer.c
	src/enet/protocol.c
	src/enet/unix.c
	src/enet/win32.c
	src/lua/lapi.c
	src/lua/lauxlib.c
	src/lua/lbaselib.c
	src/lua/lcode.c
	src/lua/ldblib.c
	src/lua/ldebug.c
	src/lua/ldo.c
	src/lua/ldump.c
	src/lua/lfunc.c
	src/lua/lgc.c
	src/lua/linit.c
	src/lua/liolib.c
	src/lua/llex.c
	src/lua/lmathlib.c
	src/lua/lmem.c
	src/lua/loadlib.c
	src/lua/lobject.c
	src/lua/lopcodes.c
	src/lua/loslib.c
	src/lua/lparser.c
	src/lua/lstate.c
	src/lua/lstring.c
	src/lua/lstrlib.c
	src/lua/ltable.c
	src/lua/ltablib.c
	src/lua/ltm.c
	src/lua/lundump.c
	src/lua/lvm.c
	src/lua/lzio.c
	src/lua/print.c
	src/ob_enum/Enums.cpp
	src/ob_enum/LuaEnum.cpp
	src/ob_instance/BasePlayerGui.cpp
	src/ob_instance/Camera.cpp
	src/ob_instance/ContentProvider.cpp
	src/ob_instance/DataModel.cpp
	src/ob_instance/Frame.cpp
	src/ob_instance/GuiBase.cpp
	src/ob_instance/GuiBase2d.cpp
	src/ob_instance/GuiLabel.cpp
	src/ob_instance/GuiObject.cpp
	src/ob_instance/HttpService.cpp
	src/ob_instance/ImageLabel.cpp
	src/ob_instance/Instance.cpp
	src/ob_instance/LayerCollector.cpp
	src/ob_instance/NetworkPeer.cpp
	src/ob_instance/NetworkServer.cpp
	src/ob_instance/RunService.cpp
	src/ob_instance/ScreenGui.cpp
	src/ob_instance/ServiceProvider.cpp
	src/ob_instance/Sound.cpp
	src/ob_instance/StarterGui.cpp
	src/ob_type/CFrame.cpp
	src/ob_type/Color3.cpp
	src/ob_type/LuaEvent.cpp
	src/ob_type/UDim.cpp
	src/ob_type/UDim2.cpp
	src/ob_type/Vector2.cpp
	src/ob_type/Vector3.cpp
	src/ob_type/WebSocket.cpp
	src/openblox/AssetLocator.cpp
	src/openblox/BaseGame.cpp
	src/openblox/Factory.cpp
	src/openblox/oboslib.cpp
	src/openblox/OpenBlox.cpp
	src/openblox/OpenBloxRenderUtil.cpp
	src/openblox/SoundManager.cpp
	src/openblox/Thread.cpp
	src/openblox/ThreadScheduler.cpp
	src/openblox/Utility.cpp
	src/openblox/WindowUtils.cpp
	src/soil/image_DXT.c
	src/soil/image_helper.c
	src/soil/SOIL.c
	src/soil/stb_image_aug.c
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED filesystem iostreams system thread)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

set(OpenBloxLibraries ${OpenBloxLibraries} ${GLFW_LIBRARIES})
set(OpenBloxLibraries ${OpenBloxLibraries} ${GLEW_LIBRARIES})
set(OpenBloxLibraries ${OpenBloxLibraries} ${OPENGL_LIBRARIES})
set(OpenBloxLibraries ${OpenBloxLibraries} ${CURL_LIBRARIES})
set(OpenBloxLibraries ${OpenBloxLibraries} ${OPENSSL_LIBRARIES})
set(OpenBloxLibraries ${OpenBloxLibraries} ${Boost_LIBRARIES})
set(OpenBloxLibraries ${OpenBloxLibraries} fmodex) #note: fmodex is not an open source library, get it here: http://www.fmod.org/download-previous-products/

add_executable(openblox "${OpenBloxSources}")
target_link_libraries(openblox "${OpenBloxLibraries}")
set_target_properties(openblox PROPERTIES LINKER_LANGUAGE CXX)
