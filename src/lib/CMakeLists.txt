set(libcarteracore_SRCS
    feed/abstractfeed.cpp
    feed/feedreplyprivate.cpp
    feed/financialinstrument.cpp
    feed/yahoofinancefeed.cpp
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/)
add_library(carteracore ${libcarteracore_SRCS})
target_link_libraries(carteracore Qt5::Core Qt5::Network)
