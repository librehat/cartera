set(libcarteracore_SRCS
	"http/client.cpp"
    "config/jsonconfigurator.cpp"
    "feed/api.cpp"
    "feed/basicfeed.cpp"
    "feed/jsonparser.cpp"
    "types/feedsource.cpp"
    "types/financialinstrument.cpp"
)

find_package(OpenSSL REQUIRED)
find_package(Boost 1.75 COMPONENTS json system REQUIRED)
find_package(Threads)

add_library(carteracore STATIC ${libcarteracore_SRCS})

target_include_directories(carteracore
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/
)
target_link_libraries(carteracore PRIVATE ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
if (Threads_FOUND)  # -lpthread may be needed
    target_link_libraries(carteracore PRIVATE Threads::Threads)
endif()

add_subdirectory(tests)
