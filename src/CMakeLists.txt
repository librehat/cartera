add_subdirectory(lib)

set(cartera_SRCS
    main.cpp
    resources.qrc
)
add_executable(cartera ${cartera_SRCS})
target_include_directories(cartera PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/)
target_link_libraries(cartera libcartera Qt5::Core Qt5::Quick Qt5::Qml Qt5::Gui Qt5::QuickControls2 Qt5::Widgets KF5::Kirigami2 KF5::I18n)
