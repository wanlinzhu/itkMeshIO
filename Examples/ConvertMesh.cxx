#include "itkMeshFileReader.h"
#include "itkMeshFileWriter.h"

#include "itkMesh.h"

int main(int argc, char **argv)
{
  if(argc < 3)
    {
    std::cerr<<"Usage : "<<argv[0]<<" inputMeshFile outputMeshFile"<<std::endl;
    return EXIT_FAILURE;
    }

  typedef float      MeshPixelType;
  const unsigned int Dimension = 3;

  typedef itk::Mesh< MeshPixelType, Dimension >   MeshType;
  typedef itk::MeshFileReader<MeshType>           ReaderType;
  typedef itk::MeshFileWriter<MeshType>           WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  try
    {
    reader->Update();
    }
  catch(itk::ExceptionObject & err)
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  //Do something with itk::Mesh

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( reader->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}


