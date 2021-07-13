import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import QtQuick.Controls 2.15
import org.kde.kirigami 2.15 as Kirigami
import "qrc:/code/backend.js" as Backend

Kirigami.ScrollablePage {
    id: root

    required property string symbol
    required property int source
    title: symbol

    supportsRefreshing: false

    ListModel {
        id: model
    }

    // We display the details declaratively
    ListView {
        spacing: Kirigami.Units.smallSpacing

        model: model

        delegate: RowLayout {
            width: parent.width
            Label {
                text: title
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
            }
            Label {
                text: display
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    function handleError(errorMessage) {
        console.error(errorMessage);
        // TODO show error message visually
    }

    Component.onCompleted: {
        Backend.getSymbolDetails(symbol, source)
        .then((details) => {
            model.clear();
            details.details.forEach(item => {
                model.append(item);
            });
        }).catch(root.handleError);
    }
}
