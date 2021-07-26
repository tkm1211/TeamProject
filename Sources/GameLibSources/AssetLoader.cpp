#include "AssetLoader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


AssetLoader assetLoader;


char* modelNames[] = {
	"model1",
	"model2",
	"model3",
	"model4",
	"model5",
	"model6",
	"model7",
	"model8",
	"model9",
	"model10"
};


void AssetLoader::Loader( const wchar_t* _fileName )
{
	wchar_t fileName[256];
	bool isInput = false;
	int cnt = 0;
	for (int i = 0; i < 256; i++)
	{
		if ( _fileName[i] == 'D' && _fileName[i+1] == 'a' && _fileName[i+2] == 't' && _fileName[i+3] == 'a' )
		{
			isInput = true;
		}
		if ( isInput )
		{
			fileName[cnt++] = _fileName[i];
		}
	}

	FileWrite( fileName );

	for ( int i = 0; i < 256; i++ )
	{
		if ( fileName[i] == '.' && fileName[i+1] == 'p' && fileName[i+2] == 'n' && fileName[i+3] == 'g' )
		{
			LoadSprite( fileName );
			break;
		}
		else if ( fileName[i] == '.' && fileName[i+1] == 'o' && fileName[i+2] == 'b' && fileName[i+3] == 'j' )
		{
			LoadOBJ( fileName );
			break;
		}
		else if ( fileName[i] == '.' && fileName[i+1] == 'f' && fileName[i+2] == 'b' && fileName[i+3] == 'x'
			   || fileName[i] == '.' && fileName[i+1] == 'F' && fileName[i+2] == 'B' && fileName[i+3] == 'X')
		{
			LoadFBX( fileName );
			break;
		}
	}
}

void AssetLoader::LoadSprite( const wchar_t* _fileName )
{
	/*if ( sprite[9] ) return;

	for ( int i = 0; i < MAX_SIZE; i++ )
	{
		if ( !sprite[i] )
		{
			sprite[i] = std::make_unique<SpriteBatch>( _fileName );
			break;
		}
	}*/
}

void AssetLoader::LoadOBJ( const wchar_t* _fileName )
{
	/*if ( model[9] ) return;

	char fileName[256];
	size_t size;
	errno_t err;
	err = wcstombs_s(&size, fileName, 256, _fileName, _TRUNCATE);

	for ( int i = 0; i < MAX_SIZE; i++ )
	{
		if ( !model[i] )
		{
			model[i] = std::make_unique<OBJ3D>( MeshType::STATIC_MESH, fileName );
			break;
		}
	}*/
}

void AssetLoader::LoadFBX( const wchar_t* _fileName )
{
	if ( model[9] ) return;

	char fileName[256];
	size_t size;
	errno_t err;
	err = wcstombs_s(&size, fileName, 256, _fileName, _TRUNCATE);

	for ( int i = 0; i < MAX_SIZE; i++ )
	{
		if ( !model[i] )
		{
			model[i] = std::make_unique<Model>( fileName, false );
			break;
		}
	}
}

void AssetLoader::FileWrite( const wchar_t* _fileName )
{
	wchar_t fileName[] = L"Data/Document/filename.txt";

	std::wifstream ifs( fileName );

	std::wofstream writeFile;
	writeFile.open( fileName, std::wios::out | std::wios::app );

	writeFile << _fileName << std::endl;

	writeFile.close();
}


void AssetLoader::Update()
{
	ImGui::PushStyleColor( ImGuiCol_TitleBgActive, ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
	ImGui::PushStyleColor( ImGuiCol_TitleBg, ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
	//ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_Once);

	ImGui::Begin( u8"Model" );
	for ( int i = 0; i < MAX_SIZE; i++ )
	{
		if ( !model[i] ) continue;
		if ( ImGui::TreeNode( modelNames[i] ) )
		{
			static DirectX::XMFLOAT3 _pos;
			ImGui::DragFloat3( "pos", &_pos.x );
			modelData[i].SetPos( _pos );

			ImGui::InputInt( "AnimationNumber", &modelAnim[i].animNum );
			if (ImGui::Button( "AnimationStart" ) )
			{
				model[i]->StartAnimation( modelAnim[i].animNum, true );
			}
			if ( ImGui::Button("AnimationStop" ) )
			{
				model[i]->PauseAnimation();
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}


void AssetLoader::Render
 (  const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& lightDirection,
	float elapsedTime )
{
	for ( int i = 0; i < MAX_SIZE; i++ )
	{
		if ( model[i] )
		{
			model[i]->Render(modelData[i].GetWorldMatrix(),  view, projection, lightDirection, modelData[i].GetColor(),  elapsedTime );
		}
	}
}