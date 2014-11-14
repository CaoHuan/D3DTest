# D3D个人练习管理
#解决方案名
project(Test)
SET( CMAKE_BUILD_TYPE Debug )
cmake_minimum_required(VERSION 2.6)

# 项目生成调试信息
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /Oy- /MTd")


# 可以使用文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#在公司时用这个路径
set(INC_DIR "D:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include" )
set(LIB_DIR "D:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86")   

# 家里的路径
#set(LIB_DIR "E:/Directx11/Lib/x86")
#set(INC_DIR "E:/Directx11/Include")

# 添加d3d头文件包含目录
link_directories(${LIB_DIR})

#添加d3d库包含路径 这个路径必须在项目的上面， 不然添加不到
include_directories(${INC_DIR})

# unicode项目
add_definitions(-DUNICODE -D_UNICODE)

# 公共文件
#set(file d3dUtility.h d3dUtility.cpp frame.cpp)
set(BASE D3DBase.h D3DBase.cpp)

# 添加一个项目
add_executable(TutorialCreate WIN32 ${BASE} init.cpp)
set(projects TutorialCreate)


add_executable(Cube WIN32 ${BASE} cube.cpp)
set(projects ${projects} Cube)

add_executable(colorTriangle WIN32 ${BASE} colorTriangle.cpp)
set(projects ${projects} colorTriangle)

set_target_properties(${projects} PROPERTIES FOLDER test)