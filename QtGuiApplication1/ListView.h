#pragma once
#include "domain.h"
#include <QtWidgets>
#include <vector>


class ListView :public QAbstractListModel
{
private:
	std::vector<Drug> drugs;
public:
	ListView(std::vector<Drug> _drugs) :drugs{_drugs} {}
	int rowCount(const QModelIndex &parent = QModelIndex()) const override
	{
		parent;
		return (int)drugs.size();
	}
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole)
		{
			auto sp = drugs[index.row()].get_name();
			return QString::fromStdString(sp);
		}
		if (role == Qt::UserRole)
		{
			auto tp = drugs[index.row()].get_producer();
			return QString::fromStdString(tp);
		}
		if (role == Qt::BackgroundRole)
		{
			auto tp = drugs[index.row()].get_active_substance();
			return QString::fromStdString(tp);
		}
		return QVariant{};
	}
};

