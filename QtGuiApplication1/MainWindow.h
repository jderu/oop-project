#pragma once
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <string>
#include <QObject>
#include <QScrollArea>
#include <QDialog>
#include <sstream>
#include <QCheckBox>
#include <QRadioButton>
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <set>
#include <QCloseEvent>
#include <QSpinBox>

#include "service.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	service gui_service;
	QWidget *ui, *database, *menu, *drug_initialization;
	QFormLayout *drug_fields;
	QLineEdit *name, *producer, *active_substance, *price;
	QTableWidget *database_table, *recipe_table;
	QSpinBox *sp;
	QListWidget *l;
	bool drug_initialization_window, recipe_enabled;

	QWidget* show_database();
	QWidget* show_database_buttons();
	QWidget* show_menu();
	QWidget* last_row_menu();
	QWidget* first_row_menu();
	QWidget* second_row_menu();
	QWidget* third_row_menu();
	QWidget* createSpinner();

	void make_field(QFormLayout *field, string name, QLineEdit *text);
	void Drug_fields(QWidget *drug_field);
	void Drug_fields_find(QWidget *drug_field);
	void insert_drug_row(int row, Drug drug, QTableWidget *parent = 0);
	void show_initialization(QBoxLayout *parent = 0);
	void show_find(QBoxLayout *parent = 0);
	void exit_initialization();

private slots:
	void add_drug();
	void add_if_valid();
	void edit_if_valid(int row, Drug drug);
	void find_if_valid();
	void remove_drug();
	void edit_drug();
	void update_database(vector<Drug> data, QTableWidget *parent = 0);
	void find_drug();
	void filter_by_price();
	void filter_by_as();
	void make_recipe_window();

signals:

public:
	explicit MainWindow(service _gui_service, QWidget *parent = 0);
	void show();
};

