# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project (delaunay_for_DEM)

# 查找依赖库
find_package(CGAL COMPONENTS Qt5)

if(CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()

if ( CGAL_FOUND )

  create_single_source_cgal_program( "conforming_delaunay.cpp" )
  create_single_source_cgal_program( "delaunay.cpp" )

  if(CGAL_Qt5_FOUND)
    foreach(target
        conforming_delaunay
        delaunay)
      target_link_libraries(${target} PUBLIC CGAL::CGAL_Qt5)
    endforeach()  
  else()
    message(STATUS "NOTICE: The example draw_triangulation_2 requires Qt and will not be compiled.")
  endif()

else()

    message(STATUS "This program requires the CGAL library, and will not be compiled.")

endif()




