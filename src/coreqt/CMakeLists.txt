add_library(carteracoreqt STATIC "backend.cpp" "types/types.cpp" "types/symbolquote.cpp" "types/symboldetails.cpp")

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Core Qml Concurrent)
find_package(Boost REQUIRED)

target_include_directories(carteracoreqt
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/
)
target_link_libraries(carteracoreqt PUBLIC carteracore)
target_link_libraries(carteracoreqt PUBLIC Qt5::Core)
target_link_libraries(carteracoreqt PUBLIC Qt5::Qml)
target_link_libraries(carteracoreqt PRIVATE Qt5::Concurrent)
