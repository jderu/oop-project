#pragma once
#include "CosReadOnlyGUI.h"

class CosCRUDGUI :public QWidget, public Observer
{
	Q_OBJECT
private:
	service *gui_service;
	QTableWidget* recipe_table;
	void update_database(vector<Drug> data, QTableWidget *parent = 0);
	void insert_drug_row(int row, Drug drug, QTableWidget *parent = 0);
	QWidget* createSpinner(QSpinBox* sp);

public:
	CosCRUDGUI(service &_gui_service, Observers* obs);
	void update()override;
};

