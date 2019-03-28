#pragma once
//GFX
#include "Gfx/Gfx.h"

//Team tag component
#include "../Components/TeamTag.h"

using namespace Oryol;

class ShapeServer
{
public:
	/// get a tree model
	static DrawState GetTreeModel();

	/// get a iron ore model
	static DrawState GetIronOreModel();

	/// get a buildingsite model
	static DrawState GetBuildingSiteModel();

	/// get a castle model
	static DrawState GetCastleModel(int teamtag);

	/// get a smithy model
	static DrawState GetSmithyModel();

	/// get a kiln model
	static DrawState GetKilnModel();

	/// Get a smelter model
	static DrawState GetSmelterModel();

	/// get a training camp model
	static DrawState GetTrainingCampModel(int teamtag);

	/// get a worker model
	static DrawState GetWorkerModel(int teamtag);

	/// get a explorer model
	static DrawState GetExplorerModel(int teamtag);

	/// get at craftman model
	static DrawState GetCraftsmanModel(int teamtag);

	/// get at soldier model
	static DrawState GetSoldierModel(int teamtag);

};