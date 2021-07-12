import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import org.kde.kirigami 2.15 as Kirigami

ItemDelegate {
    property var priceColor: isPositivePriceChange ? Kirigami.Theme.positiveTextColor: Kirigami.Theme.negativeTextColor

    anchors.left: parent.left
    anchors.right: parent.right
    height: contentRow.height

    RowLayout {
        id: contentRow
        width: parent.width

        ColumnLayout {
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true

            Label {
                text: symbol
                font.weight: Font.Black
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
            }
            Label {
                text: longName

                // Sometimes it has HTML encoded characters
                // StyledText will render them nicely (and more performant than RichText)
                textFormat: Text.StyledText
                elide: Text.ElideMiddle

                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignRight
            
            Label {
                text: currentPriceDisp
                color: priceColor
                Layout.alignment: Qt.AlignRight
            }
            Label {
                text: priceChangeDisp + " (" + priceChangePercentDisp + ")"
                color: priceColor
                Layout.alignment: Qt.AlignRight
            }
        }
    }
}