add_subdirectory(core)
add_subdirectory(coreqt)

set(cartera_SRCS
    main.cpp
    resources.qrc
)
add_executable(cartera ${cartera_SRCS})

target_include_directories(cartera PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/coreqt/)
target_include_directories(cartera PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/)

target_link_libraries(cartera PRIVATE Qt5::Core)
target_link_libraries(cartera PRIVATE Qt5::Quick)
target_link_libraries(cartera PRIVATE Qt5::Qml)
target_link_libraries(cartera PRIVATE Qt5::Gui)
target_link_libraries(cartera PRIVATE Qt5::QuickControls2)
target_link_libraries(cartera PRIVATE Qt5::Widgets)
target_link_libraries(cartera PRIVATE KF5::Kirigami2)
target_link_libraries(cartera PRIVATE KF5::I18n)
target_link_libraries(cartera PRIVATE carteracoreqt)
