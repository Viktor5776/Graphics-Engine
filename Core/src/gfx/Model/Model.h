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
		Model( Graphics& gfx, const std::string& pathString, const std::string& name, const size_t channels = 0b1, float scale = 1.0f );
		Model( Model&& other ) noexcept;
		Model& operator=( Model&& other ) noexcept;
		Model( Model& other ) = delete;
		Model& operator=( Model& other ) = delete;
		void Submit( size_t channels ) const noexcept(!_DEBUG);
		void SetRootTransform( DirectX::FXMMATRIX tf ) noexcept;
		void Accept( class ModelProbe& probe );
		void LinkTechniques( Rgph::RenderGraph& );
		std::string GetName() noexcept;
		size_t GetChannels() noexcept;
		~Model() noexcept;
	private:
		std::unique_ptr<Node> ParseNode( int& nextId, const aiNode& node, float scale ) noexcept;
	private:
		std::string name;
		size_t channels;
		std::unique_ptr<Node> pRoot;
		//sharing meshes here perhaps dangerous?
		std::vector<std::unique_ptr<Mesh>> meshPtrs;
	};

}