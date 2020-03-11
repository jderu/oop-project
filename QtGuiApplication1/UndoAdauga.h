#pragma once
#include "ActiuneUndo.h"
class UndoAdauga :public ActiuneUndo
{
private:
	Drug drug;
	repository& repo;
public:
	UndoAdauga(repository& _repo, Drug _drug) : repo(_repo), drug(_drug) {};
	void doUndo() override
	{
		vector < Drug > data = repo.getAll();
		const int position = (int)distance(data.begin(), find(data.begin(), data.end(), drug));
		repo.remove(position);
	}
};

