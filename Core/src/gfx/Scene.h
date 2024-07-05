#pragma once
#include <vector>
#include <optional>
#include "PointLight.h"
#include "Model\Model.h"
#include "CameraContainer.h"
#include "Jobber\RenderGraph.h"
#include "Graphics.h"

namespace Hydro::gfx
{

	class Scene
	{
	public:
		Scene( Graphics& gfx, Rgph::RenderGraph& rg, const std::string& path );
		void Submit( Graphics& gfx );
		void RenderSceneWindow( Graphics& gfx, Rgph::RenderGraph& rg );
		Camera& GetActiveCamera();
		PointLight& GetLight();
	private:
		void SaveScene( const std::string& path );
		void LoadScene( Graphics& gfx, Rgph::RenderGraph& rg, const std::string& path );
	private:
		std::string path;

		PointLight light;
		CameraContainer cameras;
		std::vector<Model> models;
	};
}