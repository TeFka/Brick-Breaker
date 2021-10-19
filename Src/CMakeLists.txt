# CMakeLists.txt--------------------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)


list(APPEND game_sources
    ${CMAKE_CURRENT_LIST_DIR}/BrickGameClass.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/Shader.cpp
	${CMAKE_CURRENT_LIST_DIR}/Texture.cpp
	${CMAKE_CURRENT_LIST_DIR}/Camera.cpp
	${CMAKE_CURRENT_LIST_DIR}/RenderEngine2D.cpp
	${CMAKE_CURRENT_LIST_DIR}/gameLevel.cpp
)

# /CMakeLists.txt-------------------------------------------------------------