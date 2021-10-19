# CMakeLists.txt--------------------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)


list(APPEND game_includes
    ${CMAKE_CURRENT_LIST_DIR}/BrickGameClass.h
    ${CMAKE_CURRENT_LIST_DIR}/Shader.h
	${CMAKE_CURRENT_LIST_DIR}/Texture.h
	${CMAKE_CURRENT_LIST_DIR}/Camera.h
	${CMAKE_CURRENT_LIST_DIR}/RenderEngine2D.h
	${CMAKE_CURRENT_LIST_DIR}/gameLevel.h
)

# /CMakeLists.txt-------------------------------------------------------------