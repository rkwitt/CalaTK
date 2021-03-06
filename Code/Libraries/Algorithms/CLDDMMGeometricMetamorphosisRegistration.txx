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

#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX

template < class TState >
CLDDMMGeometricMetamorphosisRegistration< TState >::CLDDMMGeometricMetamorphosisRegistration()
  : DefaultMaskKernel( "HelmholtzKernel" ), m_ExternallySetMaskKernel( false )
{
  m_MaskKernel = DefaultMaskKernel;

  m_ptrMaskKernel = NULL;
}

template < class TState >
CLDDMMGeometricMetamorphosisRegistration< TState >::~CLDDMMGeometricMetamorphosisRegistration()
{
}

template< class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "GeneralRegistrationSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "GeneralRegistrationSettings", Json::nullValue, CONF_NORMAL );

  SetJSONHelpForRootKey( GeneralRegistrationSettings, "general setting for the registration", CONF_NORMAL );

  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, MaskKernel, CONF_NORMAL );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, MaskKernel,
                     "kernel for the mask: GaussianKernel / HelmholtzKernel /  MultiGaussianKernel", CONF_NORMAL );
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetMaskKernelPointer( KernelType * ptrKernel )
{
  this->m_ptrMaskKernel = ptrKernel;
}

template < class TState >
typename CLDDMMGeometricMetamorphosisRegistration< TState >::KernelType *
CLDDMMGeometricMetamorphosisRegistration< TState >::GetMaskKernelPointer()
{
  if ( m_ptrMaskKernel.GetPointer() == NULL )
    {
    this->SetDefaultMaskKernelPointer();
    }
  return this->m_ptrMaskKernel.GetPointer();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultMaskKernelPointer()
{
  this->m_ptrMaskKernel = CKernelFactory< T, TState::ImageDimension >::CreateNewKernel( m_MaskKernel );
}

template < class TState >
const typename CLDDMMGeometricMetamorphosisRegistration< TState >::VectorImageType*
CLDDMMGeometricMetamorphosisRegistration< TState >::GetImageT( T dTime )
{
  dynamic_cast< CLDDMMGeometricMetamorphosisObjectiveFunction< TState>* >(this->m_ptrObjectiveFunction.GetPointer() )->GetImageT( this->m_ptrIm, dTime );
  return this->m_ptrIm.GetPointer();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultsIfNeeded()
{
  std::cout << "Called the default mask kernel function" << std::endl;
  if ( m_ptrMaskKernel.GetPointer() == NULL )
    {
    SetDefaultMaskKernelPointer();
    }

  this->m_ptrMaskKernel->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrMaskKernel->SetAllowHelpComments( this->GetAllowHelpComments() );

  Json::Value * node;
  node = &(this->m_CombinedJSONConfig->GetFromKey( "MaskKernel", Json::nullValue ));
  CJSONConfiguration::Pointer maskCombined = new CJSONConfiguration( node, this->m_CombinedJSONConfig->GetPrintSettings() );
  node = &(this->m_CleanedJSONConfig->GetFromKey( "MaskKernel", Json::nullValue ));
  CJSONConfiguration::Pointer maskCleaned = new CJSONConfiguration( node, this->m_CleanedJSONConfig->GetPrintSettings() );
  this->m_ptrMaskKernel->SetAutoConfiguration( maskCombined, maskCleaned );

  Superclass::SetDefaultsIfNeeded();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel.GetPointer() == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMaskKernel.GetPointer() == NULL )
    {
    throw std::runtime_error( "Mask kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric.GetPointer() == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CLDDMMGeometricMetamorphosisObjectiveFunction< TState > CLDDMMType;
  typename CLDDMMType::Pointer plddmm = new CLDDMMType;

  this->m_ptrObjectiveFunction = plddmm;

}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::PreFirstSolve()
{

  Superclass::PreFirstSolve();

  typedef CLDDMMGeometricMetamorphosisObjectiveFunction< TState > CLDDMMType;
  CLDDMMType * plddmm = NULL;

  plddmm = dynamic_cast< CLDDMMType* >( this->m_ptrObjectiveFunction.GetPointer() );

  if ( plddmm == NULL )
  {
    throw std::runtime_error( "Objective function was not intialized." );
  }

  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMaskKernelPointer( this->m_ptrMaskKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );

  plddmm->SetActiveSubjectId( this->GetActiveSubjectId() );

  // set the objective functions for the kernels and the kernel numbers

  KernelUtilsType::SetObjectiveFunctionAndKernelNumberIfNeeded( this->m_ptrKernel, plddmm, 0 );
  KernelUtilsType::SetObjectiveFunctionAndKernelNumberIfNeeded( this->m_ptrMaskKernel, plddmm, 1 );

}

#endif


