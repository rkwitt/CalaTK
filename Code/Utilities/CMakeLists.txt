#
# Add all sorts of utility functions
#
set(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
if(COMPILE_CLP_MODULES)
  add_clp_application(applyMap applyMap ${libraries})
  add_clp_application(InverseDeformationField InverseDeformationField ${libraries})
  add_clp_application(ImageReadWrite ImageReadWrite ${libraries})
endif()
