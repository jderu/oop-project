#pragma once
#include "ActiuneUndo.h"
class UndoSterge :public ActiuneUndo
{
private:
	repository& repo;
	Drug drug;
	int position;
public:
	UndoSterge(repository& _repo, Drug _drug, int _position)noexcept : repo(_repo), drug(_drug), position(_position) {};
	void doUndo() override
	{
		repo.insert(position, drug);
	}
};

