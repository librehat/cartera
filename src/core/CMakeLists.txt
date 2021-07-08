set(libcarteracore_SRCS
	"http/client.cpp"
    "feed/basicfeed.cpp"
    "parser/jsonparser.cpp"
    "types/feedsource.cpp"
    "types/financialinstrument.cpp"
)

find_package(OpenSSL REQUIRED)
find_package(Boost 1.75 COMPONENTS json system REQUIRED)

add_library(carteracore STATIC ${libcarteracore_SRCS})

target_include_directories(carteracore
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/
)
target_link_libraries(carteracore PRIVATE ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})

add_subdirectory(tests)
