option(VTK_USE_TDX "Use TDx interaction devices" OFF)
mark_as_advanced(VTK_USE_TDX)

vtk_module(vtkRenderingTDx
  DEPENDS
    vtkCommonCore
    vtkInteractionStyle
    vtkRenderingCore
  TEST_DEPENDS
    vtkTestingCore
    vtkTestingRendering
    vtkInteractionWidgets
    vtkIOPLY
    vtkIOXML
)
