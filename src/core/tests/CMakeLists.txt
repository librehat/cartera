find_package(Boost COMPONENTS filesystem REQUIRED)

enable_testing()

list(APPEND CARTERACORE_UNIT_TESTS jsonconfigurator.t jsonparser.t httpclient.t basicfeed.t feedapi.t)


foreach(UNIT_TEST IN LISTS CARTERACORE_UNIT_TESTS)
	add_executable(${UNIT_TEST} ${UNIT_TEST}.cpp)
	if(WIN32)
		set_target_properties(${UNIT_TEST} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
		set_target_properties(${UNIT_TEST} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
		set_target_properties(${UNIT_TEST} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
		set_target_properties(${UNIT_TEST} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
		set_target_properties(${UNIT_TEST} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
		set_target_properties(${UNIT_TEST} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
	endif(WIN32)

	target_include_directories(${UNIT_TEST}
		PRIVATE ${Boost_INCLUDE_DIR}
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
	)
	target_link_libraries(${UNIT_TEST} carteracore ${Boost_LIBRARIES})

	add_test(NAME "${UNIT_TEST}" COMMAND ${UNIT_TEST} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

