#pragma once
#include "ActiuneUndo.h"
class UndoModifica :public ActiuneUndo
{
private:
	repository& repo;
	Drug drug;
	int position;
public:
	UndoModifica(repository& _repo, Drug _drug, int _position)noexcept : repo(_repo), drug(_drug), position(_position) {};
	void doUndo() override
	{
		repo.set(position, drug);
	}
};

