if (VTK_USE_TDX)
  set(tdx_module "vtkRenderingTDx")
endif(VTK_USE_TDX)

vtk_module(vtkRenderingOpenGL
  LEGACY 8.1 "Please switch to using `OpenGL2` VTK_RENDERING_BACKEND to replace this module."
  IMPLEMENTS
    vtkRenderingCore
  BACKEND
    OpenGL
  IMPLEMENTATION_REQUIRED_BY_BACKEND
  COMPILE_DEPENDS
    vtkParseOGLExt
    vtkUtilitiesEncodeString
  TEST_DEPENDS
    vtkInteractionStyle
    vtkTestingRendering
    vtkIOExport
    vtkIOLegacy
    vtkIOXML
    vtkRenderingImage
    vtkRenderingLOD
    vtkImagingGeneral
    vtkImagingSources
    vtkFiltersProgrammable
    vtkFiltersSources
    vtkRenderingAnnotation
  KIT
    vtkOpenGL
  DEPENDS
    vtkCommonCore
    vtkCommonDataModel
    vtkFiltersCore
    vtkRenderingCore
    ${tdx_module}
  PRIVATE_DEPENDS
    vtkCommonExecutionModel
    vtkCommonMath
    vtkCommonSystem
    vtkCommonTransforms
    vtkIOImage
    vtkImagingCore
    vtkImagingHybrid
    vtksys
  )