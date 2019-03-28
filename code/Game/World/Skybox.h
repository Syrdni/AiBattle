#pragma once
#include "ECS/BaseComponent.h"
#include "Gfx/Gfx.h"
#include "Assets/Gfx/TextureLoader.h"
#include "Assets/Gfx/ShapeBuilder.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Shaders/shaders.h"
#include "Game/Game.h"

class Skybox : public BaseComponent
{
public:
	inline void OnLoad()
	{
		Id shader = Gfx::CreateResource(SkyboxShader::Setup());

		TextureSetup setup;
		setup.Sampler.MinFilter = TextureFilterMode::LinearMipmapLinear;
		setup.Sampler.MagFilter = TextureFilterMode::Linear;
		setup.Sampler.WrapU = TextureWrapMode::ClampToEdge;
		setup.Sampler.WrapV = TextureWrapMode::ClampToEdge;

		this->drawState.FSTexture[SkyboxShader::tex] = Gfx::LoadResource(
			TextureLoader::Create(TextureSetup::FromFile("cwd:skybox.dds", setup))
		);

		glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(50 * Level::TileSize, 0.0f, 50 * Level::TileSize));
		glm::mat4 rotate = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(512.0f));
		
		ShapeBuilder shapeBuilder;
		shapeBuilder.Layout = {
			{ VertexAttr::Position, VertexFormat::Float3 },
			{ VertexAttr::Normal, VertexFormat::Float3 }
		};
		shapeBuilder.Transform(translate * rotate * scale).Sphere(1.0f, 36, 20);
		this->drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

		auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
		ps.DepthStencilState.DepthWriteEnabled = false;
		ps.RasterizerState.CullFaceEnabled = true;
		ps.RasterizerState.CullFace = Face::Front;
		this->drawState.Pipeline = Gfx::CreateResource(ps);
	}

	inline void Update()
	{
		const Id& tex = this->drawState.FSTexture[SkyboxShader::tex];
		if (Gfx::QueryResourceInfo(tex).State == ResourceState::Valid)
		{
			this->params.mvp = Game::GetObserver();
			Gfx::ApplyDrawState(this->drawState);
			Gfx::ApplyUniformBlock(this->params);
			Gfx::Draw();
		}
	}
private:
	friend class Level;
	SkyboxShader::vsParams params;
	DrawState drawState;
};