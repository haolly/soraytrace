cmake_minimum_required (VERSION 2.6)
project (SORT)

include_directories( "${SORT_SOURCE_DIR}/src" )
include_directories( "${SORT_SOURCE_DIR}/src/thirdparty/ilmbase-1.0.2" )
include_directories( "${SORT_SOURCE_DIR}/src/thirdparty/zlib-1.2.5" )
include_directories( "${SORT_SOURCE_DIR}/src/thirdparty/openexr-1.7.0" )

macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

file(GLOB_RECURSE project_headers src/*.h)
file(GLOB_RECURSE project_cpps src/*.cpp)
file(GLOB_RECURSE project_cs src/*.c)

set(all_files ${project_headers} ${project_cpps} ${project_cs})
source_group_by_dir(all_files)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

add_executable(SORT ${all_files})

if(MSVC)
	set_target_properties( SORT PROPERTIES COMPILE_FLAGS "/Gz" )
endif(MSVC)