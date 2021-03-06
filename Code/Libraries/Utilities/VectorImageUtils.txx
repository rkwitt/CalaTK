/*
*
*  Copyright 2011, 2012 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

#ifndef VECTOR_IMAGE_UTILS_TXX
#define VECTOR_IMAGE_UTILS_TXX

//
// max
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::max( VectorImageType* in, T dMaxVal )
{
  unsigned int len = in->GetLength();

  for ( unsigned int iI=0; iI<len; ++iI )
  {
    T val = in->GetValue(iI);
    in->SetValue( iI, std::max( val, dMaxVal ) );
  }
}

//
// min
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::min( VectorImageType* in, T dMinVal )
{
  unsigned int len = in->GetLength();

  for ( unsigned int iI=0; iI<len; ++iI )
  {
    T val = in->GetValue(iI);
    in->SetValue( iI, std::min( val, dMinVal ) );
  }
}


//
// maxAll
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::maxAll( const VectorImageType* in)
{
  unsigned int len = in->GetLength();

  T m = in->GetValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = in->GetValue(iI);
    if (val > m)
      {
      m = val;
      }
    }

  return m;
}

//
// minAll
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::minAll( const VectorImageType* in)
{
  unsigned int len = in->GetLength();

  T m = in->GetValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = in->GetValue(iI);
    if (val < m)
      {
      m = val;
      }
    }

  return m;
}

//
// absMaxAll
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::absMaxAll( const VectorImageType* in)
{
  unsigned int len = in->GetLength();

  T m = in->GetValue( 0 );

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    T val = std::abs( in->GetValue(iI) );
    if (val > m)
      {
      m = val;
      }
    }

  return m;
}

//
// sumAll
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::sumAll( const VectorImageType* in)
{
  unsigned int len = in->GetLength();

  T s = 0;

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    s += in->GetValue( iI );
    }

  return s;
}

//
// abs
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::abs( VectorImageType* in)
{
  unsigned int len = in->GetLength();

  for ( unsigned int iI=0; iI<len; ++iI )
    {
    in->SetValue( iI, std::abs( in->GetValue( iI ) ) );
    }
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, T scale )
{

  assert( scale > 0.0 );

  // will only be approximate scale (up to the same integer)
  const unsigned int szxOrig = imageGraft->GetSizeX();
  const unsigned int szyOrig = imageGraft->GetSizeY();
  const unsigned int szzOrig = imageGraft->GetSizeZ();

  unsigned int szxDesired = 0;
  unsigned int szyDesired = 0;
  unsigned int szzDesired = 0;

  switch ( VImageDimension )
    {
    case 1:
      szxDesired = (unsigned int)ceil( scale*szxOrig );
      return AllocateMemoryForScaledVectorImage( imageGraft, szxDesired );
      break;
    case 2:
      szxDesired = (unsigned int)ceil( scale*szxOrig );
      szyDesired = (unsigned int)ceil( scale*szyOrig );
      return AllocateMemoryForScaledVectorImage( imageGraft, szxDesired, szyDesired );
      break;
    case 3:
      szxDesired = (unsigned int)ceil( scale*szxOrig );
      szyDesired = (unsigned int)ceil( scale*szyOrig );
      szzDesired = (unsigned int)ceil( scale*szzOrig );
      return AllocateMemoryForScaledVectorImage( imageGraft, szxDesired, szyDesired, szzDesired );
      break;
    default:
      std::runtime_error("Unsupported dimension for memory allocation.");
    }

  return NULL;
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, unsigned int szx )
{
  const unsigned int dim = imageGraft->GetDimension();
  const unsigned int szxOrig = imageGraft->GetSizeX();

  T dxOrig = imageGraft->GetSpacingX();

  T invScaleX = (T)szxOrig/(T)szx;

  // An exception to the rule -- since we an generating this new image, we do
  // not assign to a smart pointer.
  VectorImageType * newImage = new VectorImageType( szx, dim );
  newImage->SetSpacingX( dxOrig*invScaleX );
  newImage->SetOrigin( imageGraft->GetOrigin() );
  newImage->SetDirection( imageGraft->GetDirection() );

  return newImage;
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, unsigned int szx, unsigned int szy )
{
  const unsigned int dim = imageGraft->GetDimension();
  const unsigned int szxOrig = imageGraft->GetSizeX();
  const unsigned int szyOrig = imageGraft->GetSizeY();

  const T dxOrig = imageGraft->GetSpacingX();
  const T dyOrig = imageGraft->GetSpacingY();

  const T invScaleX = (T)szxOrig/(T)szx;
  const T invScaleY = (T)szyOrig/(T)szy;

  // An exception to the rule -- since we an generating this new image, we do
  // not assign to a smart pointer.
  VectorImageType * newImage = new VectorImageType( szx, szy, dim );
  newImage->SetSpacingX( dxOrig*invScaleX );
  newImage->SetSpacingY( dyOrig*invScaleY );
  newImage->SetOrigin( imageGraft->GetOrigin() );
  newImage->SetDirection( imageGraft->GetDirection() );

  return newImage;
}

//
// AllocateMemoryForScaledVectorImage
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::AllocateMemoryForScaledVectorImage( const VectorImageType* imageGraft, unsigned int szx, unsigned int szy, unsigned int szz )
{
  const unsigned int dim = imageGraft->GetDimension();
  const unsigned int szxOrig = imageGraft->GetSizeX();
  const unsigned int szyOrig = imageGraft->GetSizeY();
  const unsigned int szzOrig = imageGraft->GetSizeZ();

  const T dxOrig = imageGraft->GetSpacingX();
  const T dyOrig = imageGraft->GetSpacingY();
  const T dzOrig = imageGraft->GetSpacingZ();

  const T invScaleX = (T)szxOrig/(T)szx;
  const T invScaleY = (T)szyOrig/(T)szy;
  const T invScaleZ = (T)szzOrig/(T)szz;

  // An exception to the rule -- since we an generating this new image, we do
  // not assign to a smart pointer.
  VectorImageType * newImage = new VectorImageType( szx, szy, szz, dim );
  newImage->SetSpacingX( dxOrig*invScaleX );
  newImage->SetSpacingY( dyOrig*invScaleY );
  newImage->SetSpacingZ( dzOrig*invScaleZ );
  newImage->SetOrigin( imageGraft->GetOrigin() );
  newImage->SetDirection( imageGraft->GetDirection() );

  return newImage;
}

//
// interpolatePos 1D
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates( const VectorImageType1D* imIn, T xPos, unsigned int d)
{

  unsigned int szXold = imIn->GetSizeX();

  // make sure there are no out-of bounds issues
  xPos = MAX(0,xPos);
  xPos = MIN(szXold-1,xPos);

  // interpolate the coordinates
  unsigned int x1 = (unsigned int)xPos;
  unsigned int x2 = MIN(x1+1,szXold-1);
  T dx = xPos - x1;

  // compute the interpolated value
  return (1-dx)*( imIn->GetValue(x1, d) )
        + dx*( imIn->GetValue(x2, d) );
}

//
// interpolatePos 2D
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates( const VectorImageType2D* imIn, T xPos, T yPos, unsigned int d)
{

  unsigned int szXold = imIn->GetSizeX();
  unsigned int szYold = imIn->GetSizeY();

  // make sure there are no out-of bounds issues
  xPos = MAX(0,xPos);
  xPos = MIN(szXold-1,xPos);
  yPos = MAX(0,yPos);
  yPos = MIN(szYold-1,yPos);

  // interpolate the coordinates
  unsigned int x1 = (unsigned int)xPos;
  unsigned int x2 = MIN(x1+1,szXold-1);
  T dx = xPos - x1;

  unsigned int y1 = (unsigned int)yPos;
  unsigned int y2 = MIN(y1+1,szYold-1);
  T dy = yPos - y1;

  // compute the interpolated value
  return (1-dy)*((1-dx)*(imIn->GetValue(x1, y1, d))
        + dx*(imIn->GetValue(x2, y1, d)))
     + dy*((1-dx)*(imIn->GetValue(x1, y2, d))
        + dx*(imIn->GetValue(x2, y2, d)));

}

//
// interpolatePos 3D
//
template <class T, unsigned int VImageDimension >
T VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates( const VectorImageType3D* imIn, T xPos, T yPos, T zPos, unsigned int d)
{

  unsigned int szXold = imIn->GetSizeX();
  unsigned int szYold = imIn->GetSizeY();
  unsigned int szZold = imIn->GetSizeZ();

  // make sure there are no out-of bounds issues
  xPos = MAX(0,xPos);
  xPos = MIN(szXold-1,xPos);
  yPos = MAX(0,yPos);
  yPos = MIN(szYold-1,yPos);
  zPos = MAX(0,zPos);
  zPos = MIN(szZold-1,zPos);

  //*
  unsigned int x1 = (unsigned int)xPos;
  unsigned int x2 = MIN(x1+1,szXold-1);
  T dx = xPos - x1;

  unsigned int y1 = (unsigned int)yPos;
  unsigned int y2 = MIN(y1+1,szYold-1);
  T dy = yPos - y1;

  unsigned int z1 = (unsigned int)zPos;
  unsigned int z2 = MIN(z1+1,szZold-1);
  T dz = zPos - z1;

  // compute the interpolated value
  return (1-dz)*((1-dy)*((1-dx)*(imIn->GetValue(x1, y1, z1, d))
          + dx*(imIn->GetValue(x2, y1, z1, d)))
       + dy*((1-dx)*(imIn->GetValue(x1, y2, z1, d))
          + dx*(imIn->GetValue(x2, y2, z1, d))))
    + dz *((1-dy)*((1-dx)*(imIn->GetValue(x1, y1, z2, d))
          + dx*(imIn->GetValue(x2, y1, z2, d)))
       + dy*((1-dx)*(imIn->GetValue(x1, y2, z2, d))
          + dx*(imIn->GetValue(x2, y2, z2, d))));

}

//
// interpolate 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::interpolate( const VectorImageType3D* imIn, const VectorImageType3D* pos, VectorImageType3D* imOut, unsigned int uiNrOfThreads )
{

  int dim = (int)imIn->GetDimension();

  int szXnew = (int)pos->GetSizeX();
  int szYnew = (int)pos->GetSizeY();
  int szZnew = (int)pos->GetSizeZ();

  T dx = imIn->GetSpacingX();
  T dy = imIn->GetSpacingY();
  T dz = imIn->GetSpacingZ();

#ifdef DEBUG
  if (pos->GetDimension() != 3) {
    throw std::invalid_argument("VectorImageTypeUtils::resize -> invalid pos image");
  }
#endif

  // using linear interpolation for now
#pragma omp parallel for schedule(static) num_threads( uiNrOfThreads )
  for ( int z = 0; z < szZnew; ++z)
    {
    for ( int y = 0; y < szYnew; ++y)
      {
      for ( int x = 0; x < szXnew; ++x)
        {
        // interpolate the coordinates from the grid coordinates assuming origin 0
        // TODO: Add support for origin different than 0 here
        T xPos = pos->GetValue(x,y,z,0)/dx;
        T yPos = pos->GetValue(x,y,z,1)/dy;
        T zPos = pos->GetValue(x,y,z,2)/dz;

        for ( int d = 0; d < dim; ++d)
          {
          // set the new value
          T val = VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(imIn, xPos, yPos, zPos, d);
          imOut->SetValue(x,y,z,d, val);
          }
        }
      }
    }
}

//
// interpolate 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::interpolate( const VectorImageType2D* imIn, const VectorImageType2D* pos, VectorImageType2D* imOut, unsigned int uiNrOfThreads )
{

  int dim = (int)imIn->GetDimension();

  int szXnew = (int)pos->GetSizeX();
  int szYnew = (int)pos->GetSizeY();

  T dx = imIn->GetSpacingX();
  T dy = imIn->GetSpacingY();

#ifdef DEBUG
  if (pos->GetDimension() != 2)
    {
    throw std::invalid_argument("VectorImageTypeUtils::resize -> invalid pos image");
    }
#endif

  // using linear interpolation for now
#pragma omp parallel for schedule(static) num_threads( uiNrOfThreads )
  for ( int y = 0; y < szYnew; ++y)
    {
    for ( int x = 0; x < szXnew; ++x)
      {
      // interpolate the coordinates from the grid coordinates assuming origin 0
      // TODO: Add support for origin different than 0 here

      T xPos = pos->GetValue(x,y,0)/dx;
      T yPos = pos->GetValue(x,y,1)/dy;

      for ( int d = 0; d < dim; ++d)
        {
        // set the new value
        T val = VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(imIn, xPos, yPos, d);
        imOut->SetValue(x,y,d, val);
        }
      }
    }
}

//
// interpolate 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::interpolate( const VectorImageType1D* imIn, const VectorImageType1D* pos, VectorImageType1D* imOut, unsigned int uiNrOfThreads )
{

  int dim = (int)imIn->GetDimension();

  int szXnew = (int)pos->GetSizeX();

  T dx = imIn->GetSpacingX();

#ifdef DEBUG
  if (pos->GetDimension() != 1)
    {
    throw std::invalid_argument("VectorImageTypeUtils::resize -> invalid pos image");
    }
#endif

  // using linear interpolation for now
#pragma omp parallel for schedule(static) num_threads( uiNrOfThreads )
  for ( int x = 0; x < szXnew; ++x)
    {
    // interpolate the coordinates from the grid coordinates assuming origin 0
    // TODO: Add support for origin different than 0 here

    T xPos = pos->GetValue(x,0)/dx;

    for ( int d = 0; d < dim; ++d)
      {
      // set the new value
      T val = VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(imIn, xPos, d);
      imOut->SetValue(x,d, val);
      }
    }
}

//
// interpolateNegativeVelocityPos 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::interpolateNegativeVelocityPos( const VectorImageType3D* imIn, const VectorFieldType3D* v, T dt, VectorImageType3D* imOut, unsigned int uiNrOfThreads )
{

  int dim = (int)imIn->GetDimension();

  int szXnew = (int)imIn->GetSizeX();
  int szYnew = (int)imIn->GetSizeY();
  int szZnew = (int)imIn->GetSizeZ();

  T dx = imIn->GetSpacingX();
  T dy = imIn->GetSpacingY();
  T dz = imIn->GetSpacingZ();

  T dt_div_dx = dt/dx;
  T dt_div_dy = dt/dy;
  T dt_div_dz = dt/dz;

  // using linear interpolation for now
#pragma omp parallel for schedule(static) num_threads( uiNrOfThreads )
  for ( int z = 0; z < szZnew; ++z)
    {
    for ( int y = 0; y < szYnew; ++y)
      {
      for ( int x = 0; x < szXnew; ++x)
        {

        // interpolate the coordinates from the grid coordinates assuming origin 0
        // TODO: Add support for origin different than 0 here
        T xPos = x - v->GetX(x,y,z)*dt_div_dx;
        T yPos = y - v->GetY(x,y,z)*dt_div_dy;
        T zPos = z - v->GetZ(x,y,z)*dt_div_dz;

        for ( int d = 0; d < dim; ++d)
          {
          // set the new value
          T val = VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(imIn, xPos, yPos, zPos, d);
          imOut->SetValue(x,y,z,d, val);
          }
        }
      }
    }
}

//
// interpolateNegativeVelocityPos 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::interpolateNegativeVelocityPos( const VectorImageType2D* imIn, const VectorFieldType2D* v, T dt, VectorImageType2D* imOut, unsigned int uiNrOfThreads )
{

  int dim = (int)imIn->GetDimension();

  int szXnew = (int)imIn->GetSizeX();
  int szYnew = (int)imIn->GetSizeY();

  T dx = imIn->GetSpacingX();
  T dy = imIn->GetSpacingY();

  T dt_div_dx = dt/dx;
  T dt_div_dy = dt/dy;

  // using linear interpolation for now
#pragma omp parallel for schedule(static) num_threads( uiNrOfThreads )
  for ( int y = 0; y < szYnew; ++y)
    {
    for ( int x = 0; x < szXnew; ++x)
      {

      // interpolate the coordinates from the grid coordinates assuming origin 0
      // TODO: Add support for origin different than 0 here

      T xPos = x - v->GetX(x,y)*dt_div_dx;
      T yPos = y - v->GetY(x,y)*dt_div_dy;

      for ( int d = 0; d < dim; ++d)
        {
        // set the new value
        T val = VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(imIn, xPos, yPos, d);
        imOut->SetValue(x,y,d, val);
        }

      }
    }
}

//
// interpolateNegativeVelocityPos 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::interpolateNegativeVelocityPos( const VectorImageType1D* imIn, const VectorFieldType1D* v, T dt, VectorImageType1D* imOut, unsigned int uiNrOfThreads )
{

  int dim = (int)imIn->GetDimension();

  int szXnew = (int)imIn->GetSizeX();

  T dx = imIn->GetSpacingX();
  T dt_div_dx = dt/dx;

  // using linear interpolation for now
#pragma omp parallel for schedule(static) num_threads( uiNrOfThreads )
  for ( int x = 0; x < szXnew; ++x)
    {
    // interpolate the coordinates from the grid coordinates assuming origin 0
    // TODO: Add support for origin different than 0 here

    T xPos = x - v->GetX(x)*dt_div_dx;

    for ( int d = 0; d < dim; ++d)
      {
      // set the new value
      T val = VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(imIn, xPos, d);
      imOut->SetValue(x,d, val);
      }
    }
}


template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::resize( const VectorImageType1D* imIn, VectorImageType1D* imOut)
{

  unsigned int szXnew = imOut->GetSizeX();

  T dx = imOut->GetSpacingX();

  assert( szXnew>0 );

  typename VectorImageType::Pointer pos = new VectorImageType(szXnew, 1);

  // create the interpolation maps
  for (unsigned int x = 0; x < szXnew; ++x)
    {
    // perform the interpolation
    pos->SetValue(x,0, (T)x*dx);
    }

  // interpolate
  VectorImageUtils< T, VImageDimension >::interpolate(imIn, pos, imOut);
}


template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::resize( const VectorImageType2D* imIn, VectorImageType2D* imOut)
{

  unsigned int szXnew = imOut->GetSizeX();
  unsigned int szYnew = imOut->GetSizeY();

  T dx = imOut->GetSpacingX();
  T dy = imOut->GetSpacingY();

  assert( szXnew>0 );
  assert( szYnew>0 );

  typename VectorImageType::Pointer pos = new VectorImageType(szXnew, szYnew, 2);

  // create the interpolation maps
  for (unsigned int y = 0; y < szYnew; ++y)
    {
    for (unsigned int x = 0; x < szXnew; ++x)
      {
        // perform the interpolation
        pos->SetValue(x,y,0, (T)x*dx);
        pos->SetValue(x,y,1, (T)y*dy);
      }
    }

  // interpolate
  VectorImageUtils< T, VImageDimension >::interpolate(imIn, pos, imOut);
}


template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::resize( const VectorImageType3D* imIn, VectorImageType3D* imOut)
{

  unsigned int szXnew = imOut->GetSizeX();
  unsigned int szYnew = imOut->GetSizeY();
  unsigned int szZnew = imOut->GetSizeZ();

  T dx = imOut->GetSpacingX();
  T dy = imOut->GetSpacingY();
  T dz = imOut->GetSpacingZ();

  typename VectorImageType::Pointer pos = new VectorImageType(szXnew, szYnew, szZnew, 3);

  // create the interpolation maps
  for (unsigned int z = 0; z < szZnew; ++z)
    {
    for (unsigned int y = 0; y < szYnew; ++y)
      {
      for (unsigned int x = 0; x < szXnew; ++x)
        {
          // perform the interpolation
          pos->SetValue(x,y,z,0, (T)x*dx);
          pos->SetValue(x,y,z,1, (T)y*dy);
          pos->SetValue(x,y,z,2, (T)z*dz);
        }
      }
    }

  // interpolate
  VectorImageUtils< T, VImageDimension >::interpolate(imIn, pos, imOut);
}

//
// normalize
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::normalize(VectorImageType* imInOut, T min, T max)
{

  T minC = VectorImageUtils< T, VImageDimension >::minAll(imInOut);
  T maxC = VectorImageUtils< T, VImageDimension >::maxAll(imInOut);

  T minDiff = min - minC;
  imInOut->AddConstant(minDiff);

  T fact = (max - min) / (maxC - minC);
  imInOut->MultiplyByConstant(fact);

}

//
// normalize
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::normalizeClampNegativeMaxOne(VectorImageType* imInOut )
{

  VectorImageUtils::max( imInOut, 0 );
  T maxC = VectorImageUtils< T, VImageDimension >::maxAll( imInOut );

  imInOut->MultiplyByConstant( 1.0/maxC );
}


//
// meanPixelwise
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::meanPixelwise(std::vector<VectorImageType*> imList, unsigned int numIms, VectorImageType* imOut)
{
  unsigned len = imOut->GetLength();

  unsigned int szX = imOut->GetSizeX();
  unsigned int szY = imOut->GetSizeY();
  unsigned int szZ = imOut->GetSizeZ();
  unsigned int dim = imOut->GetDimension();

#ifdef DEBUG
  // make sure things are the right size
  for (unsigned int i = 0; i < numIms) {
  if (imList[i]->GetSizeX() != szX || imList[i]->GetSizeY() != szY || imList[i]->GetSizeZ() != szZ || imList[i]->GetDimension() != dim) {
    throw std::invalid_argument("VectorImageUtils< T, VImageDimension >::meanPixelwise -> output image size does not match");
  }
  }
#endif

  for (unsigned int ind = 0; ind<len; ++ind )
    {
    T m = 0;
    for (unsigned int i = 0; i < numIms; i++)
      {
      m += imList[i]->GetValue(ind);
      }
    m /= numIms;
    imOut->SetValue(ind, m);
    }
}

//
// multiplyVectorByImageDimensionInPlace, 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::multiplyVectorByImageDimensionInPlace( const VectorImageType1D* imIn, unsigned int dim, VectorImageType1D* imOut )
{
  unsigned int szX = imIn->GetSizeX();
  unsigned int outDim = imOut->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int d=0; d<outDim; ++d )
      {
      imOut->SetValue( x, d, imOut->GetValue( x, d )*imIn->GetValue( x, dim ) );
      }
    }
}

//
// multiplyVectorByImageDimensionInPlace, 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::multiplyVectorByImageDimensionInPlace( const VectorImageType2D* imIn, unsigned int dim, VectorImageType2D* imOut )
{
  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  unsigned int outDim = imOut->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int d=0; d<outDim; ++d )
        {
        imOut->SetValue( x, y, d, imOut->GetValue( x, y, d )*imIn->GetValue( x, y, dim ) );
        }
      }
    }
}

//
// multiplyVectorByImageDimensionInPlace, 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::multiplyVectorByImageDimensionInPlace( const VectorImageType3D* imIn, unsigned int dim, VectorImageType3D* imOut )
{
  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  unsigned int szZ = imIn->GetSizeZ();
  unsigned int outDim = imOut->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int z=0; z<szZ; ++z )
        {
        for ( unsigned int d=0; d<outDim; ++d )
          {
          imOut->SetValue( x, y, z, d, imOut->GetValue( x, y, z, d )*imIn->GetValue( x, y, z, dim ) );
          }
        }
      }
    }
}

//
// addScalarImageToVectorImageAtDimensionInPlace, 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( const VectorImageType1D* imIn, VectorImageType1D* imOut, unsigned int dim )
{
  assert( imIn->GetDimension()==1 );
  assert( imOut->GetDimension()>dim );

  unsigned int szX = imIn->GetSizeX();

  for ( unsigned int x=0; x<szX; ++x )
    {
    imOut->SetValue( x, dim, imOut->GetValue( x, dim )+imIn->GetValue( x, 0 ) );
    }
}

//
// addScalarImageToVectorImageAtDimensionInPlace, 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( const VectorImageType2D* imIn, VectorImageType2D* imOut, unsigned int dim )
{
  assert( imIn->GetDimension()==1 );
  assert( imOut->GetDimension()>dim );

  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
        imOut->SetValue( x, y, dim, imOut->GetValue( x, y, dim )+imIn->GetValue( x, y, 0 ) );
      }
    }
}

//
// addScalarImageToVectorImageAtDimensionInPlace, 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::addScalarImageToVectorImageAtDimensionInPlace( const VectorImageType3D* imIn, VectorImageType3D* imOut, unsigned int dim )
{
  assert( imIn->GetDimension()==1 );
  assert( imOut->GetDimension()>dim );

  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  unsigned int szZ = imIn->GetSizeZ();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int z=0; z<szZ; ++z )
        {
          imOut->SetValue( x, y, z, dim, imOut->GetValue( x, y, z, dim )+imIn->GetValue( x, y, z, 0 ) );
        }
      }
    }
}

//
// multiplyVectorByVectorInnerProductElementwise, 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::multiplyVectorByVectorInnerProductElementwise( const VectorImageType1D* im1, const VectorImageType1D* im2, VectorImageType1D* imOut )
{
  assert( im1->GetDimension()==im2->GetDimension() );
  assert( imOut->GetDimension()==1 );

  unsigned int szX = im1->GetSizeX();
  unsigned dim = im1->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    T dInnerProduct = 0;
    for ( unsigned d = 0; d<dim; ++d )
      {
      dInnerProduct += im1->GetValue( x, d )*im2->GetValue( x, d );
      }
    imOut->SetValue( x, 0, dInnerProduct );
    }
}

//
// multiplyVectorByVectorInnerProductElementwise, 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::multiplyVectorByVectorInnerProductElementwise( const VectorImageType2D* im1, const VectorImageType2D* im2, VectorImageType2D* imOut )
{
  assert( im1->GetDimension()==im2->GetDimension() );
  assert( imOut->GetDimension()==1 );

  unsigned int szX = im1->GetSizeX();
  unsigned int szY = im1->GetSizeY();
  unsigned dim = im1->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      T dInnerProduct = 0;
      for ( unsigned d = 0; d<dim; ++d )
       {
        dInnerProduct += im1->GetValue( x, y, d )*im2->GetValue( x, y, d );
       }
       imOut->SetValue( x, y, 0, dInnerProduct );
      }
    }
}

//
// multiplyVectorByVectorInnerProductElementwise, 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension>::multiplyVectorByVectorInnerProductElementwise( const VectorImageType3D* im1, const VectorImageType3D* im2, VectorImageType3D* imOut )
{
  assert( im1->GetDimension()==im2->GetDimension() );
  assert( imOut->GetDimension()==1 );

  unsigned int szX = im1->GetSizeX();
  unsigned int szY = im1->GetSizeY();
  unsigned int szZ = im1->GetSizeZ();
  unsigned dim = im1->GetDimension();

  for ( unsigned int x=0; x<szX; ++x )
    {
    for ( unsigned int y=0; y<szY; ++y )
      {
      for ( unsigned int z=0; z<szZ; ++z )
        {
        T dInnerProduct = 0;
        for ( unsigned d = 0; d<dim; ++d )
          {
          dInnerProduct += im1->GetValue( x, y, z, d )*im2->GetValue( x, y, z, d );
         }
        imOut->SetValue( x, y, z, 0, dInnerProduct );
        }
      }
    }
}

//
// apply ITK affine 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::applyAffineITK(typename ITKAffineTransform<T,1>::Type* itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue, T originX)
{

  // TODO: Check that this method works appropriately

  unsigned int dim = imIn->GetDimension();

#ifdef DEBUG
  unsigned int szX = imIn->GetSizeX();
  if (imOut->GetSizeX() != szX || imOut->GetDimension() != dim) {
  throw std::invalid_argument("VectorImage2DUtils::applyAffineITK -> output image size does not match");
  }
#endif

  // convert the first dim for setup
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm = VectorImageUtils< T, VImageDimension>::convertDimToITK(imIn, 0);
  typename ITKImage<T,VImageDimension>::Type::PointType origin;
  origin[0] = originX;
  itkIm->SetOrigin(origin);

  // set up the resampler
  typename ITKResampleFilter<T,VImageDimension>::Type::Pointer resampler = ITKResampleFilter<T,VImageDimension>::Type::New();
  resampler->SetTransform( itkAffine );
  resampler->SetSize( itkIm->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin(  itkIm->GetOrigin() );
  resampler->SetOutputSpacing( itkIm->GetSpacing() );
  resampler->SetOutputDirection( itkIm->GetDirection() );
  resampler->SetDefaultPixelValue( defaultPixelValue );

  for (unsigned int d = 0; d < dim; ++d)
    {
    // convert to an itk image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDim = VectorImageUtils< T, VImageDimension>::convertDimToITK(imIn, d);
    itkDim->SetOrigin(origin);

    // apply the affine transform
    resampler->SetInput( itkDim );
    resampler->Update();

    // copy the data into the output
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDimTrans = resampler->GetOutput();
    VectorImageUtils< T, VImageDimension>::convertDimFromITK(itkDimTrans.GetPointer(), d, imOut);
    }

}

//
// apply ITK affine 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::applyAffineITK(typename ITKAffineTransform<T,2>::Type* itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue, T originX, T originY)
{

  // TODO: Check that this method works appropriately

  unsigned int dim = imIn->GetDimension();

#ifdef DEBUG
  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  if (imOut->GetSizeX() != szX || imOut->GetSizeY() != szY || imOut->GetDimension() != dim) {
  throw std::invalid_argument("VectorImage2DUtils::applyAffineITK -> output image size does not match");
  }
#endif

  // convert the first dim for setup
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm = VectorImageUtils< T, VImageDimension>::convertDimToITK(imIn, 0);
  typename ITKImage<T,VImageDimension>::Type::PointType origin;
  origin[0] = originX;
  origin[1] = originY;
  itkIm->SetOrigin(origin);

  // set up the resampler
  typename ITKResampleFilter<T,VImageDimension>::Type::Pointer resampler = ITKResampleFilter<T,VImageDimension>::Type::New();
  resampler->SetTransform( itkAffine );
  resampler->SetSize( itkIm->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin(  itkIm->GetOrigin() );
  resampler->SetOutputSpacing( itkIm->GetSpacing() );
  resampler->SetOutputDirection( itkIm->GetDirection() );
  resampler->SetDefaultPixelValue( defaultPixelValue );

  for (unsigned int d = 0; d < dim; ++d)
    {

    // convert to an itk image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDim = VectorImageUtils< T, VImageDimension>::convertDimToITK(imIn, d);
    itkDim->SetOrigin(origin);

    // apply the affine transform
    resampler->SetInput( itkDim );
    resampler->Update();

    // copy the data into the output
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDimTrans = resampler->GetOutput();
    VectorImageUtils< T, VImageDimension>::convertDimFromITK(itkDimTrans.GetPointer(), d, imOut);
  }

}

//
// apply ITK affine 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::applyAffineITK(typename ITKAffineTransform<T,3>::Type* itkAffine, VectorImageType* imIn, VectorImageType* imOut, T defaultPixelValue, T originX, T originY, T originZ ) {

  unsigned int dim = imIn->GetDimension();

#ifdef DEBUG
  unsigned int szX = imIn->GetSizeX();
  unsigned int szY = imIn->GetSizeY();
  unsigned int szZ = imIn->GetSizeZ();
  if (imOut->GetSizeX() != szX || imOut->GetSizeY() != szY || imOut->GetSizeZ() != szZ || imOut->GetDimension() != dim) {
  throw std::invalid_argument("VectorImageUtils< T, VImageDimension >::applyAffineITK -> output image size does not match");
  }
#endif

  // convert the first dim for setup
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm = VectorImageUtils< T, VImageDimension >::convertDimToITK(imIn, 0);
  typename ITKImage<T,VImageDimension>::Type::PointType origin;
  origin[0] = originX;
  origin[1] = originY;
  origin[2] = originY;
  itkIm->SetOrigin(origin);

  // set up the resampler
  typedef itk::LinearInterpolateImageFunction< typename ITKImage<T,VImageDimension>::Type, T> ResampleInterpolatorType;
  typename ResampleInterpolatorType::Pointer interpolator = ResampleInterpolatorType::New();

  typename ITKResampleFilter<T,VImageDimension>::Type::Pointer resampler = ITKResampleFilter<T,VImageDimension>::Type::New();
  resampler->SetTransform( itkAffine );
  resampler->SetInterpolator ( interpolator );
  resampler->SetOutputParametersFromImage ( itkIm );
  resampler->SetDefaultPixelValue( defaultPixelValue );


  for (unsigned int d = 0; d < dim; ++d)
    {

    // convert to an itk image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDim = VectorImageUtils< T, VImageDimension >::convertDimToITK(imIn, d);
    itkDim->SetOrigin(origin);

    // apply the affine transform
    resampler->SetInput( itkDim );
    resampler->Update();

    // copy the data into the output
    typename ITKImage<T,VImageDimension>::Type::Pointer itkDimTrans = resampler->GetOutput();
    VectorImageUtils< T, VImageDimension >::convertDimFromITK(itkDimTrans.GetPointer(), d, imOut);
  }

}

template < class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::IsSameSize( const VectorImageType* im1, const VectorImageType* im2)
{

  if ( im1 == NULL || im2 == NULL )
  {
    return false;
  }

  bool bIsSameSize =
      ( im1->GetSizeX() == im2->GetSizeX() ) &&
      ( im1->GetSizeY() == im2->GetSizeY() ) &&
      ( im1->GetSizeZ() == im2->GetSizeZ() ) &&
      ( im1->GetDimension() == im2->GetDimension() );

  return bIsSameSize;

}


/////////////////////////////
// ITK Interface Functions //
/////////////////////////////

//
// to ITK (int)
//
template <class T, unsigned int VImageDimension >
typename ITKCharImage2D::Pointer VectorImageUtils< T, VImageDimension>::convertToITKChar( const VectorImage<T,2>* im)
{

  unsigned int szX = im->GetSizeX();
  unsigned int szY = im->GetSizeY();
  unsigned int dim = im->GetDimension();

  // make sure we're only dealing with one dimension
  if (dim != 1)
    {
    throw std::invalid_argument("VectorImage2DUtils::convertToITKint -> image has more than 1 vector dimension");
    }

  // Initialize ITK image
  typename ITKCharImage2D::Pointer outImage;
  outImage = ITKCharImage2D::New();

  // Set up region
  typename ITKCharImage2D::IndexType start;
  start[0] = 0;
  start[1] = 0;

  typename ITKCharImage2D::SizeType size;
  size[0] = szX;
  size[1] = szY;

  typename ITKCharImage2D::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKCharImage2D::SpacingType space;
  space[0] = im->GetSpacingX();
  space[1] = im->GetSpacingY();
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // normalize the image onto 0-255
  typename VectorImage<T,2>::Pointer imCopy = new VectorImage<T,2>(im);
  typedef VectorImageUtils<T,2> VectorImageUtilsType;

  VectorImageUtilsType::normalize(imCopy, 0, 255);

  // Copy in the data
  for (unsigned int y = 0; y < szY; ++y)
    {
    for (unsigned int x = 0; x < szX; ++x)
      {
      typename ITKCharImage2D::IndexType px;
      px[0] = x;
      px[1] = y;

      outImage->SetPixel(px, (unsigned char)imCopy->GetValue(x,y,0));
      }
    }

  // Set origin and direction
  outImage->SetOrigin(VectorImageUtilsType::convertITKVectorOrigin(im->GetOrigin()));
  outImage->SetDirection(VectorImageUtilsType::convertITKVectorDirection(im->GetDirection()));

  // return the result
  return outImage;

}

//
// to ITK 1D
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::convertToITK( const VectorImageType1D* im)
{

  if ( VImageDimension != 1 )
    {
    throw std::runtime_error( "convertToITK1D only for 1D images." );
    }

  unsigned int szX = im->GetSizeX();
  unsigned int dim = im->GetDimension();

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer outImage;
  outImage = ITKVectorImage<T,VImageDimension>::Type::New();

  // Set up region
  typename ITKVectorImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;

  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = dim;

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->GetSpacingX();
  space[1] = 1;
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // Copy in the data
  for (unsigned int x = 0; x < szX; ++x)
    {
    for (unsigned int d = 0; d < dim; ++d)
      {

      typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
      px[0] = x;
      px[1] = d;

      outImage->SetPixel(px, im->GetValue(x,d));
      }
    }

  // Set origin and direction
  outImage->SetOrigin(im->GetOrigin());
  outImage->SetDirection(im->GetDirection());

  return outImage;

}

//
// to ITK 2D
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::convertToITK( const VectorImageType2D* im)
{

  if ( VImageDimension != 2 )
    {
    throw std::runtime_error( "convertToITK2D only for 3D images." );
    }


  unsigned int szX = im->GetSizeX();
  unsigned int szY = im->GetSizeY();
  unsigned int dim = im->GetDimension();

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer outImage;
  outImage = ITKVectorImage<T,VImageDimension>::Type::New();

  // Set up region
  typename ITKVectorImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = dim;

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->GetSpacingX();
  space[1] = im->GetSpacingY();
  space[2] = 1;
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // Copy in the data
  for (unsigned int y = 0; y < szY; ++y)
    {
    for (unsigned int x = 0; x < szX; ++x)
      {
      for (unsigned int d = 0; d < dim; ++d)
        {

        typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
        px[0] = x;
        px[1] = y;
        px[2] = d;

        outImage->SetPixel(px, im->GetValue(x,y,d));
        }
      }
    }

  // Set origin and direction
  outImage->SetOrigin(im->GetOrigin());
  outImage->SetDirection(im->GetDirection());

  return outImage;

}

//
// to ITK 3D
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::convertToITK( const VectorImageType3D* im)
{

  if ( VImageDimension != 3 )
    {
    throw std::runtime_error( "convertToITK3D only for 3D images." );
    }

  unsigned int szX = im->GetSizeX();
  unsigned int szY = im->GetSizeY();
  unsigned int szZ = im->GetSizeZ();
  unsigned int dim = im->GetDimension();

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer outImage;
  outImage = ITKVectorImage<T,VImageDimension>::Type::New();

  // Set up region
  typename ITKVectorImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  start[3] = 0;

  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = szZ;
  size[3] = dim;

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->GetSpacingX();
  space[1] = im->GetSpacingY();
  space[2] = im->GetSpacingZ();
  space[3] = 1;
  outImage->SetSpacing(space);

  // Allocate region to image
  outImage->SetRegions(region);
  outImage->Allocate();

  // Copy in the data
  for (unsigned int z = 0; z < szZ; ++z)
    {
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {
        for (unsigned int d = 0; d < dim; ++d)
          {

          typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = z;
          px[3] = d;

          outImage->SetPixel(px, im->GetValue(x,y,z,d));
          }
        }
      }
    }

  // Set origin and direction
  outImage->SetOrigin(im->GetOrigin());
  outImage->SetDirection(im->GetDirection());

  // return the result
  return outImage;

}

//
// to ITK (dim), 1D
//
template <class T, unsigned int VImageDimension >
typename ITKImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::convertDimToITK( const VectorImageType1D* im, unsigned int dimIn)
{

  unsigned int szX = im->GetSizeX();

#ifdef DEBUG
  unsigned int dim = im->GetDimension();
  if (dimIn > dim) {
    std::cerr << "VectorImage1DUtils::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
  }
#endif

  // initialize itk image
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm;
  itkIm = ITKImage<T,VImageDimension>::Type::New();

  // Set up start
  typename ITKImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;

  // Set up size
  typename ITKImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;

  // Set up region
  typename ITKImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->GetSpacingX();
  itkIm->SetSpacing(space);

  // Allocate region to image
  itkIm->SetRegions(region);
  itkIm->Allocate();

  // Copy in the data
  for (unsigned int x = 0; x < szX; ++x)
    {
    typename ITKImage<T,VImageDimension>::Type::IndexType idx;
    idx[0] = x;

    typename ITKImage<T,VImageDimension>::Type::PixelType px;

    px = im->GetValue(x,dimIn);

    itkIm->SetPixel(idx, px);
    }

  // Set origin and direction
  itkIm->SetOrigin(convertITKVectorOrigin(im->GetOrigin()));
  itkIm->SetDirection(convertITKVectorDirection(im->GetDirection()));

  return itkIm;

}

//
// to ITK (dim), 2D
//
template <class T, unsigned int VImageDimension >
typename ITKImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::convertDimToITK( const VectorImageType2D* im, unsigned int dimIn)
{

  unsigned int szX = im->GetSizeX();
  unsigned int szY = im->GetSizeY();

#ifdef DEBUG
  unsigned int dim = im->GetDimension();
  if (dimIn > dim) {
    std::cerr << "VectorImage2DUtils::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
  }
#endif

  // initialize itk image
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm;
  itkIm = ITKImage<T,VImageDimension>::Type::New();

  // Set up start
  typename ITKImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;

  // Set up size
  typename ITKImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;

  // Set up region
  typename ITKImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->GetSpacingX();
  space[1] = im->GetSpacingY();
  itkIm->SetSpacing(space);

  // Allocate region to image
  itkIm->SetRegions(region);
  itkIm->Allocate();

  // Copy in the data
  for (unsigned int y = 0; y < szY; ++y)
    {
    for (unsigned int x = 0; x < szX; ++x)
      {

      typename ITKImage<T,VImageDimension>::Type::IndexType idx;
      idx[0] = x;
      idx[1] = y;

      typename ITKImage<T,VImageDimension>::Type::PixelType px;

      px = im->GetValue(x,y,dimIn);

      itkIm->SetPixel(idx, px);
      }
    }

  // Set origin and direction
  itkIm->SetOrigin(convertITKVectorOrigin(im->GetOrigin()));
  itkIm->SetDirection(convertITKVectorDirection(im->GetDirection()));

  return itkIm;

}

//
// to ITK (dim), 3D
//
template <class T, unsigned int VImageDimension >
typename ITKImage<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::convertDimToITK( const VectorImageType3D* im, unsigned int dimIn)
{

  unsigned int szX = im->GetSizeX();
  unsigned int szY = im->GetSizeY();
  unsigned int szZ = im->GetSizeZ();

#ifdef DEBUG
  unsigned int dim = im->GetDimension();
  if (dimIn > dim)
    {
    std::cerr << "VectorImageUtils< T, VImageDimension >::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
    }
#endif

  // initialize itk image
  typename ITKImage<T,VImageDimension>::Type::Pointer itkIm;
  itkIm = ITKImage<T,VImageDimension>::Type::New();

  // Set up start
  typename ITKImage<T,VImageDimension>::Type::IndexType start;
  start[0] = 0;
  start[1] = 0;
  start[2] = 0;

  // Set up size
  typename ITKImage<T,VImageDimension>::Type::SizeType size;
  size[0] = szX;
  size[1] = szY;
  size[2] = szZ;

  // Set up region
  typename ITKImage<T,VImageDimension>::Type::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Set up the spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space;
  space[0] = im->GetSpacingX();
  space[1] = im->GetSpacingY();
  space[2] = im->GetSpacingZ();
  itkIm->SetSpacing(space);

  // Allocate region to image
  itkIm->SetRegions(region);
  itkIm->Allocate();

  // Copy in the data
  for (unsigned int z = 0; z < szZ; ++z)
    {
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {

        typename ITKImage<T,VImageDimension>::Type::IndexType idx;
        idx[0] = x;
        idx[1] = y;
        idx[2] = z;

        typename ITKImage<T,VImageDimension>::Type::PixelType px;

        px = im->GetValue(x,y,z,dimIn);

        itkIm->SetPixel(idx, px);
        }
      }
    }

  // Set origin and direction
  itkIm->SetOrigin(convertITKVectorOrigin(im->GetOrigin()));
  itkIm->SetDirection(convertITKVectorDirection(im->GetDirection()));

  return itkIm;

}

//
// from ITK, 1D
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::convertFromITK( typename ITKVectorImage<T,1>::Type* itkIm)
{
  if ( VImageDimension!= 1 )
    {
    throw std::runtime_error( "ConvertFromITK1D can only be used in 1D." );
    }

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region = itkIm->GetLargestPossibleRegion();
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();

  unsigned int szX = size[0];
  unsigned int dim = size[1];

  // An exception to the rule -- since we an generating this new image, we do
  // not assign to a smart pointer.
  VectorImageType * outImage = new VectorImageType(szX, dim);
  outImage->SetSpacingX( space[0] );

  for (unsigned int x = 0; x < szX; ++x)
    {
    for (unsigned int d = 0; d < dim; ++d)
      {
      typename ITKVectorImage<T,VImageDimension>::Type::IndexType idx;
      idx[0] = x;
      idx[1] = d;
      T dCurrentElement = itkIm->GetPixel( idx );
      outImage->SetValue(x,d, dCurrentElement );
      }
    }

  // Set origin and direction
  outImage->SetOrigin(itkIm->GetOrigin());
  outImage->SetDirection(itkIm->GetDirection());

  return outImage;
}


//
// from ITK, 2D
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::convertFromITK( typename ITKVectorImage<T,2>::Type* itkIm)
{
  if ( VImageDimension!= 2 )
    {
    throw std::runtime_error( "ConvertFromITK2D can only be used in 2D." );
    }

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region = itkIm->GetLargestPossibleRegion();
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();

  unsigned int szX = size[0];
  unsigned int szY = size[1];
  unsigned int dim = size[2];

  // An exception to the rule -- since we an generating this new image, we do
  // not assign to a smart pointer.
  VectorImageType * outImage = new VectorImageType(szX, szY, dim );
  outImage->SetSpacingX( space[0] );
  outImage->SetSpacingY( space[1] );

  for (unsigned int y = 0; y < szY; ++y)
    {
    for (unsigned int x = 0; x < szX; ++x)
      {
      for (unsigned int d = 0; d < dim; ++d)
        {
        typename ITKVectorImage<T,VImageDimension>::Type::IndexType idx;
        idx[0] = x;
        idx[1] = y;
        idx[2] = d;
        T dCurrentElement = itkIm->GetPixel( idx );
        outImage->SetValue(x,y,d, dCurrentElement );
        }
      }
    }

  // Set origin and direction
  outImage->SetOrigin(itkIm->GetOrigin());
  outImage->SetDirection(itkIm->GetDirection());

  return outImage;
}


//
// from ITK, 3D
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension>::VectorImageType*
VectorImageUtils< T, VImageDimension >::convertFromITK( typename ITKVectorImage<T,3>::Type* itkIm)
{

  if ( VImageDimension!= 3 )
    {
    throw std::runtime_error( "ConvertFromITK3D can only be used in 3D." );
    }

  typename ITKVectorImage<T,VImageDimension>::Type::RegionType region = itkIm->GetLargestPossibleRegion();
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  typename ITKVectorImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  unsigned int szX = size[0];
  unsigned int szY = size[1];
  unsigned int szZ = size[2];
  unsigned int dim = size[3];

  // An exception to the rule -- since we an generating this new image, we do
  // not assign to a smart pointer.
  VectorImageType * outImage = new VectorImageType(szX, szY, szZ, dim );
  outImage->SetSpacingX( space[0] );
  outImage->SetSpacingY( space[1] );
  outImage->SetSpacingZ( space[2] );

  for (unsigned int z = 0; z < szZ; ++z)
    {
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {
        for (unsigned int d = 0; d < dim; ++d)
          {

          typename ITKVectorImage<T,VImageDimension>::Type::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = z;
          px[3] = d;

          outImage->SetValue(x,y,z,d, itkIm->GetPixel(px));
          }
        }
      }
    }

  // Set origin and direction
  outImage->SetOrigin(itkIm->GetOrigin());
  outImage->SetDirection(itkIm->GetDirection());

  return outImage;
}

//
// from ITK (dim), 1D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::convertDimFromITK( typename ITKImage<T,1>::Type* itkIm, unsigned int dimIn, VectorImageType1D* imOut)
{

  unsigned int szX = imOut->GetSizeX();

#ifdef DEBUG
  unsigned int dim = imOut->GetDimension();
  if (dimIn > dim) {
  std::cerr << "VectorImageDUtils::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
  dimIn = dim-1;
  }
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  if (size[0] != szX) {
    throw std::invalid_argument("VectorImageDUtils::convertDimFromITK -> size mismatch");
  }
#endif

  // copy the data
  for (unsigned int x = 0; x < szX; ++x)
    {

    typename ITKImage<T,VImageDimension>::Type::IndexType px;
    px[0] = x;
    imOut->SetValue(x,dimIn, itkIm->GetPixel(px));
    }

  // Set original spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  imOut->SetSpacingX(space[0]);

  // Set origin and direction
  imOut->SetOrigin(convertITKOriginToVector(itkIm->GetOrigin()));
  imOut->SetDirection(convertITKDirectionToVector(itkIm->GetDirection()));
}


//
// from ITK (dim), 2D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::convertDimFromITK( typename ITKImage<T,2>::Type* itkIm, unsigned int dimIn, VectorImageType2D* imOut)
{

  unsigned int szX = imOut->GetSizeX();
  unsigned int szY = imOut->GetSizeY();

#ifdef DEBUG
  unsigned int dim = imOut->GetDimension();
  if (dimIn > dim) {
  std::cerr << "VectorImage2DUtils::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
  dimIn = dim-1;
  }
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  if (size[0] != szX || size[1] != szY) {
    throw std::invalid_argument("VectorImage2DUtils::convertDimFromITK -> size mismatch");
  }
#endif

  // copy the data
  for (unsigned int y = 0; y < szY; ++y)
    {
    for (unsigned int x = 0; x < szX; ++x)
      {

      typename ITKImage<T,VImageDimension>::Type::IndexType px;
      px[0] = x;
      px[1] = y;
      imOut->SetValue(x,y,dimIn, itkIm->GetPixel(px));
      }
    }

  // Set original spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  imOut->SetSpacingX(space[0]);
  imOut->SetSpacingY(space[1]);

  // Set origin and direction
  imOut->SetOrigin(convertITKOriginToVector(itkIm->GetOrigin()));
  imOut->SetDirection(convertITKDirectionToVector(itkIm->GetDirection()));
}

//
// from ITK (dim), 3D
//
template <class T, unsigned int VImageDimension >
void VectorImageUtils< T, VImageDimension >::convertDimFromITK( typename ITKImage<T,3>::Type* itkIm, unsigned int dimIn, VectorImageType3D* imOut)
{

  unsigned int szX = imOut->GetSizeX();
  unsigned int szY = imOut->GetSizeY();
  unsigned int szZ = imOut->GetSizeZ();

#ifdef DEBUG
  unsigned int dim = imOut->GetDimension();
  if (dimIn > dim) {
    std::cerr << "VectorImageUtils< T, VImageDimension >::convertDimToITK -> Warning: invalid dim.  Using dim = " << dim-1 << "." << std::endl;
    dimIn = dim-1;
  }
  typename ITKVectorImage<T,VImageDimension>::Type::SizeType size = region.GetSize();
  if (size[0] != szX || size[1] != szY || size[2] != szZ) {
    throw std::invalid_argument("VectorImageUtils< T, VImageDimension >::convertDimFromITK -> size mismatch");
  }
#endif

  // copy the data
  for (unsigned int z = 0; z < szZ; ++z)
    {
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {

        typename ITKImage<T,VImageDimension>::Type::IndexType px;
        px[0] = x;
        px[1] = y;
        px[2] = z;
        imOut->SetValue(x,y,z,dimIn, itkIm->GetPixel(px));
        }
      }
    }

  // Set original spacing
  typename ITKImage<T,VImageDimension>::Type::SpacingType space = itkIm->GetSpacing();
  imOut->SetSpacingX(space[0]);
  imOut->SetSpacingY(space[1]);
  imOut->SetSpacingZ(space[2]);


  // Set origin and direction
  imOut->SetOrigin(convertITKOriginToVector(itkIm->GetOrigin()));
  imOut->SetDirection(convertITKDirectionToVector(itkIm->GetDirection()));
}

//
// convertITKVectorOrigin
//
template <class T, unsigned int VImageDimension >
typename ITKImage<T,VImageDimension>::Type::PointType VectorImageUtils< T, VImageDimension >::convertITKVectorOrigin( typename ITKVectorImage<T,VImageDimension>::Type::PointType originIn)
{
  typename ITKImage<T,VImageDimension>::Type::PointType out;
  for (unsigned int i = 0; i < out.Size(); ++i)
    {
    out[i] = originIn[i];
    }
  return out;
}

//
// convertITKVectorDirection
//
template <class T, unsigned int VImageDimension >
typename ITKImage<T,VImageDimension>::Type::DirectionType
VectorImageUtils< T, VImageDimension >::
convertITKVectorDirection( typename ITKVectorImage<T,VImageDimension>::Type::DirectionType directionIn) {
  typename ITKImage< T, VImageDimension>::Type::DirectionType out;
  for (unsigned int r = 0; r < ITKImage< T, VImageDimension>::Type::DirectionType::RowDimensions; ++r)
    {
    for (unsigned int c = 0; c < ITKImage< T, VImageDimension>::Type::DirectionType::ColumnDimensions; ++c)
      {
      out[r][c] = directionIn[r][c];
      }
    }
  return out;
}

//
// convertITKOriginToVector
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage<T,VImageDimension>::Type::PointType VectorImageUtils< T, VImageDimension >::convertITKOriginToVector( typename ITKImage<T,VImageDimension>::Type::PointType originIn)
{
  typename ITKVectorImage< T, VImageDimension>::Type::PointType out;
  out.Fill(0);
  for (unsigned int i = 0; i < originIn.Size(); ++i)
    {
    out[i] = originIn[i];
    }
  return out;
}

//
// convertITKDirectionToVector
//
template <class T, unsigned int VImageDimension >
typename ITKVectorImage<T,VImageDimension>::Type::DirectionType
VectorImageUtils< T, VImageDimension >::
convertITKDirectionToVector( typename ITKImage<T,VImageDimension>::Type::DirectionType directionIn)
{
  typename ITKVectorImage<T,VImageDimension>::Type::DirectionType out;
  out.Fill(0);
  for (unsigned int r = 0; r < ITKImage<T,VImageDimension>::Type::DirectionType::RowDimensions; ++r)
    {
    for (unsigned int c = 0; c < ITKImage<T,VImageDimension>::Type::DirectionType::ColumnDimensions; ++c)
      {
      out[r][c] = directionIn[r][c];
      }
    }
  out[VImageDimension][VImageDimension] = 1;
  return out;
}

//////////////////
// IO Functions //
//////////////////

//
// writeCommaSeparated
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeTextFile(VectorImageType* im, const std::string& filename) {


  std::ofstream writer;
  writer.open(filename.c_str());

  unsigned int toZ = 1;
  if ( VImageDimension==3 ) toZ = im->GetSizeZ();

  if(writer.is_open()) {
    /*
     * Formatting: Each dimension is enclosed by {} and will be written as an individual
     * matrix.  Within each matrix, elements are seperated by commas (,) and rows are
     * seperated by newlines (\n)
     */
    for (unsigned int d = 0; d < im->GetDimension(); d++)
      {

      // open bracket to signify beginning of matrix
      writer << "{\n";

      for (unsigned int z = 0; z < toZ; z++)
        {

        // open brace to signify beginning of slice
        writer << "[\n";

        for (unsigned int y = 0; y < im->GetSizeY(); y++)
          {
          for (unsigned int x = 0; x < im->GetSizeX(); x++)
            {

            // write the element out

            if ( VImageDimension==3 )
              {
              writer << im->GetValue(x,y,z,d);
              }
            else if ( VImageDimension==2 )
              {
              writer << im->GetValue(x,y,d);
              }

            // write a comma if not the end of a row
            if (x < im->GetSizeX() - 1) {
              writer << " ";
            }

          }

        // newline for end of row
        writer << '\n';
        }

        // closed brace to signifiy end of slice
      writer << "]\n";
      }

      // closed bracket to signify end of matrix
      writer << "}\n";
    }

    writer.close();

    return true;
  } else {
    return false;
  }

}

//
// writeFileITK, 1D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeFileITK(const VectorImageType1D* im, const std::string& filename)
{
  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
  itkImage = VectorImageUtils<T,VImageDimension>::convertToITK(im);

  // Initialize ITK writer
  typename ITKVectorImageWriter<T,VImageDimension>::Type::Pointer vectorImageWriter = ITKVectorImageWriter<T,VImageDimension>::Type::New();
  vectorImageWriter->SetFileName(filename.c_str());
  vectorImageWriter->SetInput(itkImage);
  vectorImageWriter->UseCompressionOn();

  // Try to write the image out
  try
    {
    vectorImageWriter->Update();
  }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
    std::cerr << err << std::endl;
    return false;
    }

  return true;
}



//
// writeFileITK, 2D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeFileITK(const VectorImageType2D* im, const std::string& filename)
{

  //
  // If 1-channel and trying to write a char-type file, convert to grayscale uchar
  //
  if (im->GetDimension() == 1 &&
      (ApplicationUtils::endsWith(filename, ".png") ||
       ApplicationUtils::endsWith(filename, ".PNG") ||
       ApplicationUtils::endsWith(filename, ".jpg") ||
       ApplicationUtils::endsWith(filename, ".jpeg") ||
       ApplicationUtils::endsWith(filename, ".JPG") ||
       ApplicationUtils::endsWith(filename, ".JPEG")) )
    {

    // Convert to Char
    typename ITKCharImage2D::Pointer itkImage = VectorImageUtils::convertToITKChar(im);

    // Initialize ITK writer
    typename ITKCharImageWriter2D::Pointer imageWriter = ITKCharImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }

  //
  // If 3-channel and trying to write a char-type file, convert to RGB uchar
  //
  else if (im->GetDimension() == 3 &&
           (ApplicationUtils::endsWith(filename, ".png") ||
            ApplicationUtils::endsWith(filename, ".PNG") ||
            ApplicationUtils::endsWith(filename, ".jpg") ||
            ApplicationUtils::endsWith(filename, ".jpeg") ||
            ApplicationUtils::endsWith(filename, ".JPG") ||
            ApplicationUtils::endsWith(filename, ".JPEG")) )
    {

    typedef itk::RGBPixel<unsigned char> RGBPx;
    typedef itk::Image< RGBPx, 2 > ColorImageType;
    typedef itk::ImageFileWriter< ColorImageType > ColorWriterType;

    //
    // convert to ITK RGB image
    //
    unsigned int szX = im->GetSizeX();
    unsigned int szY = im->GetSizeY();

    // Initialize ITK image
    typename ColorImageType::Pointer outImage;
    outImage = ColorImageType::New();

    // Set up region
    typename ColorImageType::IndexType start;
    start[0] = 0;
    start[1] = 0;

    typename ColorImageType::SizeType size;
    size[0] = szX;
    size[1] = szY;

    typename ColorImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ColorImageType::SpacingType space;
    space[0] = im->GetSpacingX();
    space[1] = im->GetSpacingY();
    outImage->SetSpacing(space);

    // Allocate region to image
    outImage->SetRegions(region);
    outImage->Allocate();

    // normalize the image onto 0-255
    typename VectorImageType::Pointer imNormalized = new VectorImageType( im );

    typedef VectorImageUtils<T,2> VectorImageUtilsType;

    VectorImageUtilsType::normalize(imNormalized, 0, 255);

    // Copy in the data
    for (unsigned int y = 0; y < szY; ++y)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {

        typename ColorImageType::IndexType px;
        px[0] = x;
        px[1] = y;

        RGBPx val;
        val[0] = (unsigned char)imNormalized->GetValue(x,y,0);
        val[1] = (unsigned char)imNormalized->GetValue(x,y,1);
        val[2] = (unsigned char)imNormalized->GetValue(x,y,2);

        outImage->SetPixel(px, val);
        }
      }

    // Set origin and direction

    std::cout << "WARNING: needs to be put back in." << std::endl;

    outImage->SetOrigin(VectorImageUtilsType::convertITKVectorOrigin(im->GetOrigin()));
    outImage->SetDirection(VectorImageUtilsType::convertITKVectorDirection(im->GetDirection()));

    // Initialize ITK writer
    typename ColorWriterType::Pointer colorImageWriter = ColorWriterType::New();
    colorImageWriter->SetFileName(filename.c_str());
    colorImageWriter->SetInput(outImage);
    colorImageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      colorImageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
    }

  }

  //
  // If single vector dimension, compress to a 2D scalar image
  //
  else if (im->GetDimension() == 1) {

/*
    // Get image information
    unsigned int szX = im->GetSizeX();
    unsigned int szY = im->GetSizeY();
    // Set up output image
    ITKImage2D::Pointer outVecIm = ITKImage2D::New();
    ITKImage2D::RegionType region;
    ITKImage2D::SizeType size;
    ITKImage2D::IndexType start;
    ITKImage2D::SpacingType spacing;
    size[0] = szX;
    size[1] = szY;
    start.Fill(0);
    region.SetSize(size);
    region.SetIndex(start);
    spacing[0] = im->GetSpacingX();
    spacing[1] = im->GetSpacingY();
    outVecIm->SetRegions(region);
    outVecIm->SetSpacing(spacing);
    outVecIm->SetOrigin(convertITKVectorOriginTo2D(im->GetOrigin()));
    outVecIm->SetDirection(convertITKVectorDirectionTo2D(im->GetDirection()));
    outVecIm->Allocate();

    // Copy the data
    for (unsigned int y = 0; y < szY; ++y) {
      for (unsigned int x = 0; x < szX; ++x) {
        ITKImage2D::IndexType idx = {{x,y}};
        outVecIm->SetPixel(idx, im->GetValue(x,y,0));
      }
    }
*/
  typename ITKImage<T,VImageDimension>::Type::Pointer scalarIm = convertDimToITK(im, 0);

  // Write out the image
  typename ITKImageWriter<T,VImageDimension>::Type::Pointer writer = ITKImageWriter<T,VImageDimension>::Type::New();
  writer->SetInput(scalarIm);
  writer->SetFileName(filename);
  writer->UseCompressionOn();
  try {
  writer->Update();
  }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
    std::cerr << err << std::endl;
    return false;
    }

  }

  //
  // If all other conditions fail, just do the normal writing procedure
  //
  else
    {

    // Initialize ITK image
    typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
    itkImage = VectorImageUtils<T,VImageDimension>::convertToITK(im);

    // Initialize ITK writer
    typename ITKVectorImageWriter<T,VImageDimension>::Type::Pointer vectorImageWriter = ITKVectorImageWriter<T,VImageDimension>::Type::New();
    vectorImageWriter->SetFileName(filename.c_str());
    vectorImageWriter->SetInput(itkImage);
    vectorImageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      vectorImageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage2DUtils::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }

  return true;
}


//
// writeFileITK, 3D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeFileITK( const VectorImageType3D* im, const std::string& filename)
{

  //
  // If vector dim = 1, compress to a scalar 3D image
  //
  if (im->GetDimension() == 1)
    {

    // Initialize ITK image
    typename ITKImage<T,VImageDimension>::Type::Pointer itkImage;
    itkImage = VectorImageUtils< T, VImageDimension >::convertDimToITK(im, 0);

    // Initialize ITK writer
    typename ITKImageWriter<T,VImageDimension>::Type::Pointer writer = ITKImageWriter<T,VImageDimension>::Type::New();
    writer->SetFileName(filename.c_str());
    writer->SetInput(itkImage);
    writer->UseCompressionOn();

    // Try to write the image out
    try
      {
      writer->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImageUtils< T, VImageDimension >::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }

  //
  // Otherwise, just write out as a 4D image
  //
  else
    {

    // Initialize ITK image
    typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
    itkImage = VectorImageUtils< T, VImageDimension >::convertToITK(im);

    // Initialize ITK writer
    typename ITKVectorImageWriter<T,VImageDimension>::Type::Pointer writer = ITKVectorImageWriter<T,VImageDimension>::Type::New();
    writer->SetFileName(filename.c_str());
    writer->SetInput(itkImage);
    writer->UseCompressionOn();

    // Try to write the image out
    try
      {
      writer->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImageUtils< T, VImageDimension >::writeFileITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }
    }

  return true;
}

//
// writeTimeDependantImagesITK, 1D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK1D( const std::vector< VectorImageType* >* ims, const std::string& filename)
{
  // see if we're dealing with one or more dimensions
  if ( (*ims)[0]->GetDimension() == 1)
    {
    typedef itk::Image<T, 3> ITKTimeImage1D;
    typedef itk::ImageFileWriter< ITKTimeImage1D >  ITKTimeImageWriter1D;

    // Initialize ITK image
    typename ITKTimeImage1D::Pointer itkImage;
    itkImage = ITKTimeImage1D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->GetSizeX();

    // Set up region
    typename ITKTimeImage1D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    typename ITKTimeImage1D::SizeType size;
    size[0] = szX;
    size[1] = ims->size();

    typename ITKTimeImage1D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage1D::SpacingType space;
    space[0] = (*ims)[0]->GetSpacingX();
    space[1] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {
        typename ITKTimeImage1D::IndexType px;
        px[0] = x;
        px[1] = t;

        itkImage->SetPixel(px, (*ims)[t]->GetValue(x,0));
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter1D::Pointer imageWriter = ITKTimeImageWriter1D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage1DUtils::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    }
  else
    {
    typedef itk::Image<T, 4> ITKTimeImage1D;
    typedef itk::ImageFileWriter< ITKTimeImage1D >  ITKTimeImageWriter1D;

    // Initialize ITK image
    typename ITKTimeImage1D::Pointer itkImage;
    itkImage = ITKTimeImage1D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->GetSizeX();
    unsigned int dim = (*ims)[0]->GetDimension();

    // Set up region
    typename ITKTimeImage1D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;

    typename ITKTimeImage1D::SizeType size;
    size[0] = szX;
    size[1] = dim;
    size[2] = ims->size();

    typename ITKTimeImage1D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage1D::SpacingType space;
    space[0] = (*ims)[0]->GetSpacingX();
    space[1] = 1.0;
    space[2] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t)
      {
      for (unsigned int x = 0; x < szX; ++x)
        {
        for (unsigned int d = 0; d < dim; ++d)
          {
          typename ITKTimeImage1D::IndexType px;
          px[0] = x;
          px[1] = d;
          px[2] = t;

          itkImage->SetPixel(px, (*ims)[t]->GetValue(x,d));
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter1D::Pointer imageWriter = ITKTimeImageWriter1D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage1DUtils::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    }

  return true;
}


//
// writeTimeDependantImagesITK, 2D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK2D( const std::vector< VectorImageType* >* ims, const std::string& filename)
{
  // see if we're dealing with one or more dimensions
  if ( (*ims)[0]->GetDimension() == 1)
    {

    typedef itk::Image<T, 3> ITKTimeImage2D;
    typedef itk::ImageFileWriter< ITKTimeImage2D >  ITKTimeImageWriter2D;

    // Initialize ITK image
    typename ITKTimeImage2D::Pointer itkImage;
    itkImage = ITKTimeImage2D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->GetSizeX();
    unsigned int szY = (*ims)[0]->GetSizeY();

    // Set up region
    typename ITKTimeImage2D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    typename ITKTimeImage2D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = ims->size();

    typename ITKTimeImage2D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage2D::SpacingType space;
    space[0] = (*ims)[0]->GetSpacingX();
    space[1] = (*ims)[0]->GetSpacingY();
    space[2] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t)
      {
      for (unsigned int y = 0; y < szY; ++y)
        {
        for (unsigned int x = 0; x < szX; ++x)
          {

          typename ITKTimeImage2D::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = t;

          itkImage->SetPixel(px, (*ims)[t]->GetValue(x,y,0));
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter2D::Pointer imageWriter = ITKTimeImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage2DUtils::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    }
  else
    {

    typedef itk::Image<T, 4> ITKTimeImage2D;
    typedef itk::ImageFileWriter< ITKTimeImage2D >  ITKTimeImageWriter2D;

    // Initialize ITK image
    typename ITKTimeImage2D::Pointer itkImage;
    itkImage = ITKTimeImage2D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->GetSizeX();
    unsigned int szY = (*ims)[0]->GetSizeY();
    unsigned int dim = (*ims)[0]->GetDimension();

    // Set up region
    typename ITKTimeImage2D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;

    typename ITKTimeImage2D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = dim;
    size[3] = ims->size();

    typename ITKTimeImage2D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage2D::SpacingType space;
    space[0] = (*ims)[0]->GetSpacingX();
    space[1] = (*ims)[0]->GetSpacingY();
    space[2] = 1.0;
    space[3] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t)
      {
      for (unsigned int y = 0; y < szY; ++y)
        {
      for (unsigned int x = 0; x < szX; ++x)
        {
        for (unsigned int d = 0; d < dim; ++d)
          {

          typename ITKTimeImage2D::IndexType px;
          px[0] = x;
          px[1] = y;
          px[2] = d;
          px[3] = t;

          itkImage->SetPixel(px, (*ims)[t]->GetValue(x,y,d));
          }
        }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter2D::Pointer imageWriter = ITKTimeImageWriter2D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImage2DUtils::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    }

  return true;
}


//
// writeTimeDependentImagesITK, 3D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK3D( const std::vector< VectorImageType*> * ims, const std::string& filename)
{

  // see if we're dealing with one or two dimensions
  if ((*ims)[0]->GetDimension() == 1)
    {

    typedef itk::Image<T, 4> ITKTimeImage3D;
    typedef itk::ImageFileWriter< ITKTimeImage3D >  ITKTimeImageWriter3D;

    // Initialize ITK image
    typename ITKTimeImage3D::Pointer itkImage;
    itkImage = ITKTimeImage3D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->GetSizeX();
    unsigned int szY = (*ims)[0]->GetSizeY();
    unsigned int szZ = (*ims)[0]->GetSizeZ();

    // Set up region
    typename ITKTimeImage3D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;

    typename ITKTimeImage3D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = szZ;
    size[3] = ims->size();

    typename ITKTimeImage3D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage3D::SpacingType space;
    space[0] = (*ims)[0]->GetSpacingX();
    space[1] = (*ims)[0]->GetSpacingY();
    space[2] = (*ims)[0]->GetSpacingZ();
    space[3] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t)
      {
      for (unsigned int z = 0; z < szZ; ++z)
        {
        for (unsigned int y = 0; y < szY; ++y)
          {
          for (unsigned int x = 0; x < szX; ++x)
            {

            typename ITKTimeImage3D::IndexType px;
            px[0] = x;
            px[1] = y;
            px[2] = z;
            px[3] = t;

            itkImage->SetPixel(px, (*ims)[t]->GetValue(x,y,z,0));
            }
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter3D::Pointer imageWriter = ITKTimeImageWriter3D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    }
  else
    {

    typedef itk::Image<T, 5> ITKTimeImage3D;
    typedef itk::ImageFileWriter< ITKTimeImage3D >  ITKTimeImageWriter3D;

    // Initialize ITK image
    typename ITKTimeImage3D::Pointer itkImage;
    itkImage = ITKTimeImage3D::New();

    // Convert to the ITK image
    unsigned int szX = (*ims)[0]->GetSizeX();
    unsigned int szY = (*ims)[0]->GetSizeY();
    unsigned int szZ = (*ims)[0]->GetSizeZ();
    unsigned int dim = (*ims)[0]->GetDimension();

    // Set up region
    typename ITKTimeImage3D::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;
    start[4] = 0;

    typename ITKTimeImage3D::SizeType size;
    size[0] = szX;
    size[1] = szY;
    size[2] = szZ;
    size[3] = dim;
    size[4] = ims->size();

    typename ITKTimeImage3D::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // Set up the spacing
    typename ITKTimeImage3D::SpacingType space;
    space[0] = (*ims)[0]->GetSpacingX();
    space[1] = (*ims)[0]->GetSpacingY();
    space[2] = (*ims)[0]->GetSpacingZ();
    space[3] = 1.0;
    space[4] = 1.0;
    itkImage->SetSpacing(space);

    // Allocate region to image
    itkImage->SetRegions(region);
    itkImage->Allocate();

    // Copy in the data
    for (unsigned int t = 0; t < ims->size(); ++t)
      {
      for (unsigned int z = 0; z < szZ; ++z)
        {
        for (unsigned int y = 0; y < szY; ++y)
          {
          for (unsigned int x = 0; x < szX; ++x)
            {
            for (unsigned int d = 0; d < dim; ++d)
              {

              typename ITKTimeImage3D::IndexType px;
              px[0] = x;
              px[1] = y;
              px[2] = z;
              px[3] = d;
              px[4] = t;

              itkImage->SetPixel(px, (*ims)[t]->GetValue(x,y,z,d));
              }
            }
          }
        }
      }

    // Initialize ITK writer
    typename ITKTimeImageWriter3D::Pointer imageWriter = ITKTimeImageWriter3D::New();
    imageWriter->SetFileName(filename.c_str());
    imageWriter->SetInput(itkImage);
    imageWriter->UseCompressionOn();

    // Try to write the image out
    try
      {
      imageWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK -> Writing Failed" << std::endl;
      std::cerr << err << std::endl;
      return false;
      }

    }

  return true;

}

//
// writeTimeDependentImagesITK, 2D/3D
//
template <class T, unsigned int VImageDimension >
bool VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK( const std::vector< VectorImageType*>* ims,const std::string& filename)
{
  switch ( VImageDimension )
    {
    case 1:
      return writeTimeDependantImagesITK1D( ims, filename );
      break;
    case 2:
      return writeTimeDependantImagesITK2D( ims, filename );
      break;
    case 3:
      return writeTimeDependantImagesITK3D( ims, filename );
      break;
    default:
      throw std::runtime_error("Cannot write time dependant images of desired dimension");
    }
  return EXIT_FAILURE;
}

//
// readFileITK
//
template <class T, unsigned int VImageDimension >
typename VectorImageUtils< T, VImageDimension >::VectorImageType*
VectorImageUtils< T, VImageDimension >::readFileITK(const std::string& filename)
{

  // Initialize ITK reader
  typedef typename ITKVectorImageReader<T, VImageDimension>::Type ITKVectorImageReaderType;
  typename ITKVectorImageReaderType::Pointer reader = ITKVectorImageReaderType::New();
  reader->SetFileName(filename.c_str());

  // Initialize ITK image
  typename ITKVectorImage<T,VImageDimension>::Type::Pointer itkImage;
  itkImage = reader->GetOutput();

  // Try to read the image
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    throw std::runtime_error( "VectorImageUtils< T, VImageDimension >::readFileITK -> Reading Failed" );
    return NULL;
    }

  // Convert to VectorImageType and return
  return VectorImageUtils< T, VImageDimension >::convertFromITK(itkImage.GetPointer());
}

//
// read Affine ITK
//
template <class T, unsigned int VImageDimension >
typename ITKAffineTransform<T,VImageDimension>::Type::Pointer VectorImageUtils< T, VImageDimension >::readAffineTransformITK(const std::string& filename) {

  // initialize the reader
  itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();

  // link the reader to the file and open the file
  reader->SetFileName(filename);
  reader->Update();

  // extract the transform
  typedef itk::TransformFileReader::TransformListType* TransformListType;
  TransformListType transformList = reader->GetTransformList();
  itk::TransformFileReader::TransformListType::const_iterator it;
  it = transformList->begin();
  typename ITKAffineTransform<T,VImageDimension>::Type::Pointer transform;
  if (!strcmp((*it)->GetNameOfClass(), "AffineTransform"))
    {
    transform = static_cast<typename ITKAffineTransform<T,VImageDimension>::Type*>((*it).GetPointer());
    }

  // return the transform
  return transform;

}


#endif


