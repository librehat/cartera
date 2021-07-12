import QtQuick 2.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import org.kde.kirigami 2.15 as Kirigami
import "qrc:/code/backend.js" as Backend
import "qrc:/search" as Search

Kirigami.ScrollablePage {
    id: root

    property var pageStack: null

    title: "Watch Lists"

    mainAction: Kirigami.Action {
        text: "Add"
        onTriggered: {
            addDlg.open();
            addDlg.clear();
        }
    }

    Dialog {
        id: addDlg
        anchors.centerIn: parent
        focus: true
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        TextField {
            id: newListNameTextField
            anchors.fill: parent
            placeholderText: "New List Name"
        }
        function clear() {
            newListNameTextField.clear();
        }
        onAccepted: {
            Backend.saveWatchList(newListNameTextField.text, [], [])
            .then(root.refreshData)
            .catch(root.handleError);
        }
    }

    ListModel {
        id: model
    }
    
    ListView {
        spacing: Kirigami.Units.smallSpacing

        model: model

        delegate: Kirigami.BasicListItem {
            label: name
        }
    }

    function handleError(errorMessage) {
        console.error(errorMessage);
        // TODO show error message visually
    }

    function refreshData() {
        Backend.getAllWatchListNames()
        .then((names) => {
            model.clear();
            names.forEach((name) => {
                model.append({ "name": name });
            });
        }).catch(root.handleError);
    }

    Component.onCompleted: {
        root.refreshData();
    }
}
