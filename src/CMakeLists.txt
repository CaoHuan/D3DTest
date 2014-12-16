# D3D个人练习管理
cmake_minimum_required(VERSION 2.6)

include(macros.cmake)

#解决方案名
project(Test)

# 项目生成调试信息
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /Oy- /MTd")


# 可以使用文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#在公司时用这个路径   TODO 用依赖包的形式做？
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


# 第一章
group(test01)

# 第二章
group(test02)

# 第三章
group(test03)
	addProject(tutorialCreate init.cpp)
	addProject(teapot teapot.cpp)
	addProject(triangle triangle.cpp)
	addProject(cube cube.cpp)
# 第四章
group(test04)
	addProject(colorTriangle colorTriangle.cpp)

# 第五章
group(test05)
	addProject(spotLight spotLight.cpp)
	addProject(pointLight pointLight.cpp)
	addProject(directionalLight directionalLight.cpp)
	addProject(litPyramid litPyramid.cpp)

# 第六章
group(test06)
	addProject(texQuad texQuad.cpp)
	addProject(texCube texCube.cpp)

group(test07)
	addProject(texAlpha texAlpha.cpp)