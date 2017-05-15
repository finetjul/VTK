if (VTK_USE_TDX)
  set(tdx_module "vtkRenderingTDx")
endif(VTK_USE_TDX)

vtk_module(vtkRenderingOpenGL2
  TCL_NAME
    vtkRenderingOpenGLII
  IMPLEMENTS
    vtkRenderingCore
  BACKEND
    OpenGL2
  IMPLEMENTATION_REQUIRED_BY_BACKEND
  COMPILE_DEPENDS
    vtkUtilitiesEncodeString
  TEST_DEPENDS
    vtkIOLegacy
    vtkRenderingImage
    vtkRenderingFreeType
    vtkTestingCore
    vtkTestingRendering
    vtkImagingGeneral
    vtkImagingSources
    vtkInteractionStyle
    vtkIOPLY
  KIT
    vtkOpenGL
  DEPENDS
    vtkCommonCore
    vtkCommonDataModel
    vtkRenderingCore
    ${tdx_module}
  PRIVATE_DEPENDS
    vtkCommonExecutionModel
    vtkCommonMath
    vtkCommonSystem
    vtkCommonTransforms
    vtkIOImage
    vtkImagingCore
    vtkglew
    vtksys
  )