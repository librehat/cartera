set(libcarteracore_SRCS
    feed/abstractfeed.cpp
    feed/feedreplyprivate.cpp
    feed/feedrepository.cpp
    feed/financialinstrument.cpp
    feed/yahoofinancefeed.cpp
    book/book.cpp
    book/position.cpp
    book/transaction.cpp
)

# It's a private library, build and link it statically to avoid having to deal with Qt shared library stuff
add_library(carteracore STATIC ${libcarteracore_SRCS})

target_include_directories(carteracore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/)

target_link_libraries(carteracore PUBLIC Qt5::Core)
target_link_libraries(carteracore PUBLIC Qt5::Network)
