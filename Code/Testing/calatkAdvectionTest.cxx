/**
 * Tests advection of an image
 *
 */

#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "COneStepEvolverSemiLagrangianAdvection.h"
#include "CStationaryEvolver.h"

typedef double TFLOAT;
const unsigned int DIMENSION = 2;

int calatkAdvectionTest( int argc, char* argv[] )
{

  if ( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0] << " <original image> <advected image> <velx> <vely> <advection time>" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Input: " << argv[1] << std::endl;
  std::cout << "Output: " << argv[2] << std::endl;

  TFLOAT dvx = atof( argv[3] );
  std::cout << "Velocity in x direction" << dvx << std::endl;

  TFLOAT dvy = atof( argv[4] );
  std::cout << "Velocity in y direction" << dvy << std::endl;
  
  TFLOAT dTime = atof( argv[5] );
  std::cout << "Evolution time: " << dTime << std::endl;

  // some typedefs

  typedef CALATK::VectorImage< TFLOAT, DIMENSION > VectorImageType;
  typedef CALATK::VectorField< TFLOAT, DIMENSION > VectorFieldType;
  typedef CALATK::VectorImageUtils< TFLOAT, DIMENSION > VectorImageUtilsType;

  // create the method to take a simple step
  CALATK::COneStepEvolverSemiLagrangianAdvection< TFLOAT, DIMENSION > oneStepEvolver;

  // create an evolution class
  CALATK::CStationaryEvolver< TFLOAT, DIMENSION > evolver;
  evolver.SetOneStepEvolverPointer( &oneStepEvolver );

  // load the input image

  VectorImageType* pIm = VectorImageUtilsType::readFileITK( argv[1] );

  // create a vector field of the same dimension and initialize it
  // with the given velocities in the x and the y directions

  VectorFieldType* pV = new VectorFieldType( pIm );

  unsigned int sx = pV->getSizeX();
  unsigned int sy = pV->getSizeY();
  
  for ( unsigned int iI=0; iI<sx; ++iI )
    {
    for ( unsigned int iJ=0; iJ<sy; ++iJ )
      {
      pV->setX( iI, iJ, dvx );
      pV->setY( iI, iJ, dvy );
      }
    }

  // get memory for the output image and for the temporary image
  VectorImageType* pImOut = new VectorImageType( pIm );
  VectorImageType* pImTmp = new VectorImageType( pIm );

  // now evolve it

  evolver.SolveForward( pV, pIm, pImOut, pImTmp, dTime );
  
  VectorImageUtilsType::writeFileITK( pImOut, argv[2] );

  // free memory

  delete pImOut;
  delete pImTmp;
  delete pIm;
  delete pV;

  return EXIT_SUCCESS;

}