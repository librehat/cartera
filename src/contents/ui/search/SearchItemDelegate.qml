import org.kde.kirigami 2.10 as Kirigami

// Delegate for SymbolSearchResult type
Kirigami.BasicListItem {
	label: symbol
	subtitle: `${name} (${exchangeCode}) (${typeDisp})`
}