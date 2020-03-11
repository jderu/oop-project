#pragma once
#include "domain.h"
#include <QtWidgets>
#include <vector>

class TableView :public QAbstractTableModel
{
private:
	std::vector<Drug> drugs;
public:
	TableView(std::vector<Drug> _drugs) :drugs{_drugs} {}

	int rowCount(const QModelIndex & parent = QModelIndex()) const override
	{
		parent;
		return (int)drugs.size();
	}
	int columnCount(const QModelIndex & parent = QModelIndex()) const override
	{
		parent;
		return 4;
	}
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole)
		{
			Drug p = drugs[index.row()];
			if (index.column() == 0)
				return QString::fromStdString(p.get_name());
			else if (index.column() == 1)
				return QString::fromStdString(p.get_producer());
			else if (index.column() == 2)
				return QString::fromStdString(p.get_active_substance());
			else if (index.column() == 3)
				return QString::number(p.get_price());
		}
		return QVariant{};
	}
};

