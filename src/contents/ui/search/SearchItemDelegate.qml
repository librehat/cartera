import QtQuick.Controls 2.12 as Controls
import org.kde.kirigami 2.12 as Kirigami


Kirigami.BasicListItem {
    property bool multiSelectable: false

    label: symbol
    subtitle: `${name} (${exchangeCode}) (${typeDisp})`

    leading: Controls.CheckBox {
        id: checkbox
        visible: multiSelectable
    }

    onMultiSelectableChanged: {
        checkbox.checked = false;
    }
}
