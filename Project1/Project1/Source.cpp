#include<iostream>
#include <math.h>
#define NDEBUG 
#pragma comment(lib, "glui32d.lib")
#include <GL/glui.h>

#define SIZE_1		1
#define SIZE_2		2
#define SIZE_3		3
#define SOLID		4
#define WIRE		5
#define MENU_EXIT   6

using namespace std;

enum MyColor { Red, Green, Blue };
MyColor myColor = Red;

int window;
GLUI* glui;
GLUI_Translation* xT, * yT, * zT;
GLUI_Translation* xS, * yS, * zS;
GLUI_Rotation* rotationUI;

float rotationMatrix[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

float			aspect;

float			timer_interval = 16;

float			rotateAngle = 0.0f;
float			rotateSpeed = 30.0f;

float			oldbackGray;
float			backgroundGray = 1.0f;
float			clickPt_x;


float			teapot_posX = 0.0f;
float			teapot_posY = 0.0f;
float			teapot_posZ = 0.0f;

float			teapot_scaleX = 1.0f;
float			teapot_scaleY = 1.0f;
float			teapot_scaleZ = 1.0f;

float			teapot_size = 1.0f;
int				teapotType = SOLID;

int				isUniformScale = 0;

float clamp(float v, float min, float max);


void myGlutIdle(void)
{
	if (glutGetWindow() != window)
		glutSetWindow(window);

	glutPostRedisplay();
}

void reset_Pos(GLUI_Control* control)
{
	teapot_posX = 0.0f;
	teapot_posY = 0.0f;
	teapot_posZ = 0.0f;
}

void reset_Scale(GLUI_Control* control)
{
	teapot_scaleX = 1.0f;
	teapot_scaleY = 1.0f;
	teapot_scaleZ = 1.0f;
}

void reset_Rotate(GLUI_Control* control) 
{
	for (int i = 0; i < 16; i++) {
		rotationMatrix[i] = 0;
	}
	rotationMatrix[0] = 1;
	rotationMatrix[5] = 1;
	rotationMatrix[10] = 1;
	rotationMatrix[15] = 1;
	rotationUI->reset();
}

void set_uniform_Scale(int Control) 
{
	if (isUniformScale)
	{
		yS->disable();
		zS->disable();
	}
	else 
	{
		yS->enable();
		zS->enable();
	}
}

// 渲染事件, 用來在場景上繪製東西
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backgroundGray, backgroundGray, backgroundGray, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	glTranslatef(teapot_posX, teapot_posY, teapot_posZ);
	glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
	rotationUI->get_float_array_val(rotationMatrix);
	glMultMatrixf(rotationMatrix);
	
	if (isUniformScale)
		teapot_scaleZ = teapot_scaleY = teapot_scaleX;
	glScalef(teapot_scaleX, teapot_scaleY, teapot_scaleZ);

	if (myColor == Red) {
		glColor3b(125, 0, 0);
	}
	else if (myColor == Green) {
		glColor3b(0, 125, 0);
	}
	else if (myColor == Blue) {
		glColor3b(0, 0, 125);
	}
	if (teapotType == SOLID)
		glutSolidTeapot(teapot_size);
	else
		glutWireTeapot(teapot_size);
	glutSwapBuffers();
}

// 視窗重塑事件, 當視窗大小改變時被呼叫
void My_Reshape(int width, int height)
{
	int x = 0, y = 0;
	GLUI_Master.get_viewport_area(&x, &y, &width, &height);
	aspect = width * 1.0f / height;
	glui->refresh();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(x, y, width, height);
	gluPerspective(60.0f, aspect, 0.1f, 10.0f);
	glutSwapBuffers();
}

//計時器事件, 經指定時間後該函式被呼叫
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(timer_interval, My_Timer, val);
	rotateAngle += rotateSpeed * timer_interval * 0.001;
}

//滑鼠事件, 滑鼠經點擊或釋放時觸發
void My_Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)  //取得的按鍵為滑鼠左鍵
	{
		if (state == GLUT_DOWN)      //按鍵狀態為點擊時的瞬間
		{
			oldbackGray = backgroundGray;  //儲存當下背景明暗度
			clickPt_x = x;                 //儲存當下x軸的滑鼠位置
		}
	}
}

//鍵盤事件, 接收到鍵盤字元輸入時被呼叫
void My_Keyboard(unsigned char key, int x, int y)
{
	if (key == 'W' || key == 'w') {
		teapot_posY += 0.05f;
	}
	else if (key == 'S' || key == 's') {
		teapot_posY -= 0.05f;
	}
	else if (key == 'A' || key == 'a') {
		teapot_posX -= 0.05f;
	}
	else if (key == 'D' || key == 'd') {
		teapot_posX += 0.05f;
	}
	else if (key == 'Q' || key == 'q') {
		rotateSpeed += 10.0f;
	}
	else if (key == 'E' || key == 'e') {
		rotateSpeed -= 10.0f;
	}
}

//鍵盤事件, 接收到鍵盤功能鍵或方向鍵時被呼叫
void My_SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_F1) {
		myColor = Red;
	}
	else if (key == GLUT_KEY_F2) {
		myColor = Green;
	}
	else if (key == GLUT_KEY_F3) {
		myColor = Blue;
	}
}

//選單事件, 當選單的條目被選取時被呼叫
void My_Menu(int id)
{
	if (id == SIZE_1) {
		teapot_size = 1.0f;
	}
	else if (id == SIZE_2) {
		teapot_size = 1.5f;
	}
	else if (id == SIZE_3) {
		teapot_size = 2.0f;
	}
	else if (id == SOLID) {
		teapotType = SOLID;
	}
	else if (id == WIRE) {
		teapotType = WIRE;
	}
	else if (id == MENU_EXIT) {
		exit(0);
	}
}

//滑鼠拖曳事件, 滑鼠進行拖曳時觸發
void Mouse_Moving(int x, int y) {
	backgroundGray = (x - clickPt_x) * 0.005f + oldbackGray; //根據滑鼠移動到的座標與點擊時的座標的向量差，計算明亮度
	backgroundGray = clamp(backgroundGray, 0.0f, 1.0f);      //避免計算值超過範圍
}

//init glui intreface
void initUI()
{
	GLUI_Panel* translationPanel = glui->add_panel("Translation");
	GLUI_Panel* controlTransPanel = glui->add_panel_to_panel(translationPanel, "control_trans", GLUI_PANEL_NONE);
	xT = glui->add_translation_to_panel(controlTransPanel, "X", GLUI_TRANSLATION_X, &teapot_posX);
	xT->set_speed(0.1);
	glui->add_column_to_panel(controlTransPanel, 0);
	yT = glui->add_translation_to_panel(controlTransPanel, "Y", GLUI_TRANSLATION_Y, &teapot_posY);
	yT->set_speed(0.1);
	glui->add_column_to_panel(controlTransPanel, 0);
	zT = glui->add_translation_to_panel(controlTransPanel, "Z", GLUI_TRANSLATION_Z, &teapot_posZ);
	zT->set_speed(0.1);
	GLUI_Button* rePosbt = glui->add_button_to_panel(translationPanel, "Reset Position", 1, reset_Pos);

	GLUI_Panel* scalePanel = glui->add_panel("Scale");
	GLUI_Panel* controlScalePanel = glui->add_panel_to_panel(scalePanel, "control_trans", GLUI_PANEL_NONE);
	xS = glui->add_translation_to_panel(controlScalePanel, "X", GLUI_TRANSLATION_X, &teapot_scaleX);
	xS->set_speed(0.1);
	glui->add_column_to_panel(controlScalePanel, 0);
	yS = glui->add_translation_to_panel(controlScalePanel, "Y", GLUI_TRANSLATION_X, &teapot_scaleY);
	yS->set_speed(0.1);
	glui->add_column_to_panel(controlScalePanel, 0);
	zS = glui->add_translation_to_panel(controlScalePanel, "Z", GLUI_TRANSLATION_X, &teapot_scaleZ);
	zS->set_speed(0.1);

	GLUI_Checkbox* uniformScaleCheck = glui->add_checkbox_to_panel(scalePanel, "Uniform Scale", &isUniformScale, 1, set_uniform_Scale);
	GLUI_Button* reScalebt = glui->add_button_to_panel(scalePanel, "Reset Scale", 1, reset_Scale);
	
	GLUI_Panel* rotatePanel = glui->add_panel("Rotate");
	rotationUI = glui->add_rotation_to_panel(rotatePanel,"Rotation", rotationMatrix);
	GLUI_Button* reRotatebt = glui->add_button_to_panel(rotatePanel, "Reset Rotate", 1, reset_Rotate);
	glui->set_main_gfx_window(window);
}

int main(int argc, char* argv[])
{

#ifdef __APPLE__
	//更改工作路徑
	chdir(__FILEPATH__("/../Assets/"));
#endif

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);

	window = glutCreateWindow("glut");
	//glui設定
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutKeyboardFunc(My_Keyboard);
	GLUI_Master.set_glutSpecialFunc(My_SpecialKeys);
	GLUI_Master.set_glutReshapeFunc(My_Reshape);
	glui = GLUI_Master.create_glui_subwindow(window, GLUI_SUBWINDOW_RIGHT);

	initUI();

	glClearColor(backgroundGray, backgroundGray, backgroundGray, 1.0f);

	//定義選單結構
	////////////////////
	int menu_main = glutCreateMenu(My_Menu);
	int menu_entry = glutCreateMenu(My_Menu);

	glutSetMenu(menu_main);
	glutAddSubMenu("Teapot size", menu_entry);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutSetMenu(menu_entry);
	glutAddMenuEntry("1.0", SIZE_1);
	glutAddMenuEntry("1.5", SIZE_2);
	glutAddMenuEntry("2.0", SIZE_3);
	glutAddMenuEntry("Solid", SOLID);
	glutAddMenuEntry("Wire", WIRE);

	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	////////////////////

	//註冊GLUT回呼事件
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);        //註冊滑鼠按鍵的回呼事件
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_interval, My_Timer, 0);
	glutMotionFunc(Mouse_Moving);  //註冊滑鼠拖曳時的回呼事件  
	////////////////////

	// 進入主迴圈
	glutMainLoop();

	return 0;
}

float clamp(float v, float min, float max) {
	if (v >= max) {
		return max;
	}
	else if (v <= min) {
		return min;
	}
	else {
		return v;
	}
}
