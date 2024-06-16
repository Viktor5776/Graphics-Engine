#pragma once
#include "../Graphics.h"
#include "../Bindable/BindableCommon.h"
#include <vector>
#include <filesystem>
#include "../Jobber/Technique.h"

struct aiMaterial;
struct aiMesh;

namespace Hydro::gfx
{
	class Material
	{
	public:
		Material( Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path ) noexcept(!_DEBUG);
		DynamicVertexBuffer ExtractVertices( const aiMesh& mesh ) const noexcept;
		std::vector<unsigned short> ExtractIndices( const aiMesh& mesh ) const noexcept;
		std::shared_ptr<Bind::VertexBuffer> MakeVertexBindable( Graphics& gfx, const aiMesh& mesh, float scale = 1.0f ) const noexcept(!_DEBUG);
		std::shared_ptr<Bind::IndexBuffer> MakeIndexBindable( Graphics& gfx, const aiMesh& mesh ) const noexcept(!_DEBUG);
		std::vector<Technique> GetTechniques() const noexcept;
	private:
		std::string MakeMeshTag( const aiMesh& mesh ) const noexcept;
	private:
		VertexLayout vtxLayout;
		std::vector<Technique> techniques;
		std::string modelPath;
		std::string name;
	};
}