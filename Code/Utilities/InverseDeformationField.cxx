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

/**
 * To apply a map to an image to get a transformed image
 */

#include <iostream>
#include "LDDMMUtils.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"

// ITK INLCUDES
#include <itkInverseDisplacementFieldImageFilter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkDisplacementFieldTransform.h>


#include "InverseDeformationFieldCLP.h"

typedef double TFLOAT;
const unsigned int VImageDimension = 2;

template < unsigned int VImageDimension >
int DoIt( int argc, char **argv )
{
    PARSE_ARGS;

    typedef itk::Vector<float,VImageDimension>                            PixelType;
    typedef itk::Image< PixelType, VImageDimension >                      ImageType;
    typedef itk::InverseDisplacementFieldImageFilter<ImageType,ImageType> InverseDisplacementFieldImageFilterType;
    typedef itk::ImageFileReader< ImageType >                             ReaderType;
    typedef itk::ImageFileWriter< ImageType >                             WriterType;

    typename ReaderType::Pointer                              reader = ReaderType::New();
    typename WriterType::Pointer                              writer = WriterType::New();
    typename InverseDisplacementFieldImageFilterType::Pointer filter = InverseDisplacementFieldImageFilterType::New();

    reader->SetFileName( inputFilename  );
    writer->SetFileName( outputFilename );
    writer->UseCompressionOn();

    reader->Update();

    std::cout << "Size = "    << reader->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
    std::cout << "Index = "   << reader->GetOutput()->GetLargestPossibleRegion().GetIndex() << std::endl;
    std::cout << "Spacing = " << reader->GetOutput()->GetSpacing() << std::endl;
    std::cout << "Origin = "  << reader->GetOutput()->GetOrigin() << std::endl;

    filter->SetInput(reader->GetOutput());
    filter->SetOutputSpacing(reader->GetOutput()->GetSpacing());
    filter->SetOutputOrigin(reader->GetOutput()->GetOrigin());
    filter->SetSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());
    filter->SetSubsamplingFactor(2);
    writer->SetInput( filter->GetOutput() );
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


template<>
int DoIt<4>( int argc, char **argv )
{
    PARSE_ARGS;

    typedef float PixelType;
    //  Software Guide : BeginLatex
    //
    //  Then, as usual, a decision must be made about the type of pixel used to
    //  represent the image processed by the pipeline. Note that when reading
    //  and writing images, the pixel type of the image \textbf{is not
    //  necessarily} the same as the pixel type stored in the file.  Your
    //  choice of the pixel type (and hence template parameter) should be
    //  driven mainly by two considerations:
    //
    //  \begin{itemize}
    //  \item It should be possible to cast the file pixel type in the file to
    //  the pixel type you select. This casting will be performed using the
    //  standard C-language rules, so you will have to make sure that the
    //  conversion does not result in information being lost.
    //  \item The pixel type in memory should be appropriate to the type of
    //  processing you intended to apply on the images.
    //  \end{itemize}
    //
    //  A typical selection for medical images is illustrated in
    //  the following lines.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef itk::Image< PixelType, 4 >                    ImageType;
    typedef itk::Image< itk::Vector<PixelType,3>, 3 >     ImageType3D;
    typedef itk::ImageRegionIteratorWithIndex<ImageType>  IteratorType;
    typedef itk::InverseDisplacementFieldImageFilter<ImageType3D,ImageType3D> InverseDisplacementFieldImageFilterType;
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  Note that the dimension of the image in memory should match the one of
    //  the image in file. There are a couple of special cases in which this
    //  condition may be relaxed, but in general it is better to ensure that both
    //  dimensions match.
    //
    //  We can now instantiate the types of the reader and writer. These two
    //  classes are parameterized over the image type.
    //
    //  \index{itk::ImageFileReader!Instantiation}
    //  \index{itk::ImageFileWriter!Instantiation}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typedef itk::ImageFileReader< ImageType >     ReaderType;
    typedef itk::ImageFileWriter< ImageType3D >   WriterType;

    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  Then, we create one object of each type using the New() method and
    //  assigning the result to a \doxygen{SmartPointer}.
    //
    //  \index{itk::ImageFileReader!New()}
    //  \index{itk::ImageFileWriter!New()}
    //  \index{itk::ImageFileReader!SmartPointer}
    //  \index{itk::ImageFileWriter!SmartPointer}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    typename ReaderType::Pointer reader = ReaderType::New();
    typename WriterType::Pointer writer = WriterType::New();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    //  The name of the file to be read or written is passed with the
    //  SetFileName() method.
    //
    //  \index{itk::ImageFileReader!SetFileName()}
    //  \index{itk::ImageFileWriter!SetFileName()}
    //  \index{SetFileName()!itk::ImageFileReader}
    //  \index{SetFileName()!itk::ImageFileWriter}
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    reader->SetFileName( inputFilename  );
    writer->SetFileName( outputFilename );
    writer->UseCompressionOn();
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  At first view, this may seem as a quite useless program, but it is
    //  actually implementing a powerful file format conversion tool! The
    //  execution of the pipeline is triggered by the invocation of the
    //  \code{Update()} methods in one of the final objects. In this case, the final
    //  data pipeline object is the writer. It is a wise practice of defensive
    //  programming to insert any \code{Update()} call inside a \code{try/catch} block
    //  in case exceptions are thrown during the execution of the pipeline.
    //
    //  Software Guide : EndLatex

    // Update meta info and allocate space for new image
    reader->Update();

    typename ImageType::Pointer input = reader->GetOutput();
    typename ImageType3D::Pointer output = ImageType3D::New();

    std::cout << "Size = "    << input->GetLargestPossibleRegion().GetSize() << std::endl;
    std::cout << "Index = "   << input->GetLargestPossibleRegion().GetIndex() << std::endl;
    std::cout << "Spacing = " << input->GetSpacing() << std::endl;
    std::cout << "Origin = "  << input->GetOrigin() << std::endl;

    typename ImageType::SizeType  inputSize = input->GetLargestPossibleRegion().GetSize();
    typename ImageType::IndexType inputIndex = input->GetLargestPossibleRegion().GetIndex();
    typename ImageType::SpacingType inputSpacing = input->GetSpacing();
    typename ImageType::PointType inputOrigin = input->GetOrigin();

    typename ImageType3D::SizeType    outputSize;
    typename ImageType3D::IndexType   outputIndex;
    typename ImageType3D::SpacingType outputSpacing;
    typename ImageType3D::PointType   outputOrigin;
    typename ImageType3D::RegionType  outputRegion;

    outputSize[0] = inputSize[0];
    outputSize[1] = inputSize[1];
    outputSize[2] = inputSize[2];

    outputIndex[0] = inputIndex[0];
    outputIndex[1] = inputIndex[1];
    outputIndex[2] = inputIndex[2];

    outputSpacing[0] = inputSpacing[0];
    outputSpacing[1] = inputSpacing[1];
    outputSpacing[2] = inputSpacing[2];

    outputOrigin[0] = inputOrigin[0];
    outputOrigin[1] = inputOrigin[1];
    outputOrigin[2] = inputOrigin[2];

    outputRegion.SetSize(outputSize);
    outputRegion.SetIndex(outputIndex);

    output->SetRegions(outputRegion);
    output->SetSpacing(outputSpacing);
    output->SetOrigin(outputOrigin);

    output->Allocate();

    IteratorType inputIt(input, input->GetRequestedRegion());
    for(inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt)
    {
        typename ImageType::IndexType   inputIdx = inputIt.GetIndex();
        typename ImageType3D::IndexType outputIdx;
        outputIdx[0] = inputIdx[0];
        outputIdx[1] = inputIdx[1];
        outputIdx[2] = inputIdx[2];

        typename ImageType3D::PixelType &pixel = output->GetPixel(outputIdx);

        pixel[inputIdx[3]] = inputIt.Get();
    }

    //  Software Guide : BeginLatex
    //
    //  We can now connect these readers and writers to filters to create a
    //  pipeline. For example, we can create a short pipeline by passing
    //  the output of the reader directly to the input of the writer.
    //
    //  Software Guide : EndLatex

    typename InverseDisplacementFieldImageFilterType::Pointer filter = InverseDisplacementFieldImageFilterType::New();
    filter->SetInput(output);
    filter->SetOutputSpacing(output->GetSpacing());
    filter->SetOutputOrigin(output->GetOrigin());
    filter->SetSize(output->GetLargestPossibleRegion().GetSize());


    // Software Guide : BeginCodeSnippet
    writer->SetInput( filter->GetOutput() );
    // Software Guide : EndCodeSnippet


    // Software Guide : BeginCodeSnippet
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    // Software Guide : EndCodeSnippet


    //  Software Guide : BeginLatex
    //
    //  Note that exceptions should only be caught by pieces of code that know
    //  what to do with them. In a typical application this \code{catch} block
    //  should probably reside on the GUI code. The action on the \code{catch}
    //  block could inform the user about the failure of the IO operation.
    //
    //  The IO architecture of the toolkit makes it possible to avoid explicit
    //  specification of the file format used to read or write
    //  images.\footnote{In this example no file format is specified; this
    //  program can be used as a general file conversion utility.}  The object
    //  factory mechanism enables the ImageFileReader and ImageFileWriter to
    //  determine (at run-time) with which file format it is working
    //  with. Typically, file formats are chosen based on the filename
    //  extension, but the architecture supports arbitrarily complex processes
    //  to determine whether a file can be read or written. Alternatively, the
    //  user can specify the data file format by explicit instantiation and
    //  assignment the appropriate \doxygen{ImageIO} subclass.
    //
    //  For historical reasons and as a convenience to the user, the
    //  \doxygen{ImageFileWriter} also has a Write() method that is aliased to
    //  the \code{Update()} method. You can in principle use either of them but
    //  \code{Update()} is recommended since Write() may be deprecated in the future.
    //
    //  Software Guide : EndLatex
    return EXIT_SUCCESS;
}


int main( int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputFilename );

  std::cout << "Image size = " << uiSourceImageDimension << std::endl;

  switch ( uiSourceImageDimension )
    {
    case 3:
      return DoIt<3>( argc, argv );
      break;
    case 4:
      return DoIt<4>( argc, argv );
      break;
    default:
      std::cerr << "Unsupported image dimension = " << uiSourceImageDimension << std::endl;
    }

  return EXIT_FAILURE;
}

