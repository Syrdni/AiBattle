#include "ShapeServer.h"

//GFX
#include "Assets/Gfx/ShapeBuilder.h"
#include "glm/gtx/transform.hpp"
#include "Shaders/shaders.h"

//Other
#include <cstdlib>
#include <time.h>

DrawState ShapeServer::GetTreeModel()
{
	static DrawState drawState;

	if (drawState.Pipeline.IsValid())
	{
		return drawState;
	}

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	shapeBuilder
		.Color(glm::vec4(0.44f, 0.22f, 0.11f, 1.0f))
		.Cylinder(0.1f, 1.0f, 4, 1, false)
		.Color(glm::vec4(0.0f, 0.4f, 0.0f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, 0.2f, 0)))
		.Box(0.5f, 0.8f, 0.5f, 1);

	drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}
	drawState.Pipeline = Gfx::CreateResource(ps);
	return drawState;
}

DrawState ShapeServer::GetIronOreModel()
{
	static DrawState drawState;
	
	if (drawState.Pipeline.IsValid())
	{
		return drawState;
	}

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	shapeBuilder
		.Color(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
		.Box(0.8f, 0.8f, 0.8f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, -0.3f, 0.0f)))
		.Box(1.0f, 0.4f, 1.0f, 1);

	drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}
	drawState.Pipeline = Gfx::CreateResource(ps);
	return drawState;
}

DrawState ShapeServer::GetCastleModel(int teamtag)
{
	static DrawState drawStateTeam1;
	static DrawState drawStateTeam2;

	if (drawStateTeam1.Pipeline.IsValid() && teamtag == 1)
		return drawStateTeam1;
	else if (drawStateTeam2.Pipeline.IsValid() && teamtag == 2)
		return drawStateTeam2;

	glm::vec4 color;

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	if (teamtag == 1)
		color = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	else
		color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);
	shapeBuilder
		//Base
		.Color(glm::vec4(0.658f, 0.658f, 0.658f, 1.0f))
		.Box(2.0f, 1.5f, 2.0f, 1, false)

		//Tornen
		.Transform(glm::translate(glm::vec3(0.9f, 0.4f, 0.9f)))
		.Box(0.5f, 2.0f, 0.5f, 1, false)
		.Transform(glm::translate(glm::vec3(0.9f, 0.4f, -0.9f)))
		.Box(0.5f, 2.0f, 0.5f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.9f, 0.4f, -0.9f)))
		.Box(0.5f, 2.0f, 0.5f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.9f, 0.4f, 0.9f)))
		.Box(0.5f, 2.0f, 0.5f, 1, false)

		//Corner pieces
		.Transform(glm::translate(glm::vec3(1.10f, 1.4f, 1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(1.10f, 1.4f, 0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(0.7f, 1.4f, 0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(0.7f, 1.4f, 1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)



		.Transform(glm::translate(glm::vec3(1.10f, 1.4f, -1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(1.10f, 1.4f, -0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(0.7f, 1.4f, -0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(0.7f, 1.4f, -1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)


		.Transform(glm::translate(glm::vec3(-1.10f, 1.4f, -1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(-1.10f, 1.4f, -0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.7f, 1.4f, -0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.7f, 1.4f, -1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)


		.Transform(glm::translate(glm::vec3(-1.10f, 1.4f, 1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(-1.10f, 1.4f, 0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.7f, 1.4f, 0.7f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.7f, 1.4f, 1.10f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)

		//Flag
		.Transform(glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)))
		.Color(glm::vec4(0.349, 0.349, 0.349, 1.0f))
		.Cylinder(0.1f, 2.5f, 40, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 1.8f, -0.6f)))
		.Color(color)
		.Box(0.1f, 0.8f, 1.2f, 1, false)

		//Bridge
		.Color(glm::vec4(0.498f, 0.376f, 0.223f, 1))
		.Transform(glm::translate(glm::vec3(1.2f, -0.4f, 0.0f)) * glm::rotate(glm::degrees(45.0f), glm::vec3(0, 0, 1)))
		.Box(0.1f, 1.2f, 0.8f, 1, false)

		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Transform(glm::translate(glm::vec3(0.951, -0.2f, 0.0f)))
		.Box(0.1f, 1.2f, 0.8f, 1, false)

		.Color(glm::vec4(0.627f, 0.650f, 0.650f ,1))
		.Transform(glm::translate(glm::vec3(1.2f, 0.25f, 0.35f)) * glm::rotate(glm::degrees(-45.0f), glm::vec3(0, 0, 1)))
		.Box(0.03f, 1.0, 0.03f, 1, false)

		.Transform(glm::translate(glm::vec3(1.2f, 0.25f, -0.35f)) * glm::rotate(glm::degrees(-45.0f), glm::vec3(0, 0, 1)))
		.Box(0.03f, 1.0, 0.03f, 1, false)

		//Window
		.Transform(glm::translate(glm::vec3(1.101f, 1.13f, 0.9f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)
		.Transform(glm::translate(glm::vec3(1.101f, 1.13f, -0.9f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)

		.Transform(glm::translate(glm::vec3(0.9, 1.13f, 1.101f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)
		.Transform(glm::translate(glm::vec3(0.9, 1.13f, -1.101f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)

		.Transform(glm::translate(glm::vec3(-0.9, 1.13f, 1.101f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)
		.Transform(glm::translate(glm::vec3(-0.9, 1.13f, -1.101f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)



		.Transform(glm::translate(glm::vec3(-1.101f, 1.13f, -0.9f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1, false)
		.Transform(glm::translate(glm::vec3(-1.101f, 1.13f, 0.9f)))
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Box(0.1f, 0.2f, 0.1f, 1);


	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}

	if (teamtag == 1)
	{
		drawStateTeam1.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam1.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam1;
	}
	else
	{
		drawStateTeam2.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam2.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam2;
	}
	
}

DrawState ShapeServer::GetBuildingSiteModel()
{
	static DrawState drawState;

	if (drawState.Pipeline.IsValid())
	{
		return drawState;
	}

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	shapeBuilder
		// pole
		.Color(glm::vec4(0.5f, 0.3f, 0.1f, 1.0f))
		.Transform(glm::translate(glm::vec3(-0.45f, 0, -0.45f)))
		.Cylinder(0.075f, 1.2f, 6, 3, false)
		// cross pole
		.Transform(glm::translate(glm::vec3(-0.2f, 0.5f, -0.2f)) * glm::rotate(glm::radians(90.0f), glm::vec3(-1,0,1)))
		.Cylinder(0.075f, 0.9, 6, 3, false)
		// stabelizing pole
		.Transform(glm::translate(glm::vec3(-0.35f, 0.35f, -0.35f)) * glm::rotate(glm::radians(-45.0f), glm::vec3(-1,0,1)))
		.Cylinder(0.02f, 0.75, 6, 4, false)
		// rope
		.Color(glm::vec4(0.1f, 0.1f ,0.1f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, 0.3f, 0.0f)))
		.Cylinder(0.01f, 0.6, 6, 2, false)
		// haning box
		.Color(glm::vec4(0.6f, 0.3f, 0.1f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)))
		.Box(0.3f, 0.3f, 0.3f, 1, false)
		// big box
		.Transform(glm::translate(glm::vec3(-0.35f, -0.35f, 0.35f)))
		.Box(0.3f, 0.3f, 0.3f, 1, false)
		// small box
		.Transform(glm::translate(glm::vec3(0.35f, -0.425f, -0.35f)) * glm::rotate(glm::radians(45.0f), glm::vec3(0,1,0)) * glm::rotate(glm::radians(90.0f), glm::vec3(1,0,0)))
		.Cylinder(0.075f, 1.0f, 6, 3, false)
		// log
		.Color(glm::vec4(0.45f, 0.2f, 0.05f, 1.0f))
		.Transform(glm::translate(glm::vec3(-0.42f, -0.125f, 0.42f)))
		.Box(0.15f, 0.15f, 0.15f, 1, false)
		// base plate
		.Color(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, -0.5f, 0)))
		.Box(1.0f, 0.01f, 1.0f, 1, true);

	drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}
	drawState.Pipeline = Gfx::CreateResource(ps);
	return drawState;
}

DrawState ShapeServer::GetSmithyModel()
{
	srand(time(0));
	static DrawState drawState;

	if (drawState.Pipeline.IsValid())
		return drawState;

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);

	shapeBuilder
		// poles
		.Color(glm::vec4(0.5f, 0.3f, 0.1f, 1.0f))
		.Transform(glm::translate(glm::vec3(-0.35f, -0.3f, -0.35f)))
		.Cylinder(0.075f, 0.45f, 6, 3, false)
		.Transform(glm::translate(glm::vec3(0.35f, -0.3f, -0.35f)))
		.Cylinder(0.075f, 0.45f, 6, 3, false)
		.Transform(glm::translate(glm::vec3(0.35f, -0.05, 0.35f)))
		.Cylinder(0.075f, 0.90f, 6, 3, false)
		.Transform(glm::translate(glm::vec3(-0.35f, -0.05, 0.35f)))
		.Cylinder(0.075f, 0.90f, 6, 3, false)

		// roof
		.Color(glm::vec4(0.4f, 0.2f, 0.1f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, 0.20f, 0.0f)) * glm::rotate(glm::radians(-35.0f), glm::vec3(1, 0, 0)))
		.Box(1.2f, 0.05f, 1.5f, 1, false)

		// anvil
		.Color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, -0.5f, 0.35f)))
		.Box(0.2f, 0.075f, 0.15f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, -0.425f, 0.35f)))
		.Box(0.15f, 0.10f, 0.1f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, -0.34f, 0.35f)))
		.Box(0.25f, 0.075f, 0.15f, 1, false)
		.Transform(glm::translate(glm::vec3(0.12f, -0.34f, 0.35f)) * glm::rotate(glm::radians(45.0f), glm::vec3(0, 1, 0)))
		.Box(0.11f, 0.075f, 0.11f, 1, false)

		// small burner

		.Color(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.5f, -0.4f, 0.0f)))
		.Cylinder(0.3f, 0.2f, 6, 1, false)
		.Color(glm::vec4(0.960f, 0.529f, 0.160f, 1.0f))
		.Cylinder(0.2f, 0.201f, 10, 1, false)
		.Color(glm::vec4(0.952f, 0.243f, 0.105f, 1.0f))

		// coal (alot...)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)
		.Transform(glm::translate(glm::vec3(0.35 + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f), -0.3f, -0.1f + static_cast <float> (rand()) / static_cast <float>(RAND_MAX / 0.2f))))
		.Box(0.02f, 0.02f, 0.02f, 1, false)

		// base plate
		.Color(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
			.Transform(glm::translate(glm::vec3(0, -0.5f, 0)))
			.Box(1.0f, 0.01f, 1.0f, 1, true);



	drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}
	drawState.Pipeline = Gfx::CreateResource(ps);
	return drawState;
}

DrawState ShapeServer::GetKilnModel()
{
	static DrawState drawState;

	if (drawState.Pipeline.IsValid())
	{
		return drawState;
	}

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	shapeBuilder
		//Base
		.Color(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, -0.46f, 0.0f)))
		.Box(1.0f, 0.01f, 1.0f, 1, false)

		//Kiln Base
		.Transform(glm::translate(glm::vec3(0.0f, -0.2f, 0.0f)))
		.Color(glm::vec4(0.658f, 0.658f, 0.658f, 1))
		.Cylinder(0.25f, 0.5, 40, 1, false)

		//Kiln Entrance
		.Transform(glm::translate(glm::vec3(0.2f, -0.35f, 0.0f)))
		.Box(0.25f, 0.25f, 0.25f, 1, false)

		//Kiln Entrance black
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Transform(glm::translate(glm::vec3(0.28f, -0.35f, 0.0f)))
		.Box(0.1f, 0.1f, 0.1f, 1, false)

		//Chimney
		.Color(glm::vec4(0.658f, 0.658f, 0.658f, 1))
		.Transform(glm::translate(glm::vec3(-0.15f, 0.25f, -0.12f)))
		.Cylinder(0.04f, 0.4, 40, 1, false)

		//Smoke
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Transform(glm::translate(glm::vec3(-0.15f, 0.49f, -0.12f)))
		.Sphere(0.04, 40, 40, false)

		.Transform(glm::translate(glm::vec3(-0.15f, 0.53f, -0.09f)))
		.Sphere(0.04, 40, 40, false)

		.Transform(glm::translate(glm::vec3(-0.15f, 0.57f, -0.06f)))
		.Sphere(0.04, 40, 40);



	drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}
	drawState.Pipeline = Gfx::CreateResource(ps);
	return drawState;

}

DrawState ShapeServer::GetSmelterModel()
{
	static DrawState drawState;

	if (drawState.Pipeline.IsValid())
	{
		return drawState;
	}

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	shapeBuilder
		//Base
		.Color(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, -0.46f, 0.0f)))
		.Box(1.0f, 0.01f, 1.0f, 1, false)

		//SmelterBase
		.Transform(glm::translate(glm::vec3(-0.1f, -0.2f, 0.0f)))
		.Color(glm::vec4(0.658f, 0.658f, 0.658f, 1))
		.Cylinder(0.25f, 0.5, 40, 1, false)

		//Smelter drain
		.Transform(glm::translate(glm::vec3(0.2f, -0.1f, 0.0f)))
		.Box(0.2f, 0.05f, 0.05f, 1, false)

		.Transform(glm::translate(glm::vec3(0.275f, -0.15f, 0.0f)))
		.Box(0.05f, 0.1f, 0.05f, 1, false)

		//Drain collector
		.Transform(glm::translate(glm::vec3(0.275f, -0.4f, 0.0f)))
		.Box(0.2f, 0.125f, 0.4f, 1, false)

		//Lava
		.Color(glm::vec4(0.8f, 0.223f, 0.0f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.275f, -0.3f, 0.0f)))
		.Box(0.05f, 0.2f, 0.05f, 1, false)

		.Transform(glm::translate(glm::vec3(0.275f, -0.3999f, 0.0f)))
		.Box(0.125f, 0.125f, 0.325f, 1, false)

		//Chimney
		.Color(glm::vec4(0.658f, 0.658f, 0.658f, 1))
		.Transform(glm::translate(glm::vec3(-0.15f, 0.25f, -0.12f)))
		.Cylinder(0.04f, 0.4, 40, 1, false)

		//Smoke
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1))
		.Transform(glm::translate(glm::vec3(-0.15f, 0.49f, -0.12f)))
		.Sphere(0.04, 40, 40, false)

		.Transform(glm::translate(glm::vec3(-0.15f, 0.53f, -0.09f)))
		.Sphere(0.04, 40, 40, false)

		.Transform(glm::translate(glm::vec3(-0.15f, 0.57f, -0.06f)))
		.Sphere(0.04, 40, 40);

	drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}
	drawState.Pipeline = Gfx::CreateResource(ps);
	return drawState;

}

DrawState ShapeServer::GetTrainingCampModel(int teamtag)
{
	static DrawState drawStateTeam1;
	static DrawState drawStateTeam2;

	if (drawStateTeam1.Pipeline.IsValid() && teamtag == 1)
		return drawStateTeam1;
	else if (drawStateTeam2.Pipeline.IsValid() && teamtag == 2)
		return drawStateTeam2;

	glm::vec4 color;
	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);

	if (teamtag == 1)
		color = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	else
		color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);

	shapeBuilder
		// poles
		.Color(glm::vec4(0.364f, 0.274f, 0.078f, 1.0f))
		.Transform(glm::translate(glm::vec3(-0.35f, -0.2f, -0.35f)) * glm::rotate(glm::radians(45.0f), glm::vec3(1,0,0)))
		.Cylinder(0.02f, 1.20f, 10, 3, false)
		.Transform(glm::translate(glm::vec3(0.35f, -0.2f, -0.35f)) * glm::rotate(glm::radians(45.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.02f, 1.20f, 10, 3, false)
		.Transform(glm::translate(glm::vec3(0.35f, -0.2f, 0.35f)) * glm::rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.02f, 1.20f, 10, 3, false)
		.Transform(glm::translate(glm::vec3(-0.35f, -0.2f, 0.35f)) * glm::rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.02f, 1.20f, 10, 3, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.21f, 0.0f)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1)))
		.Cylinder(0.02f, 1.0f, 10, 3, false)

		// tarp
		.Color(glm::vec4(color))
		.Transform(glm::translate(glm::vec3(0.0f, -0.23f, 0.35f)) * glm::rotate(glm::radians(45.0f), glm::vec3(1,0,0)))
		.Box(0.85f, 0.01f, 1.0f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, -0.23f, -0.35f)) * glm::rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0)))
		.Box(0.85f, 0.01f, 1.0f, 1, false)

		// base plate
		.Color(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, -0.5f, 0)))
		.Box(1.0f, 0.01f, 1.0f, 1, true);


	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}

	if (teamtag == 1)
	{
		drawStateTeam1.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam1.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam1;
	}
	else
	{
		drawStateTeam2.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam2.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam2;
	}
}

DrawState ShapeServer::GetExplorerModel(int teamtag)
{
	static DrawState drawStateTeam1;
	static DrawState drawStateTeam2;

	if (drawStateTeam1.Pipeline.IsValid() && teamtag == 1)
		return drawStateTeam1;
	else if (drawStateTeam2.Pipeline.IsValid() && teamtag == 2)
		return drawStateTeam2;

	glm::vec4 color;

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);

	if (teamtag == 1)
		color = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	else
		color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);

	shapeBuilder
		// shirt
		.Color(color)
		.Transform(glm::translate(glm::vec3(0, -0.2f, 0)))
		.Cylinder(0.1f, 0.6f, 8, 1, false)
		// arm 1
		.Transform(glm::translate(glm::vec3(0.075f, -0.2f, -0.175f)) * glm::rotate(glm::radians(80.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.02f, 0.3f, 8, 1, false)
		// arm 2
		.Transform(glm::translate(glm::vec3(-0.125f, -0.2f, 0.0f)))
		.Cylinder(0.03f, 0.25f, 8, 1, false)
		// map
		.Color(glm::vec4(0.909f, 0.760f, 0.427f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, -0.15f, -0.4f)) * glm::rotate(glm::radians(20.0f), glm::vec3(1, 0, 0)))
		.Box(0.3f, 0.01f, 0.25f, 1, false)
		// hat
		.Color(glm::vec4(0.231f, 0.231f, 0.227f, 1.0f))
		// brim
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(89.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(88.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(87.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(86.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(85.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(84.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(83.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(82.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(81.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(80.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(79.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(78.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(77.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(76.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(75.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(74.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(73.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(72.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(71.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(70.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(69.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(68.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(67.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(66.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(65.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(64.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(63.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(62.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(61.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(60.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(59.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(58.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(57.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(56.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(55.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(54.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(53.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(52.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(51.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(50.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(49.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(48.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(47.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(46.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(45.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(44.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(43.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(42.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(41.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(40.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(39.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(38.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(37.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(36.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(35.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(34.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(33.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(32.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(31.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(30.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(29.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(28.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(27.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(26.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(25.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(24.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(23.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(22.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(21.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(20.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(19.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(18.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(17.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(16.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(15.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(14.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(13.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(12.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(11.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(10.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(9.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(8.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(7.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(6.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(5.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(4.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(3.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(2.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(1.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)													
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0)))
		.Box(0.4f, 0.02f, 0.4f, 1, false)
		// top part
		.Transform(glm::translate(glm::vec3(0.0f, 0.18f, 0.04f)) * glm::rotate(glm::radians(25.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.168f, 0.16f, 20, 3, false)
		// band
		.Color(glm::vec4(0.529f, 0.392f, 0.090f, 1.0f))
		.Cylinder(0.17f, 0.03f, 20, 3, false)
		// head
		.Color(glm::vec4(0.945f, 0.650f, 0.231f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, 0.1f, 0)))
		.Sphere(0.165f, 100, 100);

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}

	if (teamtag == 1)
	{
		drawStateTeam1.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam1.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam1;
	}
	else
	{
		drawStateTeam2.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam2.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam2;
	}
}

DrawState ShapeServer::GetWorkerModel(int teamtag)
{
	static DrawState drawStateTeam1;
	static DrawState drawStateTeam2;

	if (drawStateTeam1.Pipeline.IsValid() && teamtag == 1)
		return drawStateTeam1;
	else if (drawStateTeam2.Pipeline.IsValid() && teamtag == 2)
		return drawStateTeam2;

	glm::vec4 color;

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);
	
	if (teamtag == 1)
		color = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	else
		color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);

	shapeBuilder
		// shirt
		.Color(color)
		.Transform(glm::translate(glm::vec3(0, -0.2f, 0)))
		.Cylinder(0.1f, 0.6f, 8, 1, false)
		// arm 1
		.Transform(glm::translate(glm::vec3(0.075f, -0.2f, -0.175f)) * glm::rotate(glm::radians(80.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.02f, 0.3f, 8, 1, false)
		// arm 2
		.Transform(glm::translate(glm::vec3(-0.125f, -0.2f, 0.0f)))
		.Cylinder(0.03f, 0.25f, 8, 1, false)
		// helmet
		.Color(glm::vec4(0.960f, 0.956f, 0.160f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)))
		.Sphere(0.165f, 100, 100, false)
		// pickaxe handel
		.Color(glm::vec4(0.674f, 0.407f, 0.023f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.075f, -0.08f, -0.35f)) * glm::rotate(glm::radians(-20.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)))
		.Cylinder(0.02f, 0.3f, 6, 2, false)
		// pickaxe head
		.Color(glm::vec4(0.631f, 0.631f, 0.631f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.075f, 0.05f, -0.3f)))
		.Box(0.02f, 0.04f, 0.2f, 1, false)
		.Transform(glm::translate(glm::vec3(0.075f, -0.024f, -0.46f)) * glm::rotate(glm::radians(-50.0f), glm::vec3(1, 0, 0)))
		.Box(0.02f, 0.04f, 0.2f, 1, false)
		// head
		.Color(glm::vec4(0.945f, 0.650f, 0.231f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, 0.1f, 0)))
		.Sphere(0.165f, 100, 100);

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}

	if (teamtag == 1)
	{
		drawStateTeam1.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam1.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam1;
	}
	else
	{
		drawStateTeam2.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam2.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam2;
	}
}

DrawState ShapeServer::GetCraftsmanModel(int teamtag)
{
	static DrawState drawStateTeam1;
	static DrawState drawStateTeam2;

	if (drawStateTeam1.Pipeline.IsValid() && teamtag == 1)
		return drawStateTeam1;
	else if (drawStateTeam2.Pipeline.IsValid() && teamtag == 2)
		return drawStateTeam2;

	glm::vec4 color;

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);

	if (teamtag == 1)
		color = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	else
		color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);

	shapeBuilder
		// shirt
		.Color(color)
		.Transform(glm::translate(glm::vec3(0, -0.2f, 0)))
		.Cylinder(0.1f, 0.6f, 8, 1, false)
		// arm 1
		.Transform(glm::translate(glm::vec3(0.075f, -0.2f, -0.175f)) * glm::rotate(glm::radians(80.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.02f, 0.3f, 8, 1, false)
		// arm 2
		.Transform(glm::translate(glm::vec3(-0.125f, -0.2f, 0.0f)))
		.Cylinder(0.03f, 0.25f, 8, 1, false)
		// hair
		.Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)))
		.Sphere(0.165f, 100, 100, false)
		// hammer handel
		.Color(glm::vec4(0.631, 0.631, 0.631, 1.0f))
		.Transform(glm::translate(glm::vec3(0.075f, -0.08f, -0.35f)) * glm::rotate(glm::radians(-20.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)))
		.Cylinder(0.02f, 0.3f, 6, 2, false)
		// hammer head
		.Color(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.075f, 0.03f, -0.39f)) * glm::rotate(glm::radians(-20.0f), glm::vec3(1, 0, 0)))
		.Box(0.1f, 0.1f, 0.25f, 1, false)
		// head
		.Color(glm::vec4(0.945f, 0.650f, 0.231f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, 0.1f, 0)))
		.Sphere(0.165f, 100, 100);

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}

	if (teamtag == 1)
	{
		drawStateTeam1.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam1.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam1;
	}
	else
	{
		drawStateTeam2.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam2.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam2;
	}
}

DrawState ShapeServer::GetSoldierModel(int teamtag)
{
	static DrawState drawStateTeam1;
	static DrawState drawStateTeam2;

	if (drawStateTeam1.Pipeline.IsValid() && teamtag == 1)
		return drawStateTeam1;
	else if (drawStateTeam2.Pipeline.IsValid() && teamtag == 2)
		return drawStateTeam2;

	glm::vec4 color;

	ShapeBuilder shapeBuilder;
	shapeBuilder.Layout
		.Add(VertexAttr::Position, VertexFormat::Float3)
		.Add(VertexAttr::Color0, VertexFormat::UByte4N)
		.Add(VertexAttr::Normal, VertexFormat::Float3);

	if (teamtag == 1)
		color = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
	else
		color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);

	shapeBuilder
		// shirt
		.Color(color)
		.Transform(glm::translate(glm::vec3(0, -0.2f, 0)))
		.Cylinder(0.1f, 0.6f, 8, 1, false)
		// arm 1
		.Transform(glm::translate(glm::vec3(0.075f, -0.2f, -0.175f)) * glm::rotate(glm::radians(80.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::radians(90.0f), glm::vec3(1,0,0)))
		.Cylinder(0.03f, 0.3f, 8, 1, false)
		// arm 2
		.Transform(glm::translate(glm::vec3(-0.075f, -0.2f, -0.126f)) * glm::rotate(glm::radians(80.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)))
		.Cylinder(0.03f, 0.25f, 8, 1, false)
		// helmet
		.Color(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
		.Transform(glm::translate(glm::vec3(0.0f, 0.11f, 0.01f)))
		.Sphere(0.165f, 100, 100, false)
		// sword
		.Transform(glm::translate(glm::vec3(0.075f, 0.0f, -0.35f)) * glm::rotate(glm::radians(-20.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)))
		.Box(0.03f, 0.4f, 0.003, 1, false)
		// shield
		.Transform(glm::translate(glm::vec3(-0.090f, -0.15f, -0.25f)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::radians(-45.0f), glm::vec3(0, 0, 1)))
		.Cylinder(0.2f, 0.02f, 10, 1, false)
		// head
		.Color(glm::vec4(0.945f, 0.650f, 0.231f, 1.0f))
		.Transform(glm::translate(glm::vec3(0, 0.1f, 0)))
		.Sphere(0.165f, 100, 100);

	Id shader = Gfx::CreateResource(ShapeShader::Setup());
	auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shader);
	{
		ps.DepthStencilState.DepthWriteEnabled = true;
		ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
		ps.RasterizerState.CullFaceEnabled = true;
	}

	if (teamtag == 1)
	{
		drawStateTeam1.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam1.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam1;
	}
	else
	{
		drawStateTeam2.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
		drawStateTeam2.Pipeline = Gfx::CreateResource(ps);
		return drawStateTeam2;
	}
}