import QtQuick 2.12
import QtQuick.Controls 2.12 as Controls
import QtQuick.Layouts 1.11
import org.kde.kirigami 2.10 as Kirigami

Kirigami.ApplicationWindow {
    id: root

    title: "Cartera"

    // Initial page to be loaded on app load
    pageStack.initialPage: Kirigami.Page {
        Controls.Label {
            // Center label horizontally and vertically within parent element
            anchors.centerIn: parent
            text: i18n("Hello World!")
        }
    }
}
