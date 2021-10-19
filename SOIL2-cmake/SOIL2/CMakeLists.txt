# CMakeLists.txt--------------------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)


list(APPEND soil2_sources
    ${CMAKE_CURRENT_LIST_DIR}/image_DXT.h
	${CMAKE_CURRENT_LIST_DIR}/image_DXT.c
	${CMAKE_CURRENT_LIST_DIR}/image_helper.h
	${CMAKE_CURRENT_LIST_DIR}/image_helper.c
	${CMAKE_CURRENT_LIST_DIR}/jo_jpeg.h
	${CMAKE_CURRENT_LIST_DIR}/pkm_helper.h
	${CMAKE_CURRENT_LIST_DIR}/pvr_helper.h
	${CMAKE_CURRENT_LIST_DIR}/SOIL2.h
	${CMAKE_CURRENT_LIST_DIR}/SOIL2.c
	${CMAKE_CURRENT_LIST_DIR}/stb_image.h
	${CMAKE_CURRENT_LIST_DIR}/stb_image_write.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_DDS.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_DDS_c.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_ext.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_ext_c.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_pkm.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_pkm_c.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_pvr.h
	${CMAKE_CURRENT_LIST_DIR}/stbi_pvr_c.h
	${CMAKE_CURRENT_LIST_DIR}/wfETC.h
	${CMAKE_CURRENT_LIST_DIR}/wfETC.c
)

# /CMakeLists.txt-------------------------------------------------------------