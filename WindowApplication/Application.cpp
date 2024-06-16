#include "Application.h"
#include <Core/third/ImGui/imgui.h>
#include <Core/third/ImGui/imgui_impl_dx11.h>
#include <Core/third/ImGui/imgui_impl_win32.h>
#include <dxtex/DirectXTex.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Core/src/gfx/Model/Mesh.h>
#include <Core/src/gfx/DynamicConstant.h>
#include <Core/src/gfx/Model/ModelProbe.h>
#include <Core/src/gfx/Model/Node.h>
#include <Core/src/misc/HydroXM.h>

using namespace Hydro;
using namespace Hydro::gfx;

WindowApplication::WindowApplication()
	:
	App( 1280, 720 ),
	light( window.Gfx(), 0.5f )
{

	cube.SetPos( { 4.0f,0.0f,0.0f } );
	cube2.SetPos( { 0.0f,4.0f,0.0f } );

	window.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 9.0f / 16.0f, 0.5f, 400.0f ) );
}

WindowApplication::~WindowApplication()
{}

void WindowApplication::DoFrame()
{
	float dt = timer.Mark() * speed_factor;
	window.Gfx().BeginFrame( 0.07f, 0.0f, 0.12f );
	window.Gfx().SetCamera( cam.GetMatrix() );
	light.Bind( window.Gfx(), cam.GetMatrix() );
	
	light.Submit( fc );
	cube.Submit( fc );
	sponza.Submit( fc );
	cube2.Submit( fc );
	//gobber.Submit( fc );

	fc.Execute( window.Gfx() );

	while( const auto e = window.kbd.ReadKey() )
	{
		if( !e->IsPress() )
		{
			continue;
		}

		switch( e->GetCode() )
		{
		case VK_ESCAPE:
			if( window.CursorEnabled() )
			{
				window.DisableCursor();
				window.mouse.EnableRaw();
			}
			else
			{
				window.EnableCursor();
				window.mouse.DisableRaw();
			}
			break;
		}
	}

	//Camera Movement
	if( !window.CursorEnabled() )
	{
		if( window.kbd.KeyIsPressed( 'W' ) )
		{
			cam.Translate( { 0.0f,0.0f,dt } );
		}
		if( window.kbd.KeyIsPressed( 'A' ) )
		{
			cam.Translate( { -dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'S' ) )
		{
			cam.Translate( { 0.0f,0.0f,-dt } );
		}
		if( window.kbd.KeyIsPressed( 'D' ) )
		{
			cam.Translate( { dt,0.0f,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'R' ) )
		{
			cam.Translate( { 0.0f,dt,0.0f } );
		}
		if( window.kbd.KeyIsPressed( 'F' ) )
		{
			cam.Translate( { 0.0f,-dt,0.0f } );
		}
	}

	while( const auto delta = window.mouse.ReadRawDelta() )
	{
		if( !window.CursorEnabled() )
		{
			cam.Rotate( (float)delta->x, (float)delta->y );
		}
	}

	//Imgui window to control simulation speed
	if( ImGui::Begin( "Simulation speed " ) )
	{
		ImGui::SliderFloat( "Speed", &speed_factor, 0.0f, 6.0f, "%.4f" );
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s ( hold space to pause )", window.kbd.KeyIsPressed( VK_SPACE ) ?  "Paused" : "Running" );
	}
	ImGui::End();
	
	// Mesh techniques window
	class TP : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override
		{
			using namespace std::string_literals;
			ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f }, pTech->GetName().c_str() );
			bool active = pTech->IsActive();
			ImGui::Checkbox( ("Tech Active##"s + std::to_string( techIdx )).c_str(), &active );
			pTech->SetActiveState( active );
		}
		bool OnVisitBuffer( Dcb::Buffer& buf ) override
		{
			namespace dx = DirectX;
			float dirty = false;
			const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
			auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string( bufIdx )]
			( const char* label ) mutable
			{
				tagScratch = label + tagString;
				return tagScratch.c_str();
			};

			if( auto v = buf["scale"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Scale" ), &v, 1.0f, 2.0f, "%.3f" ) );
			}
			if( auto v = buf["offset"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "offset" ), &v, 0.0f, 1.0f, "%.3f" ) );
			}
			if( auto v = buf["materialColor"]; v.Exists() )
			{
				dcheck( ImGui::ColorPicker3( tag( "Color" ), reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v)) ) );
			}
			if( auto v = buf["specularColor"]; v.Exists() )
			{
				dcheck( ImGui::ColorPicker3( tag( "Spec. Color" ), reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v)) ) );
			}
			if( auto v = buf["specularGloss"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Glossiness" ), &v, 1.0f, 100.0f, "%.1f" ) );
			}
			if( auto v = buf["specularWeight"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Spec. Weight" ), &v, 0.0f, 2.0f ) );
			}
			if( auto v = buf["useSpecularMap"]; v.Exists() )
			{
				dcheck( ImGui::Checkbox( tag( "Spec. Map Enable" ), &v ) );
			}
			if( auto v = buf["useNormalMap"]; v.Exists() )
			{
				dcheck( ImGui::Checkbox( tag( "Normal Map Enable" ), &v ) );
			}
			if( auto v = buf["normalMapWeight"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Normal Map Weight" ), &v, 0.0f, 2.0f ) );
			}
			return dirty;
		}
	};

	class MP : ModelProbe
	{
	public:
		void SpawnWindow( Model& model )
		{
			ImGui::Begin( "Model" );
			ImGui::Columns( 2, nullptr, true );
			model.Accept( *this );

			ImGui::NextColumn();
			if( pSelectedNode != nullptr )
			{
				bool dirty = false;
				const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
				auto& tf = ResolveTransform();
				ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f }, "Translation" );
				dcheck( ImGui::SliderFloat( "X", &tf.x, -60.f, 60.f ) );
				dcheck( ImGui::SliderFloat( "Y", &tf.y, -60.f, 60.f ) );
				dcheck( ImGui::SliderFloat( "Z", &tf.z, -60.f, 60.f ) );
				ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f }, "Orientation" );
				dcheck( ImGui::SliderAngle( "X-rotation", &tf.xRot, -180.0f, 180.0f ) );
				dcheck( ImGui::SliderAngle( "Y-rotation", &tf.yRot, -180.0f, 180.0f ) );
				dcheck( ImGui::SliderAngle( "Z-rotation", &tf.zRot, -180.0f, 180.0f ) );
				if( dirty )
				{
					pSelectedNode->SetAppliedTransform(
						DirectX::XMMatrixRotationX( tf.xRot ) *
						DirectX::XMMatrixRotationY( tf.yRot ) *
						DirectX::XMMatrixRotationZ( tf.zRot ) *
						DirectX::XMMatrixTranslation( tf.x, tf.y, tf.z )
					);
				}
			}
			if( pSelectedNode != nullptr )
			{
				TP probe;
				pSelectedNode->Accept( probe );
			}
			ImGui::End();
		}
	protected:
		bool PushNode( Node& node ) override
		{
			// if there is no selected node, set selectedId to an impossible value
			const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
			// build up flags for current node
			const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
				| ((node.GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
				| (node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf);
			// render this node
			const auto expanded = ImGui::TreeNodeEx(
				(void*)(intptr_t)node.GetId(),
				node_flags, node.GetName().c_str()
			);
			// processing for selecting node
			if( ImGui::IsItemClicked() )
			{
				// used to change the highlighted node on selection change
				struct Probe : public TechniqueProbe
				{
					virtual void OnSetTechnique() override
					{
						if( pTech->GetName() == "Outline" )
						{
							pTech->SetActiveState( highlighted );
						}
					}
					bool highlighted = false;
				} probe;

				// remove highlight on prev-selected node
				if( pSelectedNode != nullptr )
				{
					pSelectedNode->Accept( probe );
				}
				// add highlight to newly-selected node
				probe.highlighted = true;
				node.Accept( probe );

				pSelectedNode = &node;
			}
			// signal if children should also be recursed
			return expanded;
		}
		void PopNode( Node& node ) override
		{
			ImGui::TreePop();
		}
	private:
		Node* pSelectedNode = nullptr;
		struct TransformParameters
		{
			float xRot = 0.0f;
			float yRot = 0.0f;
			float zRot = 0.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		};
		std::unordered_map<int, TransformParameters> transformParams;
	private:
		TransformParameters& ResolveTransform() noexcept
		{
			const auto id = pSelectedNode->GetId();
			auto i = transformParams.find( id );
			if( i == transformParams.end() )
			{
				return LoadTransform( id );
			}
			return i->second;
		}
		TransformParameters& LoadTransform( int id ) noexcept
		{
			const auto& applied = pSelectedNode->GetAppliedTransform();
			const auto angles = ExtractEulerAngles( applied );
			const auto translation = ExtractTranslation( applied );
			TransformParameters tp;
			tp.zRot = angles.z;
			tp.xRot = angles.x;
			tp.yRot = angles.y;
			tp.x = translation.x;
			tp.y = translation.y;
			tp.z = translation.z;
			return transformParams.insert( { id,{ tp } } ).first->second;
		}
	};
	static MP modelProbe;

	//Imgui windows
	modelProbe.SpawnWindow( sponza );
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	cube.SpawnControlWindow( window.Gfx(), "Cube 1" );
	cube2.SpawnControlWindow( window.Gfx(), "Cube 2" );

	window.Gfx().EndFrame();
	fc.Reset();
}



