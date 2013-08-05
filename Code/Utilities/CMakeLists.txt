#
# Add all sorts of utility functions
#
set(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
if(COMPILE_CLP_MODULES)
  add_clp_application(applyMap applyMap ${libraries})
  add_clp_application(inverseDeformationField InverseDeformationField ${libraries})
  add_clp_application(imageReadWrite ImageReadWrite ${libraries})
  add_clp_application(warpImage WarpImage ${libraries})
  add_clp_application(createMask CreateMask ${libraries})
  add_clp_application(reSample ReSample ${libraries})
  add_clp_application(computeMaskDifference ComputeMaskDifference ${libraries})
  add_clp_application(removeMaskedRegion RemoveMaskedRegion ${libraries})
  add_clp_application(addVectorFields AddVectorFields ${libraries})
  add_clp_application(subtractVectorFields SubtractVectorFields ${libraries})
  add_clp_application(computeDisplacementFieldFromMap ComputeDisplacementFieldFromMap ${libraries})
endif()
