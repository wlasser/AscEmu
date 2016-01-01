# Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org>

if(APPLE)
	if(NOT IS_64BIT)
		set(EMPTY "")
		string(REGEX REPLACE ".*-m64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}")
		string(REGEX REPLACE ".*-m 64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}")
		string(REGEX REPLACE ".*-m64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
		string(REGEX REPLACE ".*-m 64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
		string(REGEX REPLACE ".*-m64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")
		string(REGEX REPLACE ".*-m 64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}")
		string(REGEX REPLACE ".*-m64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}")
		string(REGEX REPLACE ".*-m 64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}")		
		string(REGEX REPLACE ".*-m64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}")
		string(REGEX REPLACE ".*-m 64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}")
      
		add_definitions(-m32)
      
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -undefined dynamic_lookup")
		set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")
	endif(NOT IS_64BIT)
endif(APPLE )
