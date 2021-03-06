/*
Physics Effects Copyright(C) 2010 Sony Computer Entertainment Inc.
All rights reserved.

Physics Effects is open software; you can redistribute it and/or
modify it under the terms of the BSD License.

Physics Effects is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the BSD License for more details.

A copy of the BSD License is distributed with
Physics Effects under the filename: physics_effects_license.txt
*/

#include "../common/common.h"
#include "../common/ctrl_func.h"
#include "../common/render_func.h"
#include "../common/perf_func.h"
#include "physics_func.h"
#include "barrel.h"
#include "landscape.h"

#ifdef _WIN32
	#include <gl/gl.h>
	#include <gl/glu.h>
#endif

#define	SAMPLE_NAME "api_physics_effects/2_stable"

static bool s_isRunning = true;

int sceneId = 2;
bool simulating = true;

int landscapeMeshId;
int convexMeshId;

static void render(void)
{
	render_begin();
	
	for(int i=0;i<physics_get_num_rigidbodies();i++) {
		const PfxRigidState &state = physics_get_state(i);
		const PfxCollidable &coll = physics_get_collidable(i);

		PfxTransform3 rbT(state.getOrientation(), state.getPosition());

		PfxShapeIterator itrShape(coll);
		for(int j=0;j<coll.getNumShapes();j++,++itrShape) {
			const PfxShape &shape = *itrShape;
			PfxTransform3 offsetT = shape.getOffsetTransform();
			PfxTransform3 worldT = rbT * offsetT;

			switch(shape.getType()) {
				case kPfxShapeSphere:
				render_sphere(
					worldT,
					PfxVector3(1,1,1),
					PfxFloatInVec(shape.getSphere().m_radius));
				break;

				case kPfxShapeBox:
				render_box(
					worldT,
					PfxVector3(1,1,1),
					shape.getBox().m_half);
				break;

				case kPfxShapeCapsule:
				render_capsule(
					worldT,
					PfxVector3(1,1,1),
					PfxFloatInVec(shape.getCapsule().m_radius),
					PfxFloatInVec(shape.getCapsule().m_halfLen));
				break;

				case kPfxShapeCylinder:
				render_cylinder(
					worldT,
					PfxVector3(1,1,1),
					PfxFloatInVec(shape.getCylinder().m_radius),
					PfxFloatInVec(shape.getCylinder().m_halfLen));
				break;

				case kPfxShapeConvexMesh:
				render_mesh(
					worldT,
					PfxVector3(1,1,1),
					convexMeshId,2.f);
				break;

				case kPfxShapeLargeTriMesh:
				render_mesh(
					worldT,
					PfxVector3(1,1,1),
					landscapeMeshId);
				break;

				default:
				break;
			}
		}
	}

	render_end();
}

static int init(void)
{
	perf_init();
	ctrl_init();
	render_init();
	physics_init();

	landscapeMeshId = render_init_mesh(
		LargeMeshVtx,sizeof(float)*6,
		LargeMeshVtx+3,sizeof(float)*6,
		LargeMeshIdx,sizeof(unsigned short)*3,
		LargeMeshVtxCount,LargeMeshIdxCount/3);

	convexMeshId = render_init_mesh(
		BarrelVtx,sizeof(float)*6,
		BarrelVtx+3,sizeof(float)*6,
		BarrelIdx,sizeof(unsigned short)*3,
		BarrelVtxCount,BarrelIdxCount/3);

	return 0;
}

static int shutdown(void)
{
	ctrl_release();
	render_release();
	physics_release();
	perf_release();

	return 0;
}

static void update(float deltaTime)
{
	float dt = deltaTime*10.f;
	float angX,angY,r;
	render_get_view_angle(angX,angY,r);

	ctrl_update();
	
	if(ctrl_button_pressed(BTN_UP)) {
		angX -= 0.05f*dt;
		if(angX < -1.4f) angX = -1.4f;
		if(angX > -0.01f) angX = -0.01f;
	}

	if(ctrl_button_pressed(BTN_DOWN)) {
		angX += 0.05f*dt;
		if(angX < -1.4f) angX = -1.4f;
		if(angX > 0.0f) 
			angX = 0.0f;
	}

	if(ctrl_button_pressed(BTN_LEFT)) {
		angY -= 0.05f*dt;
	}

	if(ctrl_button_pressed(BTN_RIGHT)) {
		angY += 0.05f*dt;
	}

	if(ctrl_button_pressed(BTN_ZOOM_OUT)) {
		r *= (1.f+0.1f*dt);
		if(r > 500.0f) r = 500.0f;
	}

	if(ctrl_button_pressed(BTN_ZOOM_IN)) {
		r *= (1.f-0.1f*dt);
		if(r < 1.0f) r = 1.0f;
	}

	if(ctrl_button_pressed(BTN_SCENE_RESET) == BTN_STAT_DOWN) {
		physics_create_scene(sceneId);
	}

	if(ctrl_button_pressed(BTN_SCENE_NEXT) == BTN_STAT_DOWN) {
		physics_create_scene(++sceneId);
	}

	if(ctrl_button_pressed(BTN_SIMULATION) == BTN_STAT_DOWN) {
		simulating = !simulating;
	}

	if(ctrl_button_pressed(BTN_STEP) == BTN_STAT_DOWN) {
		simulating = true;
	}
	else if(ctrl_button_pressed(BTN_STEP) == BTN_STAT_UP || ctrl_button_pressed(BTN_STEP) == BTN_STAT_KEEP) {
		simulating = false;
	}

	render_set_view_angle(angX,angY,r);
}

#ifndef _WIN32

///////////////////////////////////////////////////////////////////////////////
// Main

int main(void)
{
	init();

	physics_create_scene(sceneId);
	
	printf("## %s: INIT SUCCEEDED ##\n", SAMPLE_NAME);

	while (s_isRunning) {
		update();
		if(simulating) physics_simulate();
		render();

		perf_sync();
	}

	shutdown();

	printf("## %s: FINISHED ##\n", SAMPLE_NAME);

	return 0;
}

#else

///////////////////////////////////////////////////////////////////////////////
// WinMain

extern HDC hDC;
extern HGLRC hRC;
HWND hWnd;
HINSTANCE hInstance;

void releaseWindow()
{
	if(hRC) {
		wglMakeCurrent(0,0);
		wglDeleteContext(hRC);
	}
	
	if(hDC) ReleaseDC(hWnd,hDC);
	if(hWnd) DestroyWindow(hWnd);
	
	UnregisterClass(SAMPLE_NAME,hInstance);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT	uMsg,WPARAM	wParam,LPARAM lParam)
{
	switch(uMsg) {
		case WM_SYSCOMMAND:
		{
			switch (wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

		case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		case WM_SIZE:
		render_resize(LOWORD(lParam),HIWORD(lParam));
		return 0;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

bool createWindow(char* title, int width, int height)
{
	WNDCLASS wc;
	RECT rect;
	rect.left=0;
	rect.right=width;
	rect.top=0;
	rect.bottom=height;

	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = SAMPLE_NAME;

	if(!RegisterClass(&wc)) {
		return false;
	}

	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	if(!(hWnd=CreateWindowEx(WS_EX_APPWINDOW|WS_EX_WINDOWEDGE,SAMPLE_NAME,title,
							WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
							0,0,rect.right-rect.left,rect.bottom-rect.top,
							NULL,NULL,hInstance,NULL))) {
		releaseWindow();
		return false;
	}

    static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
    };
	
	if(!(hDC=GetDC(hWnd)))
	{
		releaseWindow();
		OutputDebugString("");
		return FALSE;
	}
	
	int pixelformat;
	
    if ( (pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0 ){
		OutputDebugString("ChoosePixelFormat Failed....");
        return FALSE;
    }

    if (SetPixelFormat(hDC, pixelformat, &pfd) == FALSE){
		OutputDebugString("SetPixelFormat Failed....");
        return FALSE;
    }

	if (!(hRC=wglCreateContext(hDC))){
		OutputDebugString("Creating HGLRC Failed....");
		return FALSE;
	}
	
	// Set Vsync
	//BOOL (WINAPI *wglSwapIntervalEXT)(int) = NULL;

	//if(strstr((char*)glGetString( GL_EXTENSIONS ),"WGL_EXT_swap_control")== 0) {
	//}
	//else {
		//wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT");
		//if(wglSwapIntervalEXT) wglSwapIntervalEXT(1);
	//}
	
	wglMakeCurrent(hDC,hRC);
	
	ShowWindow(hWnd,SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	render_resize(width, height);
	
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	
	return TRUE;
}

static float sLocalTime = 0.f;
static float sFixedTimeStep = 1.f/60.f;

void stepSimulation(float dt)
{
	int maxSubSteps = 10;
	int numSimulationSubSteps = 0;
	if (maxSubSteps)
	{
		//fixed timestep with interpolation
		sLocalTime += dt;
		if (sLocalTime >= sFixedTimeStep)
		{
			numSimulationSubSteps = int( sLocalTime / sFixedTimeStep);
			sLocalTime -= numSimulationSubSteps * sFixedTimeStep;
		}
		if (numSimulationSubSteps)
		{
			//clamp the number of substeps, to prevent simulation grinding spiralling down to a halt
			int clampedSimulationSteps = (numSimulationSubSteps > maxSubSteps)? maxSubSteps : numSimulationSubSteps;
			for (int i=0;i<clampedSimulationSteps;i++)
			{
				physics_simulate();
			}
		} 
	}
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	if(!createWindow(SAMPLE_NAME,DISPLAY_WIDTH,DISPLAY_HEIGHT)) {
		MessageBox(NULL,"Can't create gl window.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}
	
	init();
	
	physics_create_scene(sceneId);

	PfxPerfCounter counter;
	counter.countBegin("dt");

	SCE_PFX_PRINTF("## %s: INIT SUCCEEDED ##\n", SAMPLE_NAME);
	
	MSG msg;
	while(s_isRunning) {
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			if(msg.message==WM_QUIT) {
				s_isRunning = false;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			counter.countEnd();
			float dt = counter.getCountTime(0)/1000.f;

			update(dt);

			if(simulating) 
				stepSimulation(dt);

			counter.resetCount();
			counter.countBegin("dt");

			render();

			perf_sync();
		}
	}

	shutdown();

	SCE_PFX_PRINTF("## %s: FINISHED ##\n", SAMPLE_NAME);

	releaseWindow();
	return (msg.wParam);
}

#endif
