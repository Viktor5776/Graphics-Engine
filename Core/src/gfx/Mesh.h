#pragma once
#include "Drawable/DrawableBase.h"
#include "Bindable/BindableCommon.h"
#include "Vertex.h"
#include <optional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hydro::gfx
{
	class Mesh : public DrawableBase<Mesh>
	{
	public:
		Mesh( Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs );
		void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const noexcept(!_DEBUG);
		DirectX::XMMATRIX GetTransformXM() const noexcept override;
	private:
		mutable DirectX::XMFLOAT4X4 transform;
	};

	class Node
	{
		friend class Model;
		friend class ModelWindow;
	public:
		Node( const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform ) noexcept(!_DEBUG);
		void Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const noexcept(!_DEBUG);
		void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept;
	private:
		void AddChild( std::unique_ptr<Node> pChild ) noexcept(!_DEBUG);
		void ShowTree( int& nodeIndex, std::optional<int>& selectedIndex, Node*& pSelectedNode ) const noexcept;
	private:
		std::string name;
		std::vector<std::unique_ptr<Node>> childPtrs;
		std::vector<Mesh*> meshPtrs;
		DirectX::XMFLOAT4X4 transform;
		DirectX::XMFLOAT4X4 appliedTransform;
	};

	class Model
	{
	public:
		Model( Graphics& gfx, const std::string fileName );
		void Draw( Graphics& gfx ) const noexcept(!_DEBUG);
		void ShowWindow( const char* windowName = nullptr ) noexcept;
		~Model() noexcept;
	private:
		static std::unique_ptr<Mesh> ParseMesh( Graphics& gfx, const aiMesh& mesh );
		std::unique_ptr<Node> ParseNode( const aiNode& node ) noexcept;
	private:
		std::unique_ptr<Node> pRoot;
		std::vector<std::unique_ptr<Mesh>> meshPtrs;
		std::unique_ptr<class ModelWindow> pWindow;
	};
}