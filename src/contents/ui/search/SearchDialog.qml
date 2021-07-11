import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import QtQuick.Controls 2.15
import org.kde.kirigami 2.10 as Kirigami
import "qrc:/code/backend.js" as Backend

Dialog {
    id: root

    property bool refreshing: false
    property var selected: resultView.currentIndex !== -1 ? searchListModel.get(resultView.currentIndex) : null

    title: "Search Symbols"

    standardButtons: Dialog.Ok | Dialog.Cancel

    header: ToolBar {
        enabled: !root.refreshing

        RowLayout {
            anchors.fill: parent
            TextField {
                id: searchTextField
                placeholderText: "Keyword"
                Layout.fillWidth: true
            }
            ToolButton {
                text: "Search"
                onClicked: root.onSearch()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Kirigami.InlineMessage {
            id: errorMessage
            Layout.fillWidth: true
            Layout.margins: Kirigami.Units.smallSpacing
            type: Kirigami.MessageType.Error
            showCloseButton: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                id: resultView
                visible: !root.refreshing
                spacing: Kirigami.Units.smallSpacing
                model: ListModel { id: searchListModel }
                delegate: SearchItemDelegate {
                    multiSelectable: false
                }
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        visible: root.refreshing
        running: root.refreshing
    }

    function clear() {
        searchListModel.clear();
        searchTextField.clear();
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
            console.error(error);
            errorMessage.text = error;
            errorMessage.visible = true;
        }).then(() => {
            root.refreshing = false;
        });
    }
}
