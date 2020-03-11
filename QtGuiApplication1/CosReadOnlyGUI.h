#pragma once
#include <QtWidgets>
#include "service.h"
#include <random>

class Observer
{
public:
	virtual void update() = 0;
};

class Observers
{
private:
	vector<Observer*> obs;public:
	void add_obs(Observer* ob);
	void notifyObservers();
};

class CosReadOnlyGUI :public QWidget, public Observer
{
	Q_OBJECT

private:
	service *gui_service;
protected:
	void paintEvent(QPaintEvent* ev) override;
	void update()override;

public:
	CosReadOnlyGUI(service &_gui_service);
};

