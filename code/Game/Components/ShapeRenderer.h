#pragma once
//Base component class, used for inheritance
#include "ECS/BaseComponent.h"

//Graphics stuff
#include "Gfx/Gfx.h"
#include "Shaders/shaders.h"
#include "TransformComponent.h"

/**
	Renders a simple shape using the given draw state.
	The visibility of the shape may be toggled using the 'visible' attribute.

	@author Philip Stenmark
*/
class ShapeRenderer : public BaseComponent
{
	ECSComponentType(ShapeRenderer);
public:
	ShapeRenderer(DrawState drawState);
	void OnLoad();
	void Update();
private:
	DrawState drawState;
	ShapeShader::params shaderParams;
	Ptr<TransformComponent> transform;
	bool visible = false;
};