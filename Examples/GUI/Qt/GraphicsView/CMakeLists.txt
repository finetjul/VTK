include(vtkQt)

if(VTK_QT_VERSION VERSION_GREATER "4")
  find_package(Qt5Widgets REQUIRED QUIET)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
  add_definitions(${Qt5Widgets_DEFINITIONS})

  find_package(Qt5OpenGL REQUIRED QUIET)
  include_directories(${Qt5OpenGL_INCLUDE_DIRS})
  add_definitions(${Qt5OpenGL_DEFINITIONS})

  find_package(Qt5WebKit REQUIRED QUIET)
  include_directories(${Qt5WebKit})
  add_definitions(${Qt5WebKit_DEFINITIONS})

  set(QT_LIBRARIES
    ${Qt5Widgets_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
    ${Qt5WebKit_LIBRARIES}
    )
else()
  find_package(Qt4 REQUIRED)
  set(QT_USE_QTOPENGL 1)
  set(QT_USE_QTWEBKIT 1)
  include(${QT_USE_FILE})
endif()

find_package(OpenGL)

if(NOT VTK_BINARY_DIR)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif(NOT VTK_BINARY_DIR)

if(NOT VTK_USE_QVTK_QTOPENGL OR NOT QT_QTWEBKIT_FOUND OR QT_VERSION_MINOR LESS 6)
  message(STATUS "VTK isn't configured to use QtOpenGL, QtWebKit wasn't found, or Qt 4.6 wasn't found.  GraphicsView example is disabled.")
else(NOT VTK_USE_QVTK_QTOPENGL OR NOT QT_QTWEBKIT_FOUND OR QT_VERSION_MINOR LESS 6)

  set(ResourceFiles
    GraphicsView.qrc
    )
  set(MocHeaders
    OpenGLScene.hpp
    QBoolAnimation.h
    WebView.h
    )

  if(VTK_QT_VERSION VERSION_GREATER "4")
    qt5_add_resources(qrcfiles ${ResourceFiles})
    qt5_wrap_cpp(mocs ${MocHeaders})
  else()
    qt4_add_resources(qrcfiles ${ResourceFiles})
    qt4_wrap_cpp(mocs ${MocHeaders})
  endif()

  add_executable(qtgraphicsview
    main.cpp
    OpenGLScene.cpp
    TreeRingViewItem.cpp
    GraphLayoutViewItem.cpp
    WebView.cpp
    ${mocs}
    ${qrcfiles}
    )

  target_link_libraries(qtgraphicsview QVTK vtkRendering)
  target_link_libraries(qtgraphicsview ${QT_LIBRARIES})

endif(NOT VTK_USE_QVTK_QTOPENGL OR NOT QT_QTWEBKIT_FOUND OR QT_VERSION_MINOR LESS 6)
