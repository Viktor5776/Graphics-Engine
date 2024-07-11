#pragma once
#include <vector>
#include <memory>

namespace Hydro::gfx
{
	class Camera;
	class Graphics;
	namespace Rgph
	{
		class RenderGraph;
	}

	class CameraContainer
	{
	public:
		void SpawnWindow( Graphics& gfx );
		void Bind( Graphics& gfx );
		void AddCamera( std::shared_ptr<Camera> pCam );
		Camera* operator->();
		~CameraContainer();
		void LinkTechniques( Rgph::RenderGraph& rg );
		void Submit( size_t channels ) const;
		Camera& GetActiveCamera();
		void SetChannels( size_t channels );
		void clear();
		std::vector<std::shared_ptr<Camera>> GetCameras();
		size_t GetChannels();
	private:
		Camera& GetControlledCamera();
	private:
		size_t channels = 0;
		std::vector<std::shared_ptr<Camera>> cameras;
		int active = 0;
		int controlled = 0;
	};
}