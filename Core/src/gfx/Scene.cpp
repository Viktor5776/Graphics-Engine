#include "Scene.h"
#include "Camera.h"
#include "Jobber\Channels.h"
#include <memory>
#include "../misc/HydroMath.h"

namespace Hydro::gfx
{

	Scene::Scene( Graphics& gfx, Rgph::RenderGraph& rg, const std::string& path )
		:
		light( gfx )
	{
		LoadScene( gfx, path );
		
		light.LinkTechniques( rg );
		cameras.LinkTechniques( rg );
		for( auto& m : models )
		{
			m.LinkTechniques( rg );
		}
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

	void Scene::RenderSceneWindow()
	{
		//TODO
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

	}

	void Scene::LoadScene( Graphics& gfx, const std::string& path )
	{
		
		//Create stuff here to test if it works
		light.Reset( { 10.0f,5.0f,0.0f }, Channels::main );
		cameras.AddCamera( std::make_unique<Camera>( gfx, "Main", DirectX::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f ) );
		models.emplace_back( Model{ gfx, "models\\Sponza\\sponza.obj", "Sponza", Channels::main | Channels::shadow, 1.0f / 20.0f } );

		//TODO load form file format 

	}

}
