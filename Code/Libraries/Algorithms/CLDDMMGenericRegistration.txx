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

#ifndef C_LDDMM_GENERIC_REGISTRATION_TXX
#define C_LDDMM_GENERIC_REGISTRATION_TXX

template < class TState >
CLDDMMGenericRegistration< TState >::CLDDMMGenericRegistration()
  : DefaultObjectiveFunction( "LDDMMGrowthModel" ), m_ExternallySetObjectiveFunction( false )
{
  m_ObjectiveFunction = DefaultObjectiveFunction;
}

template < class TState >
CLDDMMGenericRegistration< TState >::~CLDDMMGenericRegistration()
{
}

template < class TState >
void CLDDMMGenericRegistration< TState >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "GeneralRegistrationSettings", Json::nullValue );

  SetJSONObjectiveFunction( this->m_jsonConfig.GetFromKey( currentConfiguration, "ObjectiveFunction", GetExternalOrDefaultObjectiveFunction() ).asString() );
}


template < class TState >
void CLDDMMGenericRegistration< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  //typedef CObjectiveFunction< TState > CLDDMMType;
  //CLDDMMType* plddmm = NULL;
  typedef CVelocityFieldObjectiveFunctionWithMomentum< TState > LDDMMVelocityFieldObjectiveFunctionWithMomentumType;
  LDDMMVelocityFieldObjectiveFunctionWithMomentumType* plddmm = NULL;

  plddmm = dynamic_cast< LDDMMVelocityFieldObjectiveFunctionWithMomentumType * >( CObjectiveFunctionFactory< typename TState::TFloat, TState::VImageDimension >::CreateNewObjectiveFunction( m_ObjectiveFunction ) );

  if ( plddmm == NULL )
  {
    throw std::runtime_error("Could not initialize the objective function. Make sure the instantiated state type is consistent with the objective function chosen.");
    return;
  }

  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );

  plddmm->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );

  this->m_ptrObjectiveFunction = plddmm;

}

#endif