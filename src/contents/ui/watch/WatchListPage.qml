import QtQuick 2.15
import QtQml.Models 2.15
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.15 as Controls
import org.kde.kirigami 2.15 as Kirigami
import "qrc:/code/backend.js" as Backend

Kirigami.ScrollablePage {
    id: root

    signal errored(string message)
    
    supportsRefreshing: true

    contextualActions: [
        Kirigami.Action {
            text: "Add"
            onTriggered: {
                // TODO: show search to add symbols
            }
        },
        Kirigami.Action {
            text: "Refresh"
            enabled: !root.refreshing
            onTriggered: {
                root.refreshing = true;
            }
        }
    ]
    
    ListModel {
        id: watchListModel
    }

    ListView {
        spacing: Kirigami.Units.smallSpacing

        model: watchListModel

        delegate: WatchListItem {}
    }

    Connections {
        function onRefreshingChanged() {
            if (!root.refreshing) {
                return;
            }
            refreshData();
        }
    }

    function refreshData() {
        let symbols = ["IBM", "VOD.L", "MA", "BTCUSDT"];
        let sources = [0, 0, 0, 1];
        Backend.getSymbolQuotes(symbols, sources)
        .then((results) => {
            watchListModel.clear();
            results.forEach((item) => {
                watchListModel.append(item);
            });
        }).catch((error) => {
            root.errored(error);
        }).then(() => {
            root.refreshing = false;
        });
    }
}
