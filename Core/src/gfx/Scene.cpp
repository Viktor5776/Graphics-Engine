#include "Scene.h"
#include "Camera.h"
#include "Jobber\Channels.h"
#include <memory>
#include "../misc/HydroMath.h"
#include <nlohmann\json.hpp>
#include <fstream>
#include "../../third/ImGui/imgui.h"
#include "../misc/HydroXM.h"
#include "TestModelProbe.h"

namespace Hydro::gfx
{

	Scene::Scene( Graphics& gfx, Rgph::RenderGraph& rg, const std::string& path )
		:
		light( gfx )
	{
		LoadScene( gfx, rg, path );
	}

	void Scene::Submit( Graphics& gfx )
	{
		light.Bind( gfx, cameras->GetMatrix() );
		
		for( size_t i = 1; i <= Channels::maxChannels; i <<= 1 )
		{
			if( (light.GetChannels() & i) == i )
			{
				light.Submit( i );
			}
		
			if( (cameras.GetChannels() & i) == i )
			{
				cameras.Submit( i );
			}
		
			for( auto& m : models )
			{
				if( (m.GetChannels() & i) == i )
				{
					m.Submit( i );
				}
			}
		}
	}

	void Scene::RenderSceneWindow( Graphics& gfx, Rgph::RenderGraph& rg )
	{
		ImGui::Begin( "Scene" );

		ImGui::Begin( "Models" );
		for( auto& model : models )
		{
			ImGui::BeginChild( model.GetName().c_str() );
			static MP probe;
			probe.SpawnWindow( model );
			ImGui::EndChild();
		}
		ImGui::End();

		if( ImGui::Button( "Load" ) )
		{
			LoadScene( gfx, rg, path );
		}

		if( ImGui::Button( "Save" ) )
		{
			SaveScene( "Scenes\\scene_save.json" );
		}
		ImGui::End();
		
		//TODO modify scene
	}

	Camera& Scene::GetActiveCamera()
	{
		return cameras.GetActiveCamera();
	}

	PointLight& Scene::GetLight()
	{
		return light;
	}

	void Scene::SaveScene( const std::string& path )
	{
		//TODO SAVE SCENE BOOL TO SAVE ACTIVE CAMERA SHOLD BE ADDED
		using json = nlohmann::json;
		json sceneData;

		//Light
		DirectX::XMFLOAT3 lightPos = light.GetPos();
		sceneData["Light"]["pos"] = { lightPos.x, lightPos.y, lightPos.z };
		sceneData["Light"]["channels"] = light.GetChannels();

		//Cameras
		for( auto& cam : cameras.GetCameras() )
		{
			sceneData["Cameras"].push_back( {
				{ "name", cam->GetName() },
				{ "pos", { cam->GetPos().x, cam->GetPos().y, cam->GetPos().z } },
				{ "pitch", cam->GetPitch() },
				{ "yaw", cam->GetYaw() },
				{ "channels", cameras.GetChannels() }
			} );
		}

		//Models
		for( auto& model : models )
		{
			DirectX::XMFLOAT4X4 transform = model.GetRootTransform();

			DirectX::XMFLOAT3 rot = ExtractEulerAngles( transform );
			DirectX::XMFLOAT3 pos = ExtractTranslation( transform );
			DirectX::XMFLOAT3 scale = ExtractScale( transform );

			sceneData["Models"].push_back( {
				{ "path", model.GetPath() },
				{ "name", model.GetName() },
				{ "channels", model.GetChannels() },
				{ "scale", { scale.x, scale.y, scale.z } },
				{ "rotation", { rot.x, rot.y, rot.z }},
				{ "pos", { pos.x,pos.y,pos.z }}
			} );
		}

		//Save scene path
		std::ofstream file;
		file.open( path );
		file << sceneData.dump( 4 );
	}

	void Scene::LoadScene( Graphics& gfx, Rgph::RenderGraph& rg, const std::string& path )
	{
		this->path = path;

		cameras.clear();
		models.clear();

		using json = nlohmann::json;
		
		std::ifstream file( path );
		json sceneData = json::parse( file );
		
		//Light
		light.Reset( { sceneData["Light"]["pos"][0],sceneData["Light"]["pos"][1],sceneData["Light"]["pos"][2] }, sceneData["Light"]["channels"] );

		//Cameras
		for( auto& cam : sceneData["Cameras"] )
		{
			cameras.AddCamera( std::make_unique<Camera>( gfx, 
				cam["name"].get<std::string>(),
				DirectX::XMFLOAT3{
					cam["pos"][0].get<float>(),
					cam["pos"][1].get<float>(),
					cam["pos"][2].get<float>()
				},
				cam["pitch"].get<float>(),
				cam["yaw"].get<float>() ) );
			cameras.SetChannels( cam["channels"] );
		}

		//Models
		for( auto& model : sceneData["Models"] )
		{
			models.emplace_back( Model{ gfx, model["path"], model["name"], model["channels"], 1.0f } );
			models.back().SetRootTransform(
				DirectX::XMMatrixScaling( model["scale"][0], model["scale"][1], model["scale"][2] ) *
				DirectX::XMMatrixRotationRollPitchYaw( model["rotation"][0], model["rotation"][1], model["rotation"][2] ) *
				DirectX::XMMatrixTranslation( model["pos"][0], model["pos"][1], model["pos"][2] )
			);
		}

		//Link
		light.LinkTechniques( rg );
		cameras.LinkTechniques( rg );
		for( auto& m : models )
		{
			m.LinkTechniques( rg );
		}
	}

}
