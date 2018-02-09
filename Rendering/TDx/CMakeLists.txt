set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

if (NOT VTK_USE_TDX)
  message( WARNING "vtkRenderingTDx is being built, "
           "while VTK_USE_TDX is not set to On. "
           "vtkChartsCore openturns features will not be available")
endif(NOT VTK_USE_TDX)

include_directories(${TDX_INCLUDE_DIR})

set(Module_SRCS
  vtkTDxDevice.cxx
  vtkTDxInteractorStyleGeo.cxx
  )

if(WIN32)
  # In vtkTDxWinDevice.cxx, #import "progid:TDxInput.Device.1" no_namespace
  # This sourcefile is incompatible with '/MP' flag.
  get_source_file_property(compile_props "${CMAKE_CURRENT_SOURCE_DIR}/vtkTDxWinDevice.cxx" COMPILE_FLAGS)
  if(NOT compile_props)
    set(compile_props "/MP1")
  else()
    string(REPLACE "/MP[0-9]*" " " compile_props ${compile_props})
  endif()
  set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/vtkTDxWinDevice.cxx" COMPILE_FLAGS "${compile_props}")

  list(APPEND Module_SRCS vtkTDxWinDevice.cxx)
endif()
if(UNIX)
  list(APPEND Module_SRCS vtkTDxUnixDevice.cxx)
endif()
if(APPLE)
  list(APPEND Module_SRCS vtkTDxMacDevice.cxx)
endif()

list(APPEND ${vtk-module}_SYSTEM_INCLUDE_DIRS ${TDX_INCLUDE_DIR})

vtk_module_library(${vtk-module} ${Module_SRCS})
target_compile_definitions(${vtk-module} PUBLIC -DVTK_USE_TDX)
