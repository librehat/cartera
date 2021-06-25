find_package(Boost COMPONENTS filesystem REQUIRED)

enable_testing()
add_executable(carteracoretest jsonparser.t.cpp)

if(WIN32)
    set_target_properties(carteracoretest PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(carteracoretest PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
    set_target_properties(carteracoretest PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
    set_target_properties(carteracoretest PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
    set_target_properties(carteracoretest PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
    set_target_properties(carteracoretest PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:CONSOLE")
endif(WIN32)


target_include_directories(carteracoretest
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../
)
target_link_libraries(carteracoretest carteracore ${Boost_LIBRARIES})

add_test(NAME jsonparsertest COMMAND carteracoretest WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
