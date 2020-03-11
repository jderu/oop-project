#include "MainWindow.h"

MainWindow::MainWindow(service _gui_service, QWidget * parent) : QMainWindow(parent)
{
	gui_service = _gui_service;
	drug_initialization_window = false;
	recipe_enabled = false;
}

QWidget* MainWindow::createSpinner()
{
	QWidget* temp = new QWidget;
	QHBoxLayout* temp_layout = new QHBoxLayout;
	temp->setLayout(temp_layout);

	sp = new QSpinBox();
	sp->setMaximum(25);
	sp->setMinimum(0);
	temp_layout->addWidget(sp);
	QSlider *sl = new QSlider(Qt::Horizontal);
	sl->setMaximum(25);
	sl->setMinimum(0);
	temp_layout->addWidget(sl);
	QObject::connect(sp, SIGNAL(valueChanged(int)), sl, SLOT(setValue(int)));
	QObject::connect(sl, &QSlider::valueChanged, [this](int val) { sp->setValue(val); });
	return temp;
}

void MainWindow::add_drug()
{
	if (drug_initialization_window == false)
	{
		drug_initialization_window = true;
		QBoxLayout *drug_initialization_layout = new QBoxLayout(QBoxLayout::BottomToTop);

		QPushButton *exit = new QPushButton("EXIT");
		exit->setStyleSheet("background-color:rgba(242,38,19,1)");
		drug_initialization_layout->addWidget(exit);
		connect(exit, &QPushButton::clicked, [this]() { exit_initialization(); });

		QPushButton *add = new QPushButton("ADD");
		drug_initialization_layout->addWidget(add);
		connect(add, SIGNAL(clicked()), this, SLOT(add_if_valid()));

		show_initialization(drug_initialization_layout);
	}
}

void MainWindow::add_if_valid()
{
	string Sname = name->text().toStdString();
	string Sproducer = producer->text().toStdString();
	string Sactive_substance = active_substance->text().toStdString();
	int Sprice;
	try
	{
		Sprice = stoi(price->text().toStdString());
		Drug drug{Sname, Sproducer, Sactive_substance, Sprice};
		try
		{
			gui_service.add_drug(drug);
			exit_initialization();

			update_database(gui_service.print_drugs());
			//database_table->insertRow(database_table->rowCount());
			//insert_drug_row(database_table->rowCount() - 1, drug, database_table);
		}
		catch (ServiceException const &msg)
		{
			string err = msg.error_message[0];
			QPushButton *btn = new QPushButton(err.c_str());
			connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
			btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
			btn->show();
		}
	}
	catch (...)
	{
		string err = "Invalid number.\n";
		QPushButton *btn = new QPushButton(err.c_str());
		connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
		btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		btn->show();
	}
}

void MainWindow::edit_if_valid(int row, Drug drug)
{
	string Sname = name->text().toStdString();
	string Sproducer = producer->text().toStdString();
	string Sactive_substance = active_substance->text().toStdString();
	int Sprice;
	try
	{
		Sprice = stoi(price->text().toStdString());
		Drug drug1{Sname, Sproducer, Sactive_substance, Sprice};
		try
		{
			gui_service.change_drug(drug, drug1);
			exit_initialization();

			row = row;
			update_database(gui_service.print_drugs());
			//insert_drug_row(row, drug1, database_table);
		}
		catch (ServiceException const &msg)
		{
			string err = msg.error_message[0];
			QPushButton *btn = new QPushButton(err.c_str());
			connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
			btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
			btn->show();
		}
	}
	catch (...)
	{
		string err = "Invalid number.\n";
		QPushButton *btn = new QPushButton(err.c_str());
		connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
		btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		btn->show();
	}
}

void MainWindow::find_if_valid()
{
	string Sname = name->text().toStdString();
	string Sproducer = producer->text().toStdString();
	Drug drug{Sname, Sproducer, "0", 0};
	try
	{
		auto found = gui_service.find_drug(drug);
		exit_initialization();
		database_table->setRowCount(1);
		insert_drug_row(database_table->rowCount() - 1, found, database_table);
	}
	catch (ServiceException const &msg)
	{
		string err = msg.error_message[0];
		QPushButton *btn = new QPushButton(err.c_str());
		connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
		btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		btn->show();
	}
}

void MainWindow::remove_drug()
{
	if (drug_initialization_window == false)
	{
		auto selected_drug = database_table->selectedItems();
		if (selected_drug.size())
		{
			string Sname = selected_drug.at(0)->text().toStdString();
			string Sproducer = selected_drug.at(1)->text().toStdString();
			string Sas = selected_drug.at(2)->text().toStdString();
			int Sprice = stoi(selected_drug.at(3)->text().toStdString());
			Drug drug{Sname, Sproducer, Sas, Sprice};

			gui_service.remove_drug(drug);

			update_database(gui_service.print_drugs());
			//database_table->removeRow(database_table->selectedRanges().at(0).topRow());
		}
	}
}

void MainWindow::edit_drug()
{
	if (drug_initialization_window == false)
	{
		auto selected_drug = database_table->selectedItems();
		if (selected_drug.size())
		{
			drug_initialization_window = true;
			QBoxLayout *drug_initialization_layout = new QBoxLayout(QBoxLayout::BottomToTop);

			QPushButton *exit = new QPushButton("EXIT");
			exit->setStyleSheet("background-color:rgba(242,38,19,1)");
			drug_initialization_layout->addWidget(exit);
			connect(exit, &QPushButton::clicked, [this]() { exit_initialization(); });


			QPushButton *edit = new QPushButton("EDIT");
			drug_initialization_layout->addWidget(edit);

			string Sname = selected_drug.at(0)->text().toStdString();
			string Sproducer = selected_drug.at(1)->text().toStdString();
			string Sas = selected_drug.at(2)->text().toStdString();
			int Sprice = stoi(selected_drug.at(3)->text().toStdString());
			Drug drug{Sname, Sproducer, Sas, Sprice};

			int position = database_table->selectedRanges().at(0).topRow();

			connect(edit, &QPushButton::clicked, [this, position, drug]() { edit_if_valid(position, drug); });
			show_initialization(drug_initialization_layout);

			name->setText(selected_drug.at(0)->text());
			producer->setText(selected_drug.at(1)->text());
			active_substance->setText(selected_drug.at(2)->text());
			price->setText(selected_drug.at(3)->text());
		}
	}
}

void MainWindow::show()
{
	ui = new QWidget;
	auto ui_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	ui->setGeometry(400, 250, 1200, 500);
	ui->setLayout(ui_layout);
	ui->setStyleSheet("background-color:rgba(255,255,255, 1);");

	l = new QListWidget;
	l->setFixedWidth(200);

	ui_layout->addWidget(show_database());
	ui_layout->addWidget(l);
	ui_layout->addWidget(show_menu());

	ui->show();
}

QWidget* MainWindow::show_database()
{
	QWidget *database_window = new QWidget;
	database_window->setStyleSheet("background-color:rgba(255,255,170,1)");
	QBoxLayout *database_window_layout = new QBoxLayout(QBoxLayout::BottomToTop);
	database_window->setLayout(database_window_layout);

	database_window_layout->addWidget(show_database_buttons());

	database_table = new QTableWidget{0,4};
	database_table->setHorizontalHeaderLabels(QStringList({"Name", "Producer", "Active Substance", "Price"}));
	auto header_view = new QHeaderView(Qt::Vertical);
	header_view->setVisible(false);

	database_table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	database_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	database_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	database_table->setSelectionMode(QAbstractItemView::SingleSelection);
	database_table->setVerticalHeader(header_view);

	database_window_layout->addWidget(database_table);
	update_database(gui_service.print_drugs());
	return database_window;
}

QWidget * MainWindow::show_database_buttons()
{
	QWidget *buttons = new QWidget;
	buttons->setStyleSheet("background-color:rgba(170,255,255,1)");
	QBoxLayout *buttons_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	buttons->setLayout(buttons_layout);
	buttons_layout->setContentsMargins(0, 0, 0, 0);

	auto add_drug_button = new QPushButton("ADD");
	connect(add_drug_button, SIGNAL(clicked()), this, SLOT(add_drug()));
	buttons_layout->addWidget(add_drug_button);

	auto remove_drug_button = new QPushButton("REMOVE");
	connect(remove_drug_button, SIGNAL(clicked()), this, SLOT(remove_drug()));
	buttons_layout->addWidget(remove_drug_button);

	auto edit_drug_button = new QPushButton("EDIT");
	connect(edit_drug_button, SIGNAL(clicked()), this, SLOT(edit_drug()));
	buttons_layout->addWidget(edit_drug_button);

	return buttons;
}

QWidget* MainWindow::show_menu()
{
	menu = new QWidget();
	auto menu_layout = new QBoxLayout(QBoxLayout::TopToBottom);
	menu->setLayout(menu_layout);
	menu->setFixedWidth((ui->width()-150) - database_table->width());
	menu->setStyleSheet("background-color:rgba(170,170,255,1)");

	menu_layout->addWidget(first_row_menu());
	menu_layout->addWidget(second_row_menu());
	menu_layout->addWidget(third_row_menu());

	menu_layout->addStretch(true);
	menu_layout->addWidget(last_row_menu());

	return menu;
}

QWidget * MainWindow::last_row_menu()
{
	QPushButton *exit_button = new QPushButton("Exit", this);
	exit_button->setMinimumWidth(150);
	exit_button->setStyleSheet("background-color:rgba(242,38,19,1)");
	connect(exit_button, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

	QPushButton *update_database_button = new QPushButton("Show all drugs", this);
	update_database_button->setMinimumWidth(150);
	update_database_button->setStyleSheet("background-color:rgba(38,242,19,1)");
	connect(update_database_button, &QPushButton::clicked, [this]() { update_database(gui_service.print_drugs()); });

	auto menu_last_row = new QWidget();
	auto menu_last_row_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	menu_last_row_layout->setMargin(0);
	menu_last_row->setLayout(menu_last_row_layout);

	menu_last_row_layout->addWidget(update_database_button);
	menu_last_row_layout->addStretch(true);
	menu_last_row_layout->addWidget(exit_button);

	return menu_last_row;
}

QWidget * MainWindow::first_row_menu()
{
	auto menu_row = new QWidget();
	auto menu_row_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	menu_row_layout->setContentsMargins(0, 0, 0, 0);
	menu_row->setLayout(menu_row_layout);

	auto find_drug_button = new QPushButton("FIND");
	connect(find_drug_button, SIGNAL(clicked()), this, SLOT(find_drug()));

	auto make_recipe = new QPushButton("MAKE RECIPE");
	connect(make_recipe, SIGNAL(clicked()), this, SLOT(make_recipe_window()));

	auto undo_button = new QPushButton("UNDO");
	connect(undo_button, &QPushButton::clicked, [this]()
	{
		try
		{
			gui_service.undo();
			update_database(gui_service.print_drugs());
		}
		catch (ServiceException const &msg)
		{
			string err = msg.error_message[0];
			QPushButton *btn = new QPushButton(err.c_str());
			connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
			btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
			btn->show();
		}
	});

	menu_row_layout->addWidget(find_drug_button);
	menu_row_layout->addStretch(true);
	menu_row_layout->addWidget(undo_button);
	menu_row_layout->addStretch(true);
	menu_row_layout->addWidget(make_recipe);

	return menu_row;
}

QWidget * MainWindow::second_row_menu()
{
	auto menu_row = new QWidget();
	auto menu_row_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	menu_row_layout->setContentsMargins(0, 10, 0, 0);
	menu_row->setLayout(menu_row_layout);

	auto filter = new QLabel("FILTER BY: ");
	menu_row_layout->addWidget(filter);

	auto by_price = new QPushButton("PRICE");
	connect(by_price, &QPushButton::clicked, [this]() { filter_by_price(); });
	menu_row_layout->addWidget(by_price);

	auto by_as = new QPushButton("ACTIVE SUBSTANCE");
	connect(by_as, &QPushButton::clicked, [this]() { filter_by_as(); });
	menu_row_layout->addWidget(by_as);

	menu_row_layout->addStretch(true);
	return menu_row;
}

QWidget * MainWindow::third_row_menu()
{
	auto menu_row = new QWidget();
	auto menu_row_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	menu_row_layout->setContentsMargins(0, 10, 0, 0);
	menu_row->setLayout(menu_row_layout);

	auto sort_by = new QLabel("SORT BY: ");
	menu_row_layout->addWidget(sort_by);

	auto by_name = new QPushButton("NAME");
	by_name->setMinimumWidth(40);
	connect(by_name, &QPushButton::clicked, [this]()
	{
		auto data = gui_service.sort("by_name");
		update_database(data);
	});
	menu_row_layout->addWidget(by_name);

	auto by_producer = new QPushButton("PRODUCER");
	by_producer->setMinimumWidth(80);
	connect(by_producer, &QPushButton::clicked, [this]()
	{
		auto data = gui_service.sort("by_producer");
		update_database(data);
	});
	menu_row_layout->addWidget(by_producer);

	auto by_as_price = new QPushButton("ACTIVE SUBSTANCE AND PRICE");
	connect(by_as_price, &QPushButton::clicked, [this]()
	{
		auto data = gui_service.sort("by_as_price");
		update_database(data);
	});
	menu_row_layout->addWidget(by_as_price);

	menu_row_layout->addStretch(true);
	return menu_row;
}

void MainWindow::show_initialization(QBoxLayout *parent)
{
	drug_initialization = new QWidget;
	drug_initialization->setStyleSheet("background-color:rgba(170,255,170, 1);");
	drug_initialization->setLayout(parent);

	QWidget *drug_init = new QWidget;
	Drug_fields(drug_init);
	parent->addWidget(drug_init);

	drug_initialization->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
	drug_initialization->show();
}

void MainWindow::show_find(QBoxLayout * parent)
{
	drug_initialization = new QWidget;
	drug_initialization->setStyleSheet("background-color:rgba(170,255,170, 1);");
	drug_initialization->setLayout(parent);

	QWidget *drug_init = new QWidget;
	Drug_fields_find(drug_init);
	parent->addWidget(drug_init);

	drug_initialization->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
	drug_initialization->show();
}

void MainWindow::exit_initialization()
{
	drug_initialization->close();
	drug_initialization_window = false;
}

void MainWindow::make_field(QFormLayout * parent, string sname, QLineEdit *text)
{
	QLabel *label = new QLabel(sname.c_str());
	parent->addRow(label, text);
}

void MainWindow::Drug_fields(QWidget * parent)
{
	drug_fields = new QFormLayout;
	parent->setLayout(drug_fields);

	name = new QLineEdit;
	make_field(drug_fields, "Name", name);
	producer = new QLineEdit;
	make_field(drug_fields, "Producer", producer);
	active_substance = new QLineEdit;
	make_field(drug_fields, "Active substance", active_substance);
	price = new QLineEdit;
	make_field(drug_fields, "Price", price);
}

void MainWindow::Drug_fields_find(QWidget * parent)
{
	drug_fields = new QFormLayout;
	parent->setLayout(drug_fields);

	name = new QLineEdit;
	make_field(drug_fields, "Name", name);
	producer = new QLineEdit;
	make_field(drug_fields, "Producer", producer);
}

void MainWindow::update_database(vector<Drug> sdata, QTableWidget* parent)
{
	if (parent == 0)
		parent = database_table;
	int row = 0;
	parent->setRowCount((int)sdata.size());
	for (auto drug : sdata)
	{
		insert_drug_row(row, drug, parent);
		row++;
	}
	auto v = gui_service.sort("by_name");
	l->clear();
	auto n = v.size();
	if (n)
	{
		size_t i = 0;
		auto last = v[i].get_name();
		int q = 0;
		while (i < n)
		{
			if (v[i].get_name() == last)
				q++;
			else
			{
				l->addItem((last + " apears " + to_string(q) + " times").c_str());
				last = v[i].get_name();
				q = 1;
			}
			i++;
		}
		l->addItem((last + " apears " + to_string(q) + " times").c_str());
	}
}

void MainWindow::find_drug()
{
	if (drug_initialization_window == false)
	{
		drug_initialization_window = true;
		QBoxLayout *drug_initialization_layout = new QBoxLayout(QBoxLayout::BottomToTop);

		QPushButton *exit = new QPushButton("EXIT");
		exit->setStyleSheet("background-color:rgba(242,38,19,1)");
		drug_initialization_layout->addWidget(exit);
		connect(exit, &QPushButton::clicked, [this]() { exit_initialization(); });

		QPushButton *find = new QPushButton("FIND");
		drug_initialization_layout->addWidget(find);
		connect(find, SIGNAL(clicked()), this, SLOT(find_if_valid()));

		show_find(drug_initialization_layout);
	}
}

void MainWindow::filter_by_price()
{
	if (drug_initialization_window == false)
	{
		drug_initialization_window = true;
		QBoxLayout *drug_initialization_layout = new QBoxLayout(QBoxLayout::BottomToTop);

		QPushButton *exit = new QPushButton("EXIT");
		exit->setStyleSheet("background-color:rgba(242,38,19,1)");
		drug_initialization_layout->addWidget(exit);
		connect(exit, &QPushButton::clicked, [this]() { exit_initialization(); });

		QPushButton *filter = new QPushButton("FILTER");
		drug_initialization_layout->addWidget(filter);
		connect(filter, &QPushButton::clicked, [this]()
		{
			try
			{
				auto data = gui_service.filter_by_price(stoi(price->text().toStdString()));
				exit_initialization();
				update_database(data);
			}
			catch (...)
			{
				string err = "Invalid number.\n";
				QPushButton *btn = new QPushButton(err.c_str());
				connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
				btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
				btn->show();
			}
		});

		drug_initialization = new QWidget;
		drug_initialization->setStyleSheet("background-color:rgba(170,255,170, 1);");
		drug_initialization->setLayout(drug_initialization_layout);

		QWidget *drug_init = new QWidget;
		drug_fields = new QFormLayout;
		drug_init->setLayout(drug_fields);

		price = new QLineEdit;
		make_field(drug_fields, "Price", price);

		drug_initialization_layout->addWidget(drug_init);

		drug_initialization->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		drug_initialization->show();
	}
}

void MainWindow::filter_by_as()
{
	if (drug_initialization_window == false)
	{
		drug_initialization_window = true;
		QBoxLayout *drug_initialization_layout = new QBoxLayout(QBoxLayout::BottomToTop);

		QPushButton *exit = new QPushButton("EXIT");
		exit->setStyleSheet("background-color:rgba(242,38,19,1)");
		drug_initialization_layout->addWidget(exit);
		connect(exit, &QPushButton::clicked, [this]() { exit_initialization(); });

		QPushButton *filter = new QPushButton("FILTER");
		drug_initialization_layout->addWidget(filter);
		connect(filter, &QPushButton::clicked, [this]()
		{
			auto data = gui_service.filter_by_active_substance(active_substance->text().toStdString());
			exit_initialization();
			update_database(data);
		});

		drug_initialization = new QWidget;
		drug_initialization->setStyleSheet("background-color:rgba(170,255,170, 1);");
		drug_initialization->setLayout(drug_initialization_layout);

		QWidget *drug_init = new QWidget;
		drug_fields = new QFormLayout;
		drug_init->setLayout(drug_fields);

		active_substance = new QLineEdit;
		make_field(drug_fields, "Active substance", active_substance);

		drug_initialization_layout->addWidget(drug_init);

		drug_initialization->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		drug_initialization->show();
	}
}

void MainWindow::make_recipe_window()
{
	if (recipe_enabled == false)
	{
		recipe_enabled = true;
		auto recipe_window = new QWidget;
		recipe_window->setFixedSize(540, 500);
		recipe_table = new QTableWidget{0,4};
		recipe_window->setStyleSheet("background-color:rgba(255,255,170,1)");
		QBoxLayout *recipe_window_layout = new QBoxLayout(QBoxLayout::TopToBottom);
		recipe_window->setLayout(recipe_window_layout);

		recipe_table->setHorizontalHeaderLabels(QStringList({"Name", "Producer", "Active Substance", "Price"}));

		recipe_table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
		recipe_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		recipe_table->setSelectionBehavior(QAbstractItemView::SelectRows);
		recipe_table->setSelectionMode(QAbstractItemView::SingleSelection);

		QWidget *buttons = new QWidget;
		buttons->setStyleSheet("background-color:rgba(170,255,255,1)");
		QBoxLayout *buttons_layout = new QBoxLayout(QBoxLayout::LeftToRight);
		buttons->setLayout(buttons_layout);
		buttons_layout->setContentsMargins(0, 0, 0, 0);

		auto clear_button = new QPushButton("CLEAR");
		connect(clear_button, &QPushButton::clicked, [this]()
		{
			gui_service.recipe_clear();
			auto data = gui_service.get_recipe();
			update_database(data, recipe_table);
		});
		buttons_layout->addWidget(clear_button);

		auto save_button = new QPushButton("SAVE AS HTML");
		connect(save_button, &QPushButton::clicked, [this]() { gui_service.recipe_save_to_HTML("recipe.HTML"); });
		buttons_layout->addWidget(save_button);

		QPushButton *exit_button = new QPushButton("Exit", this);
		exit_button->setMinimumWidth(150);
		exit_button->setStyleSheet("background-color:rgba(242,38,19,1)");
		connect(exit_button, &QPushButton::clicked, [this, recipe_window]() { recipe_enabled = false;  recipe_window->close(); });
		buttons_layout->addWidget(exit_button);

		QWidget *generate = new QWidget;
		generate->setStyleSheet("background-color:rgba(170,255,255,1)");
		QBoxLayout *generate_layout = new QBoxLayout(QBoxLayout::LeftToRight);
		generate->setLayout(generate_layout);
		generate_layout->setContentsMargins(0, 0, 0, 0);

		auto generate_button = new QPushButton("GENERATE");
		connect(generate_button, &QPushButton::clicked, [this]()
		{
			try
			{
				gui_service.recipe_clear();
				gui_service.recipe_generate(sp->value());
				auto data = gui_service.get_recipe();
				update_database(data, recipe_table);
			}
			catch (ServiceException const &msg)
			{
				string err = msg.error_message[0];
				QPushButton *btn = new QPushButton(err.c_str());
				connect(btn, &QPushButton::clicked, [btn]() { btn->close(); });
				btn->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
				btn->show();
			}
		});

		auto recipe_add_button = new QPushButton("ADD");
		connect(recipe_add_button, &QPushButton::clicked, [this]()
		{
			auto selected_drug = database_table->selectedItems();
			if (selected_drug.size())
			{
				string Sname = selected_drug.at(0)->text().toStdString();
				string Sproducer = selected_drug.at(1)->text().toStdString();
				string Sas = selected_drug.at(2)->text().toStdString();
				int Sprice = stoi(selected_drug.at(3)->text().toStdString());
				Drug drug{Sname, Sproducer, Sas, Sprice};

				gui_service.recipe_add(drug);
				auto data = gui_service.get_recipe();
				update_database(data, recipe_table);
			}
		});

		generate_layout->addWidget(recipe_add_button);
		generate_layout->addWidget(generate_button);
		generate_layout->addWidget(createSpinner());

		recipe_window_layout->addWidget(recipe_table);
		recipe_window_layout->addWidget(buttons);
		recipe_window_layout->addWidget(generate);

		recipe_window->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		recipe_window->show();
	}
}

void MainWindow::insert_drug_row(int row, Drug drug, QTableWidget* parent)
{
	parent->setItem(row, 0, new QTableWidgetItem(drug.get_name().c_str()));
	parent->setItem(row, 1, new QTableWidgetItem(drug.get_producer().c_str()));
	parent->setItem(row, 2, new QTableWidgetItem(drug.get_active_substance().c_str()));
	parent->setItem(row, 3, new QTableWidgetItem(to_string(drug.get_price()).c_str()));
}

