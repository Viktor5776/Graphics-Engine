#include "Node.h"
#include "Mesh.h"
#include "ModelProbe.h"
#include "../../../third/ImGui/imgui.h"

namespace Hydro::gfx
{

	Node::Node( int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in ) noexcept(!_DEBUG)
		:
		id( id ),
		meshPtrs( std::move( meshPtrs ) ),
		name( name )
	{
		DirectX::XMStoreFloat4x4( &transform, transform_in );
		DirectX::XMStoreFloat4x4( &appliedTransform, DirectX::XMMatrixIdentity() );
	}

	void Node::Submit( DirectX::FXMMATRIX accumulatedTransform ) const noexcept(!_DEBUG)
	{
		const auto built =
			DirectX::XMLoadFloat4x4( &appliedTransform ) *
			DirectX::XMLoadFloat4x4( &transform ) *
			accumulatedTransform;
		for( const auto pm : meshPtrs )
		{
			pm->Submit( built );
		}
		for( const auto& pc : childPtrs )
		{
			pc->Submit( built );
		}
	}

	void Node::AddChild( std::unique_ptr<Node> pChild ) noexcept(!_DEBUG)
	{
		assert( pChild );
		childPtrs.push_back( std::move( pChild ) );
	}

	void Node::SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept
	{
		DirectX::XMStoreFloat4x4( &appliedTransform, transform );
	}

	const DirectX::XMFLOAT4X4& Node::GetAppliedTransform() const noexcept
	{
		return appliedTransform;
	}

	int Node::GetId() const noexcept
	{
		return id;
	}

	void Node::Accept( ModelProbe& probe )
	{
		if( probe.PushNode( *this ) )
		{
			for( auto& cp : childPtrs )
			{
				cp->Accept( probe );
			}
			probe.PopNode( *this );
		}
	}

	void Node::Accept( TechniqueProbe& probe )
	{
		for( auto& mp : meshPtrs )
		{
			mp->Accept( probe );
		}
	}

}