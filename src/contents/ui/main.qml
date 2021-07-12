import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.15 as Kirigami
import com.librehat.cartera 1.0 as Cartera
import "qrc:/watch" as Watch

Kirigami.ApplicationWindow {
    id: root

    title: "Cartera"

    // Initial page to be loaded on app load
    pageStack.initialPage: Watch.WatchListSelectionPage {
        pageStack: root.pageStack
    }
}
