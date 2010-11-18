SET(PROJECT_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})
SET(TEST_DATA_ROOT ${CMAKE_SOURCE_DIR}/Data)
SET(TEST_OUTPUT ${CMAKE_BINARY_DIR}/Testing/Temporary)

ADD_EXECUTABLE(MeshFileReadWriteTest MeshFileReadWriteTest.cxx )
TARGET_LINK_LIBRARIES(MeshFileReadWriteTest ITKQuadEdgeMesh ITKMeshIO)

ADD_EXECUTABLE(MeshFileReadWriteVectorAttributeTest MeshFileReadWriteVectorAttributeTest.cxx )
TARGET_LINK_LIBRARIES(MeshFileReadWriteVectorAttributeTest ITKQuadEdgeMesh ITKMeshIO)

ADD_EXECUTABLE(PolylineReadWriteTest PolylineReadWriteTest.cxx )
TARGET_LINK_LIBRARIES(PolylineReadWriteTest ITKQuadEdgeMesh ITKMeshIO)

ADD_TEST(MeshFileReadWriteTest_1 
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/input.vtk
	${TEST_OUTPUT}/output.vtk
	)

ADD_TEST(MeshFileReadWriteTest_2 
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere.fsb
	${TEST_OUTPUT}/lh_sphere.vtk
	)
ADD_TEST(MeshFileReadWriteTest_3 
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere.fsb
	${TEST_OUTPUT}/lh_sphere.fsb
	)
ADD_TEST(MeshFileReadWriteTest_4
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere_thickness.vtk
	${TEST_OUTPUT}/lh_sphere_thickness.vtk
	)
ADD_TEST(MeshFileReadWriteTest_5 
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.thickness.fcv
	${TEST_OUTPUT}/lh_thickness.fcv
)
ADD_TEST(MeshFileReadWriteTest_6 
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/rh.pial.fsa
	${TEST_OUTPUT}/rh_pial.fsa
)

ADD_TEST(MeshFileReadWriteTest_7 
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/cube.byu
	${TEST_OUTPUT}/cube.byu
	)
ADD_TEST(MeshFileReadWriteTest_8
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere_thickness_b.vtk
	${TEST_OUTPUT}/lh_sphere_thickness_a.vtk
	1
	)
ADD_TEST(MeshFileReadWriteTest_9
	${PROJECT_TEST_PATH}/MeshFileReadWriteVectorAttributeTest
	${TEST_DATA_ROOT}/sphere_normals.vtk
	${TEST_OUTPUT}/sphere_normals.vtk
	)
ADD_TEST(MeshFileReadWriteTest_10
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/octa.off
	${TEST_OUTPUT}/octa.off
	)
ADD_TEST(MeshFileReadWriteTest_11
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/box.obj
	${TEST_OUTPUT}/box.obj
	)
ADD_TEST(MeshFileReadWriteTest_12 
	${PROJECT_TEST_PATH}/PolylineReadWriteTest
	${TEST_DATA_ROOT}/fibers.vtk
	${TEST_OUTPUT}/fibers.vtk
	)
ADD_TEST(MeshFileReadWriteTest_13 
	${PROJECT_TEST_PATH}/PolylineReadWriteTest
	${TEST_DATA_ROOT}/fibers.vtk
	${TEST_OUTPUT}/fibers_binary.vtk
	1
	)
ADD_TEST(MeshFileReadWriteTest_14 
	${PROJECT_TEST_PATH}/PolylineReadWriteTest
	${TEST_DATA_ROOT}/lh.bert.pial.gii
	${TEST_OUTPUT}/lh.bert.pial.gii
	)
ADD_TEST(MeshFileReadWriteTest_15 
	${PROJECT_TEST_PATH}/PolylineReadWriteTest
	${TEST_DATA_ROOT}/lh.aparc.gii
	${TEST_OUTPUT}/lh.aparc.gii
	)