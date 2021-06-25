set(libcarteracore_SRCS
    "parser/jsonparser.cpp"
    "types/financialinstrument.cpp"
)

# beast is header-only, we only need to specify the ones that have separate libraries here
find_package(Boost 1.75 COMPONENTS json REQUIRED)

add_library(carteracore STATIC ${libcarteracore_SRCS})

target_include_directories(carteracore
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/
)
target_link_libraries(carteracore PUBLIC ${Boost_LIBRARIES})

add_subdirectory(tests)
