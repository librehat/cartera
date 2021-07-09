import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import QtQuick.Controls 2.12 as Controls
import org.kde.kirigami 2.10 as Kirigami
import "/code/backend.js" as Backend

Kirigami.ScrollablePage {
	id: root

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

	ListView {
		spacing: Kirigami.Units.smallSpacing
		model: ListModel { id: searchListModel }
		delegate: SearchItemDelegate {}
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
			// TODO: handle the error
		}).then(() => {
			root.refreshing = false;
		});
	}
}
