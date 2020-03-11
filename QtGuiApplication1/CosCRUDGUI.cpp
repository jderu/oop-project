#include "CosCRUDGUI.h"

void CosCRUDGUI::update_database(vector<Drug> sdata, QTableWidget * parent)
{
	int row = 0;
	parent->setRowCount((int)sdata.size());
	for (auto drug : sdata)
	{
		insert_drug_row(row, drug, parent);
		row++;
	}
}

void CosCRUDGUI::insert_drug_row(int row, Drug drug, QTableWidget * parent)
{
	parent->setItem(row, 0, new QTableWidgetItem(drug.get_name().c_str()));
	parent->setItem(row, 1, new QTableWidgetItem(drug.get_producer().c_str()));
	parent->setItem(row, 2, new QTableWidgetItem(drug.get_active_substance().c_str()));
	parent->setItem(row, 3, new QTableWidgetItem(to_string(drug.get_price()).c_str()));
}

QWidget * CosCRUDGUI::createSpinner(QSpinBox * sp)
{
	QWidget* temp = new QWidget;
	QHBoxLayout* temp_layout = new QHBoxLayout;
	temp->setLayout(temp_layout);

	sp->setMaximum(25);
	sp->setMinimum(0);
	temp_layout->addWidget(sp);
	QSlider *sl = new QSlider(Qt::Horizontal);
	sl->setMaximum(25);
	sl->setMinimum(0);
	temp_layout->addWidget(sl);
	QObject::connect(sp, qOverload<int>(&QSpinBox::valueChanged), [sl](int val) { sl->setValue(val); });
	QObject::connect(sl, &QSlider::valueChanged, [sp](int val) { sp->setValue(val); });
	return temp;
}

CosCRUDGUI::CosCRUDGUI(service &_gui_service, Observers* obs)
{
	gui_service = &_gui_service;

	setFixedSize(540, 500);
	recipe_table = new QTableWidget{0,4};
	setStyleSheet("background-color:rgba(255,255,170,1)");
	QBoxLayout *recipe_window_layout = new QBoxLayout(QBoxLayout::TopToBottom);
	setLayout(recipe_window_layout);

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
		gui_service->recipe_clear();
		auto data = gui_service->get_recipe();
		update_database(data, recipe_table);
	});
	buttons_layout->addWidget(clear_button);

	QPushButton *exit_button = new QPushButton("Exit", this);
	exit_button->setMinimumWidth(150);
	exit_button->setStyleSheet("background-color:rgba(242,38,19,1)");
	connect(exit_button, &QPushButton::clicked, [this]() { close(); });
	buttons_layout->addWidget(exit_button);

	QWidget *generate = new QWidget;
	generate->setStyleSheet("background-color:rgba(170,255,255,1)");
	QBoxLayout *generate_layout = new QBoxLayout(QBoxLayout::LeftToRight);
	generate->setLayout(generate_layout);
	generate_layout->setContentsMargins(0, 0, 0, 0);

	auto sp = new QSpinBox();
	QWidget* temp = createSpinner(sp);

	auto generate_button = new QPushButton("GENERATE");
	connect(generate_button, &QPushButton::clicked, [this, sp, obs]()
	{
		try
		{
			gui_service->recipe_clear();
			gui_service->recipe_generate(sp->value());
			auto data = gui_service->get_recipe();
			update_database(data, recipe_table);
			obs->notifyObservers();
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

	generate_layout->addWidget(generate_button);
	generate_layout->addWidget(temp);

	recipe_window_layout->addWidget(recipe_table);
	recipe_window_layout->addWidget(buttons);
	recipe_window_layout->addWidget(generate);

	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void CosCRUDGUI::update()
{
	auto d = gui_service->get_recipe();
	update_database(d, recipe_table);
}
