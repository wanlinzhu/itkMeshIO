set(PROJECT_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})
set(TEST_DATA_ROOT ${MeshIO_SOURCE_DIR}/Data)
set(TEST_OUTPUT ${MeshIO_BINARY_DIR}/Testing/Temporary)

add_executable(MeshFileReadWriteTest MeshFileReadWriteTest.cxx )
target_link_libraries(MeshFileReadWriteTest ITKMeshIO)

add_executable(MeshFileReadWriteVectorAttributeTest MeshFileReadWriteVectorAttributeTest.cxx )
target_link_libraries(MeshFileReadWriteVectorAttributeTest ITKMeshIO)

add_executable(PolylineReadWriteTest PolylineReadWriteTest.cxx )
target_link_libraries(PolylineReadWriteTest ITKMeshIO)

add_test(MeshFileReadWriteTest_1
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/input.vtk
	${TEST_OUTPUT}/output.vtk
	)

add_test(MeshFileReadWriteTest_2
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere.fsb
	${TEST_OUTPUT}/lh_sphere.vtk
	)
add_test(MeshFileReadWriteTest_3
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere.fsb
	${TEST_OUTPUT}/lh_sphere.fsb
	)
add_test(MeshFileReadWriteTest_4
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere_thickness.vtk
	${TEST_OUTPUT}/lh_sphere_thickness.vtk
	)
add_test(MeshFileReadWriteTest_5
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.thickness.fcv
	${TEST_OUTPUT}/lh_thickness.fcv
)
add_test(MeshFileReadWriteTest_6
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/rh.pial.fsa
	${TEST_OUTPUT}/rh_pial.fsa
)

add_test(MeshFileReadWriteTest_7
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/cube.byu
	${TEST_OUTPUT}/cube.byu
	)
add_test(MeshFileReadWriteTest_8
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.sphere_thickness_b.vtk
	${TEST_OUTPUT}/lh_sphere_thickness_a.vtk
	1
	)
add_test(MeshFileReadWriteTest_9
	${PROJECT_TEST_PATH}/MeshFileReadWriteVectorAttributeTest
	${TEST_DATA_ROOT}/sphere_normals.vtk
	${TEST_OUTPUT}/sphere_normals.vtk
	)
add_test(MeshFileReadWriteTest_10
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/octa.off
	${TEST_OUTPUT}/octa.off
	)
add_test(MeshFileReadWriteTest_11
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/box.obj
	${TEST_OUTPUT}/box.obj
	)
add_test(MeshFileReadWriteTest_12
	${PROJECT_TEST_PATH}/PolylineReadWriteTest
	${TEST_DATA_ROOT}/fibers.vtk
	${TEST_OUTPUT}/fibers.vtk
	)
add_test(MeshFileReadWriteTest_13
	${PROJECT_TEST_PATH}/PolylineReadWriteTest
	${TEST_DATA_ROOT}/fibers.vtk
	${TEST_OUTPUT}/fibers_binary.vtk
	1
	)
add_test(MeshFileReadWriteTest_14
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.bert.pial.gii
	${TEST_OUTPUT}/lh.bert.pial.gii
	)
add_test(MeshFileReadWriteTest_15
	${PROJECT_TEST_PATH}/MeshFileReadWriteTest
	${TEST_DATA_ROOT}/lh.aparc.gii
	${TEST_OUTPUT}/lh.aparc.gii
	)
