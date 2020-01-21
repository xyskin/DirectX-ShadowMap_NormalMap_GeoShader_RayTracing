////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include <iostream>
using namespace std;
GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Model = 0;
	m_PointLightPos = 0;
	m_ModelPos = 0;
	m_light = 0;
	m_lightShader = 0;
	m_camera = 0;

	m_lightModel = 0;
	m_Model2 = 0;
	m_Model2Pos = 0;

	m_shadowCam = 0;
	m_shadowShader = 0;
	m_renderToTexture = 0;

	m_comShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/stone01.dds", L"../Engine/data/bump01.dds", 1.0f);
	if (!result)
	{
		return false;
	}

	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	result = m_Model2->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/stone01.dds", L"../Engine/data/bump01.dds", 0.2f);
	if (!result)
	{
		return false;
	}

	m_lightModel = new ModelClass;
	if (!m_lightModel)
	{
		return false;
	}

	result = m_lightModel->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/stone01.dds", L"../Engine/data/bump01.dds", 0.05f);
	if (!result)
	{
		return false;
	}

	m_lightShader = new LightShaderClass;
	if (!m_lightShader)
	{
		return false;
	}

	result = m_lightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	m_shadowShader = new ShadowShaderClass;
	if (!m_shadowShader)
	{
		return false;
	}

	result = m_shadowShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	m_renderToTexture = new RenderTextureClass;
	if (!m_renderToTexture)
	{
		return false;
	}

	result = m_renderToTexture->Initialize(m_D3D->GetDevice(), screenHeight * 2, screenHeight * 2, SCREEN_NEAR, SCREEN_DEPTH);
	if (!result)
	{
		return false;
	}

	m_PointLightPos = new PositionClass;
	if (!m_PointLightPos)
	{
		return false;
	}

	m_ModelPos = new PositionClass;
	if (!m_ModelPos)
	{
		return false;
	}

	m_Model2Pos = new PositionClass;
	if (!m_Model2Pos)
	{
		return false;
	}

	m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}

	m_camera = new CameraClass;
	if (!m_camera)
	{
		return false;
	}

	m_shadowCam = new CameraClass;
	if (!m_shadowCam)
	{
		return false;
	}

	m_PointLightPos->SetPosition(1.5f, 5.0f, 1.5f);
	m_ModelPos->SetPosition(0.0f, 0.0f, 0.0f);
	m_ModelPos->SetRotation(0.0f, 0.0f, 0.0f);

	m_Model2Pos->SetPosition(0.0f, 1.2f, 0.0f);
	m_Model2Pos->SetRotation(0.0f, 0.0f, 0.0f);

	m_light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_camera->SetUp(0.0f, 1.0f, 0.0f);
	m_camera->SetLookAt(0.0f, 0.0f, 1.0f);

	m_shadowCam->SetPosition(1.5f, 5.0f, 1.5f);
	m_shadowCam->SetRotation(0.0f, 0.0f, 0.0f);
	m_shadowCam->SetUp(0.0f, 0.0f, 1.0f);
	m_shadowCam->SetLookAt(-1.5f, -5.0f, -1.5f);

	m_comShader = new ComputeShaderClass;
	if (!m_comShader)
	{
		return false;
	}
	result = m_comShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_PointLightPos)
	{
		delete m_PointLightPos;
		m_PointLightPos = 0;
	}

	if (m_ModelPos)
	{
		delete m_ModelPos;
		m_ModelPos = 0;
	}

	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	if (m_Model2Pos)
	{
		delete m_Model2Pos;
		m_Model2Pos = 0;
	}

	if (m_lightModel)
	{
		delete m_lightModel;
		m_lightModel = 0;
	}

	if (m_shadowCam)
	{
		delete m_shadowCam;
		m_shadowCam = 0;
	}

	if (m_shadowShader)
	{
		m_shadowShader->Shutdown();
		delete m_shadowShader;
		m_shadowShader = 0;
	}

	if (m_renderToTexture)
	{
		m_renderToTexture->Shutdown();
		delete m_renderToTexture;
		m_renderToTexture = 0;
	}

	if (m_camera)
	{
		m_comShader->ShutDown();
		delete m_comShader;
		m_comShader = 0;
	}
	return;
}


bool GraphicsClass::Frame(InputClass* input)
{
	bool result;


	// Render the graphics scene.
	result = Render(input);
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(InputClass* input)
{
	bool result;
	D3DXMATRIX world, view, proj, ortho, sview, sproj, sortho;
	static float time = 0;
	static float bias = 0.01f;
	time += bias;
	//if (time >= 3.1415926f / 2.0f)
	//	bias = -bias;

	//if (time <= 0)
	//	bias = -bias;

	//cout << "time = " << time << endl;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	if (input->IsButtonClick(DIK_W))
	{
		m_camera->MoveForward();
	}

	if (input->IsButtonClick(DIK_S))
	{
		m_camera->MoveBackward();
	}

	if (input->IsButtonClick(DIK_A))
	{
		m_camera->MoveLeft();
	}

	if (input->IsButtonClick(DIK_D))
	{
		m_camera->MoveRight();
	}

	int mouseX, mouseY;
	input->GetMousePos(mouseX, mouseY);
	m_camera->SetRotation(mouseY / 10.0f, mouseX / 10.0f, 0);

	m_D3D->GetWorldMatrix(world);
	m_D3D->GetProjectionMatrix(proj);
	m_D3D->GetOrthoMatrix(ortho);

	m_camera->Render();
	m_camera->GetViewMatrix(view);

	m_renderToTexture->GetProjectionMatrix(sproj);
	m_renderToTexture->GetOrthoMatrix(sortho);

	m_shadowCam->Render();
	m_shadowCam->GetViewMatrix(sview);

	//m_Model->Render(m_D3D->GetDeviceContext());

	//D3DXMatrixRotationX(&world, m_ModelPos->GetRotation().x);
	//D3DXMatrixRotationY(&world, m_ModelPos->GetRotation().y);
	//D3DXMatrixRotationZ(&world, m_ModelPos->GetRotation().z);
	//D3DXMatrixTranslation(&world, m_ModelPos->GetPosition().x, m_ModelPos->GetPosition().y, m_ModelPos->GetPosition().z);

	//m_lightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), world, view, proj,
	//	m_PointLightPos->GetPosition(), m_light->GetAmbientColor(), m_light->GetDiffuseColor(), m_Model->GetTexture(),
	//	sview, sproj, m_renderToTexture->GetShaderResourceView(), time, m_camera->GetPosition(), m_Model->GetNormalMap());

	//m_Model2->Render(m_D3D->GetDeviceContext());
	//m_D3D->GetWorldMatrix(world);
	//D3DXMatrixRotationX(&world, m_Model2Pos->GetRotation().x);
	//D3DXMatrixRotationY(&world, m_Model2Pos->GetRotation().y);
	//D3DXMatrixRotationZ(&world, m_Model2Pos->GetRotation().z);
	//D3DXMatrixTranslation(&world, m_Model2Pos->GetPosition().x, m_Model2Pos->GetPosition().y, m_Model2Pos->GetPosition().z);

	//m_lightShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), world, view, proj,
	//	m_PointLightPos->GetPosition(), m_light->GetAmbientColor(), m_light->GetDiffuseColor(), m_Model2->GetTexture(),
	//	sview, sproj, m_renderToTexture->GetShaderResourceView(), time, m_camera->GetPosition(), m_Model2->GetNormalMap());


	//m_lightModel->Render(m_D3D->GetDeviceContext());
	//m_D3D->GetWorldMatrix(world);
	//D3DXMatrixRotationX(&world, m_PointLightPos->GetRotation().x);
	//D3DXMatrixRotationY(&world, m_PointLightPos->GetRotation().y);
	//D3DXMatrixRotationZ(&world, m_PointLightPos->GetRotation().z);
	//D3DXMatrixTranslation(&world, m_PointLightPos->GetPosition().x, m_PointLightPos->GetPosition().y, m_PointLightPos->GetPosition().z);
	//m_lightShader->Render(m_D3D->GetDeviceContext(), m_lightModel->GetIndexCount(), world, view, proj,
	//	m_PointLightPos->GetPosition(), m_light->GetAmbientColor(), m_light->GetDiffuseColor(), m_lightModel->GetTexture(),
	//	sview, sproj, NULL, time, m_camera->GetPosition(), m_lightModel->GetNormalMap());
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	cout << "posX = " << m_camera->GetPosition().x << " posY = " << m_camera->GetPosition().y << " posZ = " << m_camera->GetPosition().z << endl;
	return true;
}

bool GraphicsClass::RenderSceneToTexture()
{
	D3DXMATRIX world, sview, sproj, sortho;

	m_renderToTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the refraction render to texture.
	m_renderToTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	m_D3D->GetWorldMatrix(world);
	m_renderToTexture->GetProjectionMatrix(sproj);
	m_renderToTexture->GetOrthoMatrix(sortho);

	m_shadowCam->Render();
	m_shadowCam->GetViewMatrix(sview);

	m_Model->Render(m_D3D->GetDeviceContext());

	D3DXMatrixRotationX(&world, m_ModelPos->GetRotation().x);
	D3DXMatrixRotationY(&world, m_ModelPos->GetRotation().y);
	D3DXMatrixRotationZ(&world, m_ModelPos->GetRotation().z);
	D3DXMatrixTranslation(&world, m_ModelPos->GetPosition().x, m_ModelPos->GetPosition().y, m_ModelPos->GetPosition().z);

	m_shadowShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), world, sview, sproj);

	m_Model2->Render(m_D3D->GetDeviceContext());
	m_D3D->GetWorldMatrix(world);

	D3DXMatrixRotationX(&world, m_Model2Pos->GetRotation().x);
	D3DXMatrixRotationY(&world, m_Model2Pos->GetRotation().y);
	D3DXMatrixRotationZ(&world, m_Model2Pos->GetRotation().z);
	D3DXMatrixTranslation(&world, m_Model2Pos->GetPosition().x, m_Model2Pos->GetPosition().y, m_Model2Pos->GetPosition().z);

	m_shadowShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), world, sview, sproj);
	m_comShader->Render(m_D3D->GetDeviceContext());

	m_D3D->SetBackBufferRenderTarget();

	m_D3D->ResetViewport();

	ID3D11Resource* res;
	ID3D11Texture2D* tex;
	//m_renderToTexture->GetShaderResourceView()->GetResource(&res);
	//res->QueryInterface(&tex);

	tex = m_comShader->GetTexture();

	HRESULT result = D3DX11SaveTextureToFile(m_D3D->GetDeviceContext(), tex, D3DX11_IFF_PNG, L"../Engine/raytracing.png");
	if (FAILED(result))
	{
		return false;
	}
	return true;
}