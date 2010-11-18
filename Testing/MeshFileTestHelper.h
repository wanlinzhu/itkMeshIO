#include "itkMeshFileReader.h"
#include "itkMeshFileWriter.h"
#include <itksys/SystemTools.hxx>

template< class TMesh >
int
test(char *INfilename, char *OUTfilename, bool IsBinary)
{
  const double tol = 1e-6;

  typedef itk::MeshFileReader< TMesh > MeshFileReaderType;
  typedef itk::MeshFileWriter< TMesh > MeshFileWriterType;

  typename MeshFileReaderType::Pointer reader = MeshFileReaderType::New();
  reader->SetFileName(INfilename);
  try
    {
    reader->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cerr << "Read file " << INfilename << " failed " << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  typename MeshFileWriterType::Pointer writer = MeshFileWriterType::New();
	if(itksys::SystemTools::GetFilenameLastExtension(INfilename) == itksys::SystemTools::GetFilenameLastExtension(OUTfilename) )
    {
    writer->SetMeshIO(reader->GetMeshIO());
    }
  writer->SetFileName(OUTfilename);
  writer->SetInput( reader->GetOutput() );

  // NOTE ALEX: we should document the usage
  if ( IsBinary )
    {
    writer->SetFileTypeAsBINARY();
    }

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cerr << "Write file " << OUTfilename << " failed " << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  if ( !itksys::SystemTools::FilesDiffer(INfilename, OUTfilename) )
    {
    return EXIT_SUCCESS;
    }

  typename MeshFileReaderType::Pointer reader1 = MeshFileReaderType::New();
  reader1->SetFileName(OUTfilename);
  try
    {
    reader1->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cerr << "Read file " << OUTfilename << " failed " << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  typename TMesh::PointsContainerPointer points0 = reader->GetOutput()->GetPoints();
  typename TMesh::PointsContainerPointer points1 = reader1->GetOutput()->GetPoints();

  // Test points
  if ( points0 && points1 )
    {
    typename TMesh::PointsContainerConstIterator pt0 = points0->Begin();
    typename TMesh::PointsContainerConstIterator pt1 = points1->Begin();
    while ( pt0 != points0->End() )
      {
      if ( pt0.Value().SquaredEuclideanDistanceTo( pt1.Value() ) > tol )
        {
        std::cerr << "Input mesh and output mesh are different in points!" << std::endl;
        std::cerr << "Input point = "  << pt0.Value() << std::endl;
        std::cerr << "Output point = " << pt1.Value() << std::endl;
        return EXIT_FAILURE;
        }
       pt0++;
       pt1++;
      }
    }

  // Test cells
  typename TMesh::CellsContainerPointer cells0 = reader->GetOutput()->GetCells();
  typename TMesh::CellsContainerPointer cells1 = reader1->GetOutput()->GetCells();
  if ( cells0 && cells1 )
    {
    typename TMesh::CellsContainerConstIterator ceIt0 = cells0->Begin();
    typename TMesh::CellsContainerConstIterator ceIt1 = cells1->Begin();

    while ( ( ceIt0 != cells0->End() )  && ( ceIt1 != cells1->End() ) )
      {
      if ( ceIt0.Value()->GetType() != ceIt1.Value()->GetType() )
        {
        std::cerr << "Input mesh and output mesh are different in cell type!" << std::endl;
        return EXIT_FAILURE;
        }

      typename TMesh::CellType::PointIdIterator pit0 = ceIt0.Value()->PointIdsBegin();
      typename TMesh::CellType::PointIdIterator pit1 = ceIt1.Value()->PointIdsBegin();
      while ( pit0 != ceIt0.Value()->PointIdsEnd() )
        {
        if ( *pit0 != *pit1 )
          {
          std::cerr << "Input mesh and output mesh are different in cells!" << std::endl;
          return EXIT_FAILURE;
          }
        pit0++;
        pit1++;
        }
      ceIt0++;
      ceIt1++;
      }
    }

  // Test point data
  typename TMesh::PointDataContainerPointer pointData0 = reader->GetOutput()->GetPointData();
  typename TMesh::PointDataContainerPointer pointData1 = reader1->GetOutput()->GetPointData();
  if ( pointData0 && pointData1 )
    {
    typename TMesh::PointDataContainerIterator pdIt0 = pointData0->Begin();
    typename TMesh::PointDataContainerIterator pdIt1 = pointData1->Begin();
    while ( pdIt0 != pointData0->End() )
      {
      if ( pdIt0.Value() != pdIt1.Value() )
        {
        std::cerr << "Input mesh and output mesh are different in point data!" << std::endl;
        std::cerr << "Input = "  << pdIt0.Value() << std::endl;
        std::cerr << "Output = " << pdIt1.Value() << std::endl;
        return EXIT_FAILURE;
        }
      pdIt0++;
      pdIt1++;
      }
    }
  else
    {
    if ( pointData0.GetPointer() != pointData1.GetPointer() )
      {
      std::cerr << "Input mesh and output mesh are different in point data!" << std::endl;
      std::cerr << "pointData0 = "  << pointData0.GetPointer() << std::endl;
      std::cerr << "pointData1 = " << pointData1.GetPointer() << std::endl;
      return EXIT_FAILURE;
      }
    }

  // Test cell data
  typename TMesh::PointDataContainerPointer cellData0 = reader->GetOutput()->GetCellData();
  typename TMesh::PointDataContainerPointer cellData1 = reader1->GetOutput()->GetCellData();
  if ( cellData0 && cellData1 )
    {
    typename TMesh::CellDataContainerIterator cdIt0 = cellData0->Begin();
    typename TMesh::CellDataContainerIterator cdIt1 = cellData1->Begin();
    while ( cdIt0 != cellData0->End() )
      {
      if ( cdIt0.Value() != cdIt1.Value() )
        {
        std::cerr << "Input mesh and output mesh are different in cell data!" << std::endl;
        std::cerr << "Input = "  << cdIt0.Value() << std::endl;
        std::cerr << "Output = " << cdIt1.Value() << std::endl;
        return EXIT_FAILURE;
        }
      cdIt0++;
      cdIt1++;
      }
    }
  else
    {
    if ( cellData0.GetPointer() != cellData1.GetPointer() )
      {
      std::cerr << "Input mesh and output mesh are different in cell data!" << std::endl;
      std::cerr << "pointData0 = "  << cellData0.GetPointer() << std::endl;
      std::cerr << "pointData1 = " << cellData1.GetPointer() << std::endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}
