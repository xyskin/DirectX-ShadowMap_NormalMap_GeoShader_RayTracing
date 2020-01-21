////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "inputclass.h"
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "modelclass.h"
#include "positionclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"
#include "cameraclass.h"
#include "rendertextureclass.h"
#include "shadowshaderclass.h"
#include "computeshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass*);

private:
	bool Render(InputClass*);
	bool RenderSceneToTexture();
private:
	D3DClass* m_D3D;
	ModelClass* m_Model, *m_Model2, *m_lightModel;
	PositionClass* m_PointLightPos, *m_ModelPos, *m_Model2Pos;
	LightClass* m_light;
	LightShaderClass* m_lightShader;
	CameraClass* m_camera, *m_shadowCam;
	RenderTextureClass* m_renderToTexture;
	ShadowShaderClass* m_shadowShader;

	int m_screenHeight, m_screenWidth;

	ComputeShaderClass* m_comShader;
};

#endif