#pragma once
#include "../Graphics.h"
#include <string>
#include <memory>
#include <filesystem>

struct aiMesh;
struct aiMaterial;
struct aiNode;

namespace Hydro::gfx
{
	class Node;
	class Mesh;
	class ModelWindow;

	namespace Rgph
	{
		class RenderGraph;
	}

	class Model
	{
	public:
		Model( Graphics& gfx, const std::string& pathString, float scale = 1.0f );
		void Submit( size_t channels ) const noexcept(!_DEBUG);
		void SetRootTransform( DirectX::FXMMATRIX tf ) noexcept;
		void Accept( class ModelProbe& probe );
		void LinkTechniques( Rgph::RenderGraph& );
		~Model() noexcept;
	private:
		std::unique_ptr<Node> ParseNode( int& nextId, const aiNode& node, float scale ) noexcept;
	private:
		std::unique_ptr<Node> pRoot;
		//sharing meshes here perhaps dangerous?
		std::vector<std::unique_ptr<Mesh>> meshPtrs;
	};

}