set(libcarteracore_SRCS
	"http/client.cpp"
    "feed/basicfeed.cpp"
    "parser/jsonparser.cpp"
    "types/feedsource.cpp"
    "types/financialinstrument.cpp"
)

find_package(Boost 1.75 COMPONENTS json REQUIRED)

set(CPR_BUILD_TESTS OFF)
set(CPR_FORCE_USE_SYSTEM_CURL ON)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG f4622efcb59d84071ae11404ae61bd821c1c344b) # the commit hash for 1.6.2
FetchContent_MakeAvailable(cpr)

add_library(carteracore STATIC ${libcarteracore_SRCS})

target_include_directories(carteracore
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/
)
target_link_libraries(carteracore PRIVATE ${Boost_LIBRARIES} cpr::cpr)

add_subdirectory(tests)
