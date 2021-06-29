import QtQuick 2.12
import QtQuick.Controls 2.12 as Controls
import QtQuick.Layouts 1.11
import org.kde.kirigami 2.10 as Kirigami
import com.librehat.cartera 1.0

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

    Component.onCompleted: {
        Backend.searchSymbols("IBM", function(results) {
            for (let i in results) {
                console.log(i, results[i]);
            }
        });
        Backend.getQuote("VOD.L", function(quote) {
            console.log("VOD.L quote", quote);
        });
    }
}
