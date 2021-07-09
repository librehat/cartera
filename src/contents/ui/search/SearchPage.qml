import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import QtQuick.Controls 2.12 as Controls
import org.kde.kirigami 2.10 as Kirigami
import "qrc:/code/backend.js" as Backend

Kirigami.ScrollablePage {
    id: root

    signal errored(string message)
    signal confirmed(var symbolIdentifiers)

    header: Controls.ToolBar {
        RowLayout {
            anchors.fill: parent
            Controls.TextField {
                id: searchTextField
                placeholderText: "Keyword"
                Layout.fillWidth: true
            }
            Controls.ToolButton {
                text: "Search"
                onClicked: root.onSearch()
            }
        }
    }
    
    contextualActions: [
        Kirigami.Action {
            id: multiSelectAction
            text: "Multi-Select"
            checkable: true
        }
    ]

    mainAction: Kirigami.Action {
        text: "Confirm"

        onTriggered: {
            // TODO
        }
    }

    ListView {
        spacing: Kirigami.Units.smallSpacing
        model: ListModel { id: searchListModel }
        delegate: SearchItemDelegate {
            multiSelectable: multiSelectAction.checked
        }
    }

    function onSearch() {
        root.refreshing = true;
        Backend.searchSymbols(searchTextField.text)
        .then((results) => {
            searchListModel.clear();
            results.forEach((item) => {
                searchListModel.append(item);
            });
        }).catch((error) => {
            root.errored(error);
        }).then(() => {
            root.refreshing = false;
        });
    }
}
