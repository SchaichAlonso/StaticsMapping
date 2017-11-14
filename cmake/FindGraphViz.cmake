# this will set the following variables:
# graphviz_LIBRARIES
# graphviz_FOUND
# graphviz_INCLUDE_DIRECTORIES

if (NOT WIN32)
  find_package (PkgConfig)
  pkg_check_modules (graphviz ${REQUIRED} libgvc libcdt libcgraph libpathplan)
  if (graphviz_FOUND)
    set (graphviz_INCLUDE_DIRECTORIES ${graphviz_INCLUDE_DIRS})
  endif (graphviz_FOUND)
  
  set (graphviz_DEFINITIONS ${graphviz_CFLAGS_OTHER})
endif (NOT WIN32)

find_path (graphviz_INCLUDE_DIRECTORIES graphviz/gvc.h
  HINTS
    ${graphviz_INCLUDEDIR}
    ${graphviz_INCLUDE_DIRS}
  PATH_SUFFIXES
    graphviz)

foreach (libname gvc;cdt;cgraph;pathplan)
  find_library (graphviz_${libname}_LIBRARY
  NAMES
    ${libname}
  HINTS
   ${graphviz_LIBDIR}
   ${graphviz_LIBRARY_DIRS})
endforeach ()
    
if (graphviz_INCLUDE_DIRECTORIES
    AND graphviz_gvc_LIBRARY
    AND graphviz_cdt_LIBRARY
    AND graphviz_cgraph_LIBRARY
    AND graphviz_pathplan_LIBRARY)
  
  set (graphviz_FOUND 1)
  set (graphviz_LIBRARIES
      ${graphviz_gvc_LIBRARY}
      ${graphviz_cdt_LIBRARY}
      ${graphviz_cgraph_LIBRARY}
      ${graphviz_pathplan_LIBRARY})
      
else ()
  set (graphviz_FOUND 0)
endif ()

include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (GraphViz DEFAULT_MSG graphviz_LIBRARIES graphviz_INCLUDE_DIRECTORIES)
