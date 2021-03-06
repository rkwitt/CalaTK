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

#ifndef C_SOLVER_LINE_SEARCH_H
#define C_SOLVER_LINE_SEARCH_H

#include "CALATKCommon.h"
#include "CObjectiveFunction.h"
#include "CSolver.h"


// TODO: Make this a generic line search. This one is a line search for a gradient descent

namespace CALATK
{
/**
 * Base class for a generic line search algorithm
 */
template < class TState >
class CSolverLineSearch : public CSolver< TState >
{
public:
  /** Standard class typedefs. */
  typedef CSolverLineSearch               Self;
  typedef CSolver< TState >               Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename TState::FloatType                 T;
  typedef typename Superclass::ObjectiveFunctionType ObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues         CEnergyValues;

/**
 *Constructor setting the default values
 */
  CSolverLineSearch();

/**
 * Destructor
 */
  virtual ~CSolverLineSearch();

  SetMacro( InitialStepSize, T );
  GetMacro( InitialStepSize, T );

  SetMacro( AdjustStepSizeUpFactor, T );
  GetMacro( AdjustStepSizeUpFactor, T );

  SetMacro( AdjustStepSizeDownFactor, T );
  GetMacro( AdjustStepSizeDownFactor, T );

  SetMacro( ReductionFactor, T );
  GetMacro( ReductionFactor, T );

  SetMacro( MinAllowedStepSize, T );
  GetMacro( MinAllowedStepSize, T );

  SetMacro( MaxNumberOfIterations, unsigned int );
  GetMacro( MaxNumberOfIterations, unsigned int );

  SetMacro( MaxNumberOfTries, unsigned int );
  GetMacro( MaxNumberOfTries, unsigned int );

  SetMacro( AdjustStepSizeUpNumber, unsigned int );
  GetMacro( AdjustStepSizeUpNumber, unsigned int );

  SetMacro( AdjustStepSizeDownNumber, unsigned int );
  GetMacro( AdjustStepSizeDownNumber, unsigned int );

  SetMacro( DecreaseConstant, T );
  GetMacro( DecreaseConstant, T );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

  bool LineSearchWithBacktracking( CEnergyValues CurrentEnergy, T dDesiredStepSize, T& dAlpha, CEnergyValues& ResultingEnergy, unsigned int &uiIter, TState* pTempState );

  T m_InitialStepSize;
  
  T m_AdjustStepSizeUpFactor;
  T m_AdjustStepSizeDownFactor;
  T m_ReductionFactor;

  T m_MinAllowedStepSize;
  
  T m_DecreaseConstant;

  unsigned int m_MaxNumberOfIterations;
  unsigned int m_MaxNumberOfTries;

  unsigned int m_AdjustStepSizeUpNumber;
  unsigned int m_AdjustStepSizeDownNumber;
  

private:

  const T DefaultInitialStepSize;
  const T DefaultAdjustStepSizeUpFactor;
  const T DefaultAdjustStepSizeDownFactor;
  const T DefaultReductionFactor;
  const T DefaultMinAllowedStepSize;
  const T DefaultDecreaseConstant;
  const unsigned int DefaultMaxNumberOfIterations;
  const unsigned int DefaultMaxNumberOfTries;
  const unsigned int DefaultAdjustStepSizeUpNumber;
  const unsigned int DefaultAdjustStepSizeDownNumber;

  bool m_ExternallySetInitialStepSize;
  bool m_ExternallySetAdjustStepSizeUpFactor;
  bool m_ExternallySetAdjustStepSizeDownFactor;
  bool m_ExternallySetReductionFactor;
  bool m_ExternallySetMinAllowedStepSize;
  bool m_ExternallySetDecreaseConstant;
  bool m_ExternallySetMaxNumberOfIterations;
  bool m_ExternallySetMaxNumberOfTries;
  bool m_ExternallySetAdjustStepSizeUpNumber;
  bool m_ExternallySetAdjustStepSizeDownNumber;

};

#include "CSolverLineSearch.txx"

} // end namespace

#endif
