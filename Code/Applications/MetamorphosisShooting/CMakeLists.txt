#
# Add the Metamorphosis shooting application
#
set(libraries ${FFTW_LIB} ${ITK_LIBRARIES})
add_clp_application(MetamorphosisShooting MetamorphosisShooting ${libraries})
