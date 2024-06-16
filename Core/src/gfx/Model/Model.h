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
	class FrameCommander;
	class ModelWindow;

	class Model
	{
	public:
		Model( Graphics& gfx, const std::string& pathString, float scale = 1.0f );
		void Submit( FrameCommander& frame ) const noexcept(!_DEBUG);
		void SetRootTransform( DirectX::FXMMATRIX tf ) noexcept;
		void Accept( class ModelProbe& probe );
		~Model() noexcept;
	private:
		std::unique_ptr<Node> ParseNode( int& nextId, const aiNode& node, float scale ) noexcept;
	private:
		std::unique_ptr<Node> pRoot;
		//sharing meshes here perhaps dangerous?
		std::vector<std::unique_ptr<Mesh>> meshPtrs;
		//std::unique_ptr<ModelWindow> pWindow;
	};

}