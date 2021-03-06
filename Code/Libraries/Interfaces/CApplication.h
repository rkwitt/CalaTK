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
#ifndef C_APPLICATION_H
#define C_APPLICATION_H

#include "CAlgorithmFactory.h"

namespace CALATK
{

/** \class CApplication
 *
 * \brief High level interface to a CALATK analysis.
 *
 * This class parses JSON configuration files to perform a CALATK analysis.
 *
 * The analysis can be an image-to-image registration, registration across a
 * time series, or atlas building depending on the input parameter and data specified.
 *
 * The algorithm and parameters of the given algorithm are determined by the
 * one JSON configuration and the input images, etc. are determined by another
 * JSON configuration.
 *
 */
class CApplication: public CBase
{
public:
  /** Standard class typedefs. */
  typedef CApplication                    Self;
  typedef CBase                           Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;
  bool JSONTransform;

  /** Default constructor. */
  CApplication();

  /** Parse C main() arguments to find the configuration file arguments.  The
   * arguments will be parsed as:
   *
   *   appname <given_algorithm_parameters_config.json>
   *           (<given_data_parameters_config.json> | <source_image_file> <target_image_file> <warped_source_output_file>)
   *           [<used_algorithm_parameters_config.json>]
   *           [<used_data_parameters_config.json>]
   */
  CApplication( const int argc, char **argv );

  /** Set the JSON file that contains algorithm parameters. */
  void SetGivenAlgorithmConfigurationFile( const std::string & file );
  std::string GetGivenAlgorithmConfigurationFile() const;

  /** For a simple a simple image-to-image registration, the input images file
   * names can be specified here instead of specification inside a data parameters
   * configuration file. */
  void SetGivenSourceAndTargetImageFiles( const std::string & source, const std::string & target );
  /** For a simple a simple image-to-image registration, the warped output image file
   * name can be specified here instead of specification inside a data parameters
   * configuration file. */
  void SetGivenWarpedImageFile( const std::string & wraped );

  /** Set the JSON file that contains input data and desired output data. */
  void SetGivenDataConfigurationFile( const std::string & file );
  std::string GetGivenDataConfigurationFile() const;

  /** Set file to output the algorithm configuration that was used for the
   * analysis.  This is the given algorithm configuration with the defaults used
   * for non-specified values. */
  void SetUsedAlgorithmConfigurationFile( const std::string & file );
  std::string GetUsedAlgorithmConfigurationFile() const;

  /** Set file to output the data configuration that was used for the
   * analysis.  This is the given data configuration with the defaults used
   * for non-specified values. */
  void SetUsedDataConfigurationFile( const std::string & file );
  std::string GetUsedDataConfigurationFile() const;

  /** Run the analysis based on the given Algorithm and Data configurations. */
  void Solve();

private:
  std::string m_GivenAlgorithmConfigurationFile;
  std::string m_GivenDataConfigurationFile;
  std::string m_UsedAlgorithmConfigurationFile;
  std::string m_UsedDataConfigurationFile;

  CJSONConfiguration::Pointer m_CombinedAlgorithmJSONConfig;
  CJSONConfiguration::Pointer m_CombinedDataJSONConfig;
  CJSONConfiguration::Pointer m_CleanedAlgorithmJSONConfig;
  CJSONConfiguration::Pointer m_CleanedDataJSONConfig;

  void InstantiateConfigs();

  /** Templated version of Solve. */
  template< class TFloat, unsigned int VImageDimension >
  void InternalSolve();
};

}; // end namespace CALATK

#endif
