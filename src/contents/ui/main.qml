import QtQuick 2.12
import QtQuick.Controls 2.12 as Controls
import QtQuick.Layouts 1.11
import org.kde.kirigami 2.10 as Kirigami
import com.librehat.cartera 1.0
import "./search" as Search

Kirigami.ApplicationWindow {
    id: root

    title: "Cartera"

    // Initial page to be loaded on app load
    pageStack.initialPage: Search.SearchPage {
    }
}
