add_library(carteracoreqt STATIC "backend.cpp" "types.cpp")

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Core Qml Concurrent)

target_include_directories(carteracoreqt
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/
)
target_link_libraries(carteracoreqt PUBLIC carteracore)
target_link_libraries(carteracoreqt PUBLIC Qt5::Core)
target_link_libraries(carteracoreqt PUBLIC Qt5::Qml)
target_link_libraries(carteracoreqt PRIVATE Qt5::Concurrent)
