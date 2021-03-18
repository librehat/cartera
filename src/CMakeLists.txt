set(cartera_SRCS
    main.cpp
    resources.qrc
)
add_executable(cartera ${cartera_SRCS})
target_link_libraries(cartera Qt5::Core Qt5::Quick Qt5::Qml Qt5::Gui Qt5::QuickControls2 Qt5::Widgets KF5::Kirigami2 KF5::I18n)
