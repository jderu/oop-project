#include "CosReadOnlyGUI.h"

CosReadOnlyGUI::CosReadOnlyGUI(service &_gui_service)
{
	gui_service = &_gui_service;
	setFixedSize(540, 500);
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void CosReadOnlyGUI::paintEvent(QPaintEvent * ev)
{
	QPainter p{this};
	ev = ev;
	const vector<Drug> &recipe_data = gui_service->get_recipe();

	p.setPen(Qt::blue);
	p.setFont(QFont("Arial", 30));
	p.drawText(rect(), Qt::AlignBottom | Qt::AlignRight, QString::number(recipe_data.size()));

	for (int i = 0; i < recipe_data.size(); i++)
	{
		auto img = QImage("Istvan.jpg");
		int x = rand() % (540 - img.height());
		int y = rand() % (500 - img.width());
		p.drawImage(x, y, img);
	}
}

void CosReadOnlyGUI::update()
{
	repaint();
}

void Observers::add_obs(Observer * ob)
{
	obs.push_back(ob);
	ob = ob;
}

void Observers::notifyObservers()
{
	for (auto i : obs)
		i->update();
}
