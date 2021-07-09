import QtQuick 2.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15 as Controls
import org.kde.kirigami 2.15 as Kirigami
import "qrc:/code/backend.js" as Backend

Kirigami.ScrollablePage {
    id: root

    signal errored(string message)
    
    supportsRefreshing: true

    contextualActions: [
        Kirigami.Action {
            text: "Refresh"
            enabled: !root.refreshing
            onTriggered: {
                root.refreshing = true;
            }
        }
    ]

    ListView {
        spacing: Kirigami.Units.smallSpacing
        model: ListModel { id: watchListModel }
        delegate: WatchListItem { }
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
        // TODO
        let symbols = ["IBM", "VOD.L", "MA", "BTCUSDT"];
        let sources = [0, 0, 0, 1];
        Backend.getQuotes(symbols, sources)
        .then((results) => {
            console.log(results);
        }).catch((error) => {
            root.errored(error);
        }).then(() => {
            root.refreshing = false;
        });
    }
}
