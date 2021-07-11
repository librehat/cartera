import QtQuick 2.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15 as Controls
import org.kde.kirigami 2.15 as Kirigami
import "qrc:/code/backend.js" as Backend
import "qrc:/search" as Search

Kirigami.ScrollablePage {
    id: root

    property string listName: ""

    title: "Watch List"

    supportsRefreshing: true

    mainAction: Kirigami.Action {
        text: "Add"
        onTriggered: searchDlg.open()
    }

    contextualActions: [
        Kirigami.Action {
            text: "Refresh"
            enabled: !root.refreshing && watchListModel.count > 0
            onTriggered: {
                root.refreshing = true;
            }
        }
    ]

    Search.SearchDialog {
        id: searchDlg

        width: parent.width
        height: parent.height

        onAccepted: {
            root.addSymbol(searchDlg.selected.symbol, searchDlg.selected.source);
            searchDlg.clear();
        }
        onRejected: searchDlg.clear()
    }
    
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

    function handleError(errorMessage) {
        console.error(errorMessage);
        // TODO show error message visually
    }

    function refreshData() {
        const symbols = [];
        const sources = [];
        for (let i = 0; i < watchListModel.count; ++i) {
            const item = watchListModel.get(i);
            symbols.push(item.symbol);
            sources.push(item.source);
        }
        Backend.getSymbolQuotes(symbols, sources)
        .then((results) => {
            for (let i in results) {
                watchListModel.set(i, results[i]);
            }
        }).catch((error) => root.handleError)
        .then(() => {
            root.refreshing = false;
        });
    }

    function addSymbol(symbol, source) {
        Backend.getSymbolQuotes([symbol], [source])
        .then((results) => {
            watchListModel.append(results[0]);
        }).catch((error) => root.handleError);
    }
}
