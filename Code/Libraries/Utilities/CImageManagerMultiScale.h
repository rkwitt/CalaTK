#ifndef C_IMAGE_MANAGER_MULTI_SCALE_H
#define C_IMAGE_MANAGER_MULTI_SCALE_H

#include "CImageManagerFullScale.h"
#include "CResampler.h"
#include "CResamplerLinear.h"
#include <vector>
#include <algorithm>
#include "CALATKCommon.h"


/**
 * CIageManagerMultiScale.h -- Specializes the full scale image manager, so we can use multiple scales
 * Images are automatically reloaded when the scale is changed
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class CImageManagerMultiScale : public CImageManagerFullScale< T, VImageDimension, TSpace >
{
public:

  /** Some useful typedefs */

  typedef CImageManagerFullScale< T, VImageDimension, TSpace > Superclass;
  typedef CResampler< T, VImageDimension > ResamplerType;
  
  typedef typename Superclass::SImageInformation SImageInformation;

  typedef typename Superclass::VectorImageType VectorImageType;

  CImageManagerMultiScale();
  ~CImageManagerMultiScale();

  void AddScale( T dScale, unsigned int uiScaleIndx );
  void RemoveScale( unsigned int uiScaleIndx );

  unsigned int GetNumberOfScales();

  void SelectScale( unsigned int uiScaleIndx );

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   * Could just return an image, or a downsampled version, ... depending on implementation 
   */
  virtual void GetImage( SImageInformation* pCurrentImInfo );

  void SetResamplerPointer( ResamplerType* ptrResampler );
  const ResamplerType* GetResamplerPointer() const;

  virtual bool SupportsMultiScaling()
  {
    return true;
  }

  SetMacro( Sigma, T );
  GetMacro( Sigma, T );

protected:

  void SetDefaultResamplerPointer();
  void DeleteDefaultResampler();
  void SetScale( SImageInformation* pCurrentImInfo );

private:

  bool m_bSetDefaultResampler;

  unsigned int m_uiCurrentlySelectedScale;
  std::vector< T > m_ScaleVector;
  std::vector< bool > m_ScaleWasSet;

  // disallow any changing of the scales once images were read
  bool m_bImagesWereRead;

  ResamplerType* m_ptrResampler;

  T m_Sigma;

};

#include "CImageManagerMultiScale.txx"

} // end namespace


#endif
