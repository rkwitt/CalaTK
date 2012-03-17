/*
*
*  Copyright 2011 by the CALATK development team
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

#ifndef C_METAMORPHOSIS_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
#define C_METAMORPHOSIS_ADJOINT_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H

#include "CLDDMMGeodesicShootingObjectiveFunction.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"


namespace CALATK
{
/**
  * Implements the shooting method for metamorphosis
  *
  */

template < class TState >
class CMetamorphosisAdjointGeodesicShootingObjectiveFunction
    : public CLDDMMGeodesicShootingObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CMetamorphosisAdjointGeodesicShootingObjectiveFunction Self;
  typedef CLDDMMGeodesicShootingObjectiveFunction< TState >      Superclass;
  typedef itk::SmartPointer< Self >                              Pointer;
  typedef itk::SmartPointer< const Self >                        ConstPointer;

  /* Some useful typedefs */
  typedef typename TState::TFloat T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CMetamorphosisAdjointGeodesicShootingObjectiveFunction();
  virtual ~CMetamorphosisAdjointGeodesicShootingObjectiveFunction();

  void InitializeState();
  void InitializeState( TState* ptrState );

  void GetImage( VectorImageType* ptrIm, T dTime );
  void GetMomentum( VectorImageType* ptrMomentum, T dTime );

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetMapFromTo(VectorFieldType *ptrMap, T dTimeFrom, T dTimeTo);

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();
  void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType *ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

  void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

  // methods for the augmented Lagrangian
  void SetSquaredPenaltyScalarWeight( T dWeight );
  T GetSquaredPenaltyScalarWeight();
  VectorImageType* GetPointerToImageLagrangianMultiplier();
  const VectorImageType* GetPointerToCurrentImageResidual();

  GetMacro( Rho, T );
  SetMacro( Rho, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:

  /** Some useful protected typedefs */

  typedef std::vector< typename VectorFieldType::Pointer >* VectorPointerToVectorFieldPointerType;

  typedef std::vector< typename VectorImageType::Pointer >* VectorPointerToVectorImagePointerType;

  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState* ptrState );

  void DeleteData();

  void CreateNewStateStructures();
  void ShallowCopyStateStructures( TState* ptrState );
  void CreateGradientAndAuxiliaryStructures();

  void ComputeImageMomentumForward();
  void ComputeAdjointsBackward();
  void ComputeVelocityAdjoint(const VectorImageType * ptrI, const VectorImageType * ptrP, const VectorImageType * ptrLambdaI, const VectorImageType * ptrLambdaP, VectorFieldType * LambdaVOut);

  void CreateTimeDiscretization();

  typedef CImageManager< T, TState::VImageDimension > ImageManagerType;
  typedef typename ImageManagerType::ImageInformation ImageInformation;
  typedef typename ImageManagerType::SubjectInformationType SubjectInformationType;

  typedef CTimePoint< T, VectorImageType, VectorFieldType > STimePoint;

private:
  typename VectorFieldType::Pointer m_ptrMapIn; // map for the numerical solution
  typename VectorFieldType::Pointer m_ptrMapOut; // map for the numerical solution
  typename VectorFieldType::Pointer m_ptrMapTmp; // map for the numerical solution
  typename VectorFieldType::Pointer m_ptrMapIdentity; // stores the identity map
  typename VectorFieldType::Pointer m_ptrMapIncremental; // stores the incremental map for the source terms of the adjoint PDEs

  typename VectorImageType::Pointer m_ptrCurrentAdjointIDifference;
  typename VectorImageType::Pointer m_ptrDeterminantOfJacobian;

  // temporary storage memory managed by this class
  typename VectorFieldType::Pointer m_ptrTmpField;
  typename VectorFieldType::Pointer m_ptrTmpFieldConv;
  typename VectorImageType::Pointer m_ptrTmpScalarImage;
  typename VectorImageType::Pointer m_ptrTmpImage;

  typename VectorImageType::Pointer m_ptrDI;
  typename VectorImageType::Pointer m_ptrDP;

  VectorPointerToVectorImagePointerType m_ptrI; // image
  VectorPointerToVectorImagePointerType m_ptrP; // momentum

  // just for testing, remove later
  VectorPointerToVectorImagePointerType tstLamI;
  VectorPointerToVectorImagePointerType tstLamP;

  typename VectorImageType::Pointer m_ptrCurrentLambdaI; // adjoint 1
  typename VectorImageType::Pointer m_ptrCurrentLambdaP; // adjoint 2
  typename VectorFieldType::Pointer m_ptrCurrentLambdaV; // adjoint 3

  VectorPointerToVectorFieldPointerType m_ptrVelocityField;

  std::vector< T > m_vecMeasurementTimepoints;

  // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

  std::vector< STimePoint > m_vecTimeDiscretization;
  std::vector< T > m_vecTimeIncrements;


  T m_Rho;  ///< how much the image source term is penalized

  const T DefaultRho;
  bool m_ExternallySetRho;

  // augmented Lagrangian
  T m_AugmentedLagrangianMu;

  typename VectorImageType::Pointer m_ptrImageLagrangianMultiplier;

};

#include "CMetamorphosisAdjointGeodesicShootingObjectiveFunction.txx"

} // end namespace
#endif // C_METAMORPHOSIS_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_H
