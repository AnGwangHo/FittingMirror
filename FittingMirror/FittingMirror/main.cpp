#include "stdafx.h"
#include <glut.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>
#include <gl\GL.h>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <NuiApi.h>
#include <tchar.h>

#include <iostream>

#include <math.h>
#include "Main.h"
#include "polygon.h"

using namespace std;
using namespace cv;

#define COLOR_WIDTH 640
#define COLOR_HEIGHT 480

extern CvPoint points[NUI_SKELETON_POSITION_COUNT];//���̷����� �� ���������� 20����ŭ �迭�� ����
//CvCapture* capture;//ī�޶�� ���� �������� �޾ƿ�
extern IplImage* iplTexture = 0;//OpenCV ���� ī�޶�κ��� ���� �������� �����ϴ� ����
GLuint texture;  // array in which the generated texture names are stored

//������ ������
int screenW;
int screenH;

//ī�޶� �ٶ󺸴� ��ǥ�� ��ġ��
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 2.0f;

//���� view size ������ ���� ����
long double view_size[2] = { 1.0f, 1.0f };

CreateRGBImage cr;//���� Ŭ���� ����


extern bool history_choice;//�����丮 ��ȸ ���� ���� flag
int upclothsx, upclothsy;

extern bool user_appreciate;//skeleton�� ���������� ������� �νĿ��θ� üũ�ϴ� flag
extern bool ad_on, ad_off;//���� on/off flag

//�����丮 ��ȸ ���� flag
extern bool history_left_click;
extern bool history_right_click;

// 3ds file loader class
Model_3DS	Suit3DS;
Model_3DS   GreenSuit3DS;
Model_3DS   GirlShirt;

//GL �ؽ��� ó���� ���� �迭
GLuint textures[12];

float rotation = 0.0f;//���� ȸ���� ����

extern bool fitting_first_popup;//�˾� ���� ���� flag
extern bool history_exit;//�����丮��ȸ �ݱ� flag

IplImage *viewNowImage;//����� ���� �����ִ� ����
IplImage *user_guide = cvLoadImage("./Image/user_guide.JPG");//���̵���� �˾�â �̹���
IplImage *adImage = cvLoadImage("./Image/ready.png");//���� �̹���

//���� �����丮 ��ȸ ���ý� ������ UI�̹���
IplImage *hitstory_close = cvLoadImage("./Image/close.png");
IplImage *histroy_user_save = cvLoadImage("./Image/cloud.png");

//main UI �̹���
IplImage* ui_save = cvLoadImage("./Image/save.png");
IplImage* ui_history = cvLoadImage("./Image/history.png");
IplImage* ui_cloth_box = cvLoadImage("./Image/fittingBox.png");
IplImage* ui_login = cvLoadImage("./Image/login.png");//�α��� �̹���
IplImage* ui_logout = cvLoadImage("./Image/logout.png");//�α׾ƿ� �̹���

//���� UI �̹���
IplImage* ui_check = cvLoadImage("./Image/check.png");
IplImage* ui_qrcheck = cvLoadImage("./Image/QRCheck.png");

extern bool user_coner_ok;//�ڳ� ���� �Ϸ� flag
extern bool picture_save;//���� ���� flag
extern bool login;//�α��� flag
extern bool qrcode;//QR�ڵ� ���� ���� flag
extern float picture_save_time;//���� ���� �ð�
extern int currently_clothes_Num;//���� ���õ� ���� �ѹ�

bool textureChange = false;//�� �ؽ��� ���� flag
bool ad_reset = false;//���� ��ε��

extern CvPoint conner_points[NUI_SKELETON_POSITION_COUNT];//����� �ڳ� ��ǥ �迭
extern int user_hip, user_shoulder_top, user_shoulder_max ;//�����Ǿ� ������ ��

//���� ȸ�� �� �� x,y,z ���� ����
float suit_pos_z, suit_pos_y;
float suit_x, suit_y, suit_z;

void cameraInit();
void myInit();
void myIdle();
void drawGraphics();
void myReshape(int width, int height);
void fileLoad();
void InitializeKinect();
void clothInit();

//���� ���ν���
int main(int argc, char * argv[])
{
	// initialize glut
	glutInit(&argc, argv);//OpenGL ����� ���� �ʱ��Լ�
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//â ��¿��� ��� ����

	cameraInit();//ī�޶� �ʱ�ȭ

	// initialize the window
	glutInitWindowSize(screenW, screenH);//���α׷� â ũ��
	glutInitWindowPosition(100, 100);//�»���� �������� ��Ÿ�� ��ġ�� ����
	glutCreateWindow("Fitting Mirror");//�����̸�

	myInit();//�ʱ⼳�� ȣ��
	glGenTextures(12, &textures[0]);//�ѹ��� ȣ����������.

	//==============3ds==================
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
	glewInit();
	// 3ds file loading
	clothInit();
	//=================3ds===============
	
	// set the display callback for the current window
	glutDisplayFunc(drawGraphics);//���÷��� �̺�Ʈ�� ���� �ݹ��Լ��� ���ڰ��� ����.
	glutReshapeFunc(myReshape);//â�� ũ�Ⱑ ���Ҷ� ���ڷ� ���޵� ������ â�� ũ�⸦ ����
	
	glutIdleFunc(myIdle);//�ƹ��͵� ���� ������ ȣ���ϴ� �޼ҵ�, �� �޼ҵ带 ȣ������ ������ �����̴� ���� ���� ���� ����.
	
	glutMainLoop();//�̺�Ʈ���� ȣ�� OpenGL�� ���� �������κ����� ������ �κ�

	/* Memory leaks detecting */
	_CrtDumpMemoryLeaks();

	return 0;
}

/*3DS �� �ʱ�ȭ �޼ҵ�*/
void clothInit(){
	//����
	Suit3DS.Load("3DS/suit/Suite N160211.3DS");

	Suit3DS.scale.x = 0.0009f;
	Suit3DS.scale.y = 0.001f;
	Suit3DS.scale.z = 0.001f;

	Suit3DS.pos.y = -2.8f;
	Suit3DS.pos.z = 0.25f;
	Suit3DS.pos.x = 0.0f;

	//������� ����
	GreenSuit3DS.Load("3DS/suit2/Costume N020410.3ds");
	GreenSuit3DS.scale.x = 0.032f;
	GreenSuit3DS.scale.y = 0.04f;
	GreenSuit3DS.scale.z = 0.03f;

	GreenSuit3DS.pos.x = -0.6f;
	GreenSuit3DS.pos.z = 0.3f;
	GreenSuit3DS.pos.y = -0.3f;

	//���� ����(�� ����)
	GirlShirt.Load("3DS/girlshirt/Suit 2 N240308.3ds");
	GirlShirt.Materials->tex.Load("3DS/girlshirt/flowers_2020130.JPG");
	GirlShirt.scale.x = 0.03f;
	GirlShirt.scale.y = 0.03f;
	GirlShirt.scale.z = 0.03f;

	GirlShirt.pos.z = 0.2f;

	//�ؽ��ĸ� ����(���⹫��)
	GirlShirt.Materials->tex.Load("3DS/girlshirt/fabric_strawberry_motif_9271276.JPG");
}

/*ī�޶� ���� �� �ʱ�ȭ �޼ҵ�*/
void cameraInit()
{
	InitializeKinect();//Ű��Ʈ ���� �ʱ�ȭ
	
	// grab and return a frame from a camera
	iplTexture = cvCreateImage(cvSize(COLOR_WIDTH, COLOR_HEIGHT), IPL_DEPTH_8U, 4);//openCV�Լ��� Ű��Ʈ�� �̹����� ������ ����
	
	cout << endl << "grab a frame!" << endl;//�ܼ�â�� ����� �˷��ش�.
	
	screenW = iplTexture->width;//������ ���� ����
	screenH = iplTexture->height;//������ ���� ����
}

/**OpenGL�� Texture�� Binding��
* OpenGL �ʱ�ȭ �޼ҵ�
*/
void myInit()
{
	glViewport(0, 0, screenW, screenH);//������ â �������� x,y�� ���� ���̰� screenW ���̰� screenH�� ����Ʈ
	glClearColor(1.0, 1.0, 1.0, 0.0);//R,G,B,Alpha(������) GL����� ����
	glColor3f(1.0, 1.0, 0.0);//������ ���� ���ķ� ���� ����

	// create a texture for the video
	glGenTextures(1, &texture); // generate texture names
	glBindTexture(GL_3D_COLOR_TEXTURE, texture); // bind a named texture to a texturing target

	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}


void myIdle()
{
	glutPostRedisplay(); // redisplay the window's normal plane
}

/*
*Ű��Ʈ�� ī�޶� ������ openCV�� �����ͼ� openGL 2d��ü�� ���̰� �� ���� ���� �ؽ��ĸ� �Ѵ�.
*OpenGL��ǥ�迡 �׷��� ����ڿ��� ���������� �����ִ� �޼ҵ�
*/
void drawGraphics()
{
	HANDLE colorStreamHandle;//�÷� ������ ������ �ڵ鷯
	HANDLE nextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//���� ���� ������ ���� �÷�����

	HANDLE nextSkeletonFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//���� ���� ������ ���� ���̷��� ����

	HRESULT hr;//����� ������ ����(���� ������ ����� �������� ture, false�� üũ��
	
	//Ű��Ʈ�� ������� �˻� �ڵ�
	//Ʈ��ŷ Ȱ��ȭ
	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, nextColorFrameEvent, &colorStreamHandle);//���� ������ �������� ���Ͽ� ��Ʈ�� open

	if (FAILED(hr)){//������ ���н�
		cout << "Could not open ImageStream" << endl;
	}

	hr = NuiSkeletonTrackingEnable(nextSkeletonFrameEvent, 0);//���� ���̷��� Ʈ��ŷ�� ��� ���ɿ��� ��ȯ

	if (FAILED(hr)){//��� �Ұ��� �� ��
		cout << "Could not open SkeletonStream" << endl;
	}

	WaitForSingleObject(nextColorFrameEvent, 0);//1��° ���ڰ� set�ɶ����� 2��° ���ڸ�ŭ ��ٸ���. ���� ������ ���ö����� ���
	cr.createRGBImage(colorStreamHandle, iplTexture);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background color

	//����ڰ� �νĵǸ�
	if (user_appreciate == TRUE){
		//���� ����
		if (ad_off == TRUE){
			cvReleaseImage(&adImage);
			ad_reset = TRUE;
			ad_off == FALSE;
		}
		if (history_exit == true && qrcode == false){
			if (currently_clothes_Num == 1){
				Suit3DS.visible = true;
			} else if (currently_clothes_Num == 2){
				GreenSuit3DS.visible = true;
			}
			else if (currently_clothes_Num == 3 || currently_clothes_Num == 4){
				GirlShirt.visible = true;
			}
		}
		//����=======================================���� ������� ����� �����ִ� main View================================
		glMatrixMode(GL_PROJECTION);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		// make a texture
		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// specify a two-dimensional texture image
		// glTexiplTexture2D(<#GLenum target#>, <#GLint level#>, <#GLenum internalformat#>, <#GLsizei width#>, <#GLsizei height#>, <#GLint border#>, <#GLenum format#>, <#GLenum type#>, <#const GLvoid * pixels#>)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iplTexture->width, iplTexture->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, iplTexture->imageData);//<#GLenum format#> 7��° ���ڸ� GL_RGB->GL_BGRA_EXT�� �����Ͽ����� ����
		
		// draw 2D square plane to be mapped by the texture
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);// the drawing color
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-view_size[0], view_size[1], 0.267f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(view_size[0], view_size[1], 0.267f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(view_size[0], -view_size[1], 0.267f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-view_size[0], -view_size[1], 0.267f);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		//��=======================================���� ������� ����� �����ִ� main View================================

		//����=================������� ������ ������ ���� ���� Coner �ν�====================
		if (fitting_first_popup == false){

			glMatrixMode(GL_PROJECTION);
			glBindTexture(GL_TEXTURE_2D, textures[5]);
			// make a texture
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

			// specify a two-dimensional texture image
			// glTexiplTexture2D(<#GLenum target#>, <#GLint level#>, <#GLenum internalformat#>, <#GLsizei width#>, <#GLsizei height#>, <#GLint border#>, <#GLenum format#>, <#GLenum type#>, <#const GLvoid * pixels#>)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, user_guide->width, user_guide->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, user_guide->imageData);//<#GLenum format#>

			// draw 2D square plane to be mapped by the texture
			glEnable(GL_TEXTURE_2D);

			//����ȭ(����)��Ű�� �޼ҵ�
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor3f(1.0f, 1.0f, 1.0f);// the drawing color
			//�����ߴ� �̹���
			glBegin(GL_QUADS);
			{
				glColor4f(1.0f, 1.0f, 1.0f, 0.95f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, 0.7f, 0.3f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.7f, 0.3f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(0.7f, -0.7f, 0.3f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, -0.7f, 0.3f);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

		}
		if ((picture_save_time < 3.0f && fitting_first_popup == true && user_coner_ok == false) || (login == false && qrcode == true)){
			glMatrixMode(GL_PROJECTION);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			if ((login == false && qrcode == true)){
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ui_qrcheck->width, ui_qrcheck->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_qrcheck->imageData);
			}
			else{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ui_check->width, ui_check->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_check->imageData);
			}
			
			
			glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
			{
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex4f(view_size[0] - 0.33f, view_size[1] - 0.67f, 0.3f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex4f(view_size[0] - 0.02f, view_size[1] - 0.67f, 0.3f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex4f(view_size[0] - 0.02f, view_size[1] - 1.09f, 0.3f, 1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex4f(view_size[0] - 0.33f, view_size[1] - 1.09f, 0.3f, 1.0f);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			//���� �ʽð��� 3�������϶��� ��ü
			if (login == false && qrcode == true){
				glMatrixMode(GL_MODELVIEW);
				glBegin(GL_LINE_LOOP);
				{
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex4f(-0.6f, 0.7f, 0.3f, 1.0f);
					glVertex4f(0.6f, 0.7f, 0.3f, 1.0f);
					glVertex4f(0.6f, -0.7f, 0.3f, 1.0f);
					glVertex4f(-0.6f, -0.7f, 0.3f, 1.0f);
				}
				glEnd();
			}
		}
		//��=================������� ������ ������ ���� ���� Coner �ν�====================
		
		//����======================����ڰ� ������ ���� ���� ���� ���ٰ��� ���� �޼ҵ�===============
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, 1.0f, 1.0f, 1000.0f);  // �ְ� ������� ǥ��.
		//��======================����ڰ� ������ ���� ���� ���� ���ٰ��� ���� �޼ҵ�===============

		/*=================================================================================
		|								Main UI �׸��� �κ�								  |
		==================================================================================*/
		if (user_coner_ok == true){//����� ���� ������ ������ ���
			if (login == false && qrcode == true){
				if (currently_clothes_Num == 1){
					Suit3DS.visible = false;
				}
				else if (currently_clothes_Num == 2){
					GreenSuit3DS.visible = false;
				}
				else if (currently_clothes_Num == 3 || currently_clothes_Num == 4){
					GirlShirt.visible = false;
				}
			}
			//�α���, �α׾ƿ� �̹���
			if (login == true && picture_save == false && history_choice == false && qrcode == false){
				//�α׾ƿ� �̹���
				glMatrixMode(GL_PROJECTION);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_logout->width, ui_logout->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_logout->imageData);

				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(-0.184f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(0.066f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(0.066f, view_size[1] - 0.32f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(-0.184f, view_size[1] - 0.32f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			else if (login == false && picture_save == false && history_choice == false && qrcode == false){
				//�α��� �̹���
				glMatrixMode(GL_PROJECTION);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_login->width, ui_login->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_login->imageData);

				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(-0.184f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(0.066f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(0.066f, view_size[1] - 0.32f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(-0.184f, view_size[1] - 0.32f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}

			if (history_choice == true && picture_save == false && qrcode == false){//�����丮 ��ȸ�� ���� ���� ���
				//������ �������� �̹����� ����
				glMatrixMode(GL_PROJECTION);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, histroy_user_save->width, histroy_user_save->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, histroy_user_save->imageData);

				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(-view_size[0] + 0.02f, 0.98f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(-view_size[0] + 0.27f, 0.98f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(-view_size[0] + 0.27f, 0.68f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(-view_size[0] + 0.02f, 0.68f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);

			}
			else if (history_choice == false && picture_save == false && qrcode == false){//�����丮 ��ȸ ������ ���� �ʾ��� ���
				//���ùڽ� �̹����� �����ش�.
				glMatrixMode(GL_PROJECTION);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_cloth_box->width, ui_cloth_box->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_cloth_box->imageData);

				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(-view_size[0] + 0.02f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(-view_size[0] + 0.27f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(-view_size[0] + 0.27f, view_size[1] - 0.32f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(-view_size[0] + 0.02f, view_size[1] - 0.32f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);

			}

			//���������丮�� ���� ����
			if (currently_clothes_Num != 0 && history_choice == false && picture_save == false && qrcode == false){//���� ���� �Ͽ��� �����丮 ��ȸ ������ ���� �ʾ��� ���
				glMatrixMode(GL_PROJECTION);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_save->width, ui_save->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_save->imageData);//<#GLenum format#> 7��° ���ڸ� GL_RGB->GL_BGRA_EXT�� �����Ͽ����� ����

				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(-view_size[0] + 0.02f, view_size[1] - 0.85f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(-view_size[0] + 0.27f, view_size[1] - 0.85f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(-view_size[0] + 0.27f, view_size[1] - 1.15f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(-view_size[0] + 0.02f, view_size[1] - 1.15f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
		

			//�����丮 ��ȸ&�ݱ�
			if (history_exit == true && picture_save == false && qrcode == false){//���� �����丮 ��ȸ�� ������ �ȵ� ����� �����丮 ��ȸ�̹����� ����
				//�����丮 ��ȸ
				glMatrixMode(GL_PROJECTION);
				// make a texture
				// set texture parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_history->width, ui_history->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_history->imageData);

				// draw 2D square plane to be mapped by the texture
				glEnable(GL_TEXTURE_2D);

				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(view_size[0] - 0.27f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(view_size[0] - 0.02f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(view_size[0] - 0.02f, view_size[1] - 0.32f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(view_size[0] - 0.27f, view_size[1] - 0.32f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			else if (history_exit == false && picture_save == false && qrcode == false){//�����丮 ��ȸ�� ���õ� ���� �ݱ� ��ư�̹����� ����
				//�ݱ� �̹���
				glMatrixMode(GL_PROJECTION);
				// make a texture
				// set texture parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hitstory_close->width, hitstory_close->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, hitstory_close->imageData);

				// draw 2D square plane to be mapped by the texture
				glEnable(GL_TEXTURE_2D);

				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 1.0f, 1.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex4f(view_size[0] - 0.27f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex4f(view_size[0] - 0.02f, view_size[1] - 0.02f, 0.3f, 1.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex4f(view_size[0] - 0.02f, view_size[1] - 0.32f, 0.3f, 1.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex4f(view_size[0] - 0.27f, view_size[1] - 0.32f, 0.3f, 1.0f);
				}
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
		}
		/*=================================================================================
		|								Main UI �׸��� �κ�								  |
		==================================================================================*/
		//���� ���� ���� ��
		if (currently_clothes_Num == 1){//����
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glMatrixMode(GL_MODELVIEW);

			float gx = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat glx = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));
			
			float gy = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat gly = -((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));
			
			//������ ȸ����Ų��.
			Suit3DS.rot.y = rotation;

			if (currently_clothes_Num - 130 < 0){//����
				suit_pos_z = ((130 - currently_clothes_Num) / 30)*0.1f;
			}
			Suit3DS.pos.y = gly - 1.15f;
			Suit3DS.pos.x = glx;
			Suit3DS.Draw();
			glDisable(GL_TEXTURE20);
		}
		else if (currently_clothes_Num == 2){//������帶��
			glBindTexture(GL_TEXTURE_2D, textures[8]);
			glMatrixMode(GL_MODELVIEW);

			float gx = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat glx = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));

			float gy = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat gly = -((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));

			GreenSuit3DS.rot.y = rotation;

			GreenSuit3DS.pos.y = gly - 1.3f;
			GreenSuit3DS.pos.x = glx;
			GreenSuit3DS.Draw();
			glDisable(GL_TEXTURE_2D);
		}
		else if (currently_clothes_Num == 3){//���ڼ���(�ɹ���)
			if (textureChange == false){
				GirlShirt.Materials->tex.Load("3DS/girlshirt/flowers_2020130.JPG");
				textureChange = true;
			}
			glBindTexture(GL_TEXTURE_2D, textures[9]);
			glMatrixMode(GL_MODELVIEW);

			float gx = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat glx = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));

			float gy = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat gly = -((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));

			GirlShirt.rot.y = rotation-90.0f;

			GirlShirt.pos.y = gly - 0.6f;
			GirlShirt.pos.x = glx;
			GirlShirt.Draw();
			glDisable(GL_TEXTURE_2D);
		}
		else if (currently_clothes_Num == 4){//���ڼ���(���⹫��)
			//�ؽ��� ����
			if (textureChange == true){
				GirlShirt.Materials->tex.Load("3DS/girlshirt/fabric_strawberry_motif_9271276.JPG");
				textureChange = false;
			}
			
			glBindTexture(GL_TEXTURE_2D, textures[9]);
			glMatrixMode(GL_MODELVIEW);

			float gx = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat glx = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));

			float gy = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat gly = -((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));

			GirlShirt.rot.y = rotation-90.f;

			GirlShirt.pos.y = gly - 0.6f;
			GirlShirt.pos.x = glx;
			GirlShirt.Draw();
			glDisable(GL_TEXTURE_2D);
		}

		//���������丮 ��ȸ ���ý�
		if (history_choice == true && history_exit==false){
			if (currently_clothes_Num == 1){ Suit3DS.visible = false; }
			else if (currently_clothes_Num == 2){ GreenSuit3DS.visible = false; }
			else if (currently_clothes_Num == 3 || currently_clothes_Num == 4){ GirlShirt.visible = false; }

			//glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D, textures[2]);
			glMatrixMode(GL_PROJECTION);
			// make a texture
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// specify a two-dimensional texture image
			// glTexiplTexture2D(<#GLenum target#>, <#GLint level#>, <#GLenum internalformat#>, <#GLsizei width#>, <#GLsizei height#>, <#GLint border#>, <#GLenum format#>, <#GLenum type#>, <#const GLvoid * pixels#>)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewNowImage->width, viewNowImage->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, viewNowImage->imageData);//<#GLenum format#> 7��° ���ڸ� GL_RGB->GL_BGRA_EXT�� �����Ͽ����� ����

			// draw 2D square plane to be mapped by the texture
			glEnable(GL_TEXTURE_2D);
			
			//����ȭ(����)��Ű�� �޼ҵ�
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor3f(1.0f, 1.0f, 1.0f);// the drawing color

			//�����ߴ� �̹���
			glBegin(GL_QUADS);
			{
				glColor4f(1.0f, 1.0f, 1.0f, 0.95f);
				glTexCoord2f(0.0f, 0.0f); glVertex4f(-0.7f, 0.7f, 0.3f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex4f(0.7f, 0.7f, 0.3f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex4f(0.7f, -0.7f, 0.3f, 1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex4f(-0.7f, -0.7f, 0.3f, 1.0f);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			glMatrixMode(GL_MODELVIEW);
			if (history_left_click == TRUE){
				//���� ȭ��ǥ
				glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(-0.7f, -0.2f, 0.3f);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(-0.7f, 0.2f, 0.3f);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(-0.8f, 0.0f, 0.3f);
				glEnd();
			}
			
			glMatrixMode(GL_MODELVIEW);
			if (history_right_click == TRUE){
				//������ ȭ��ǥ
				glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(0.7f, -0.2f, 0.3f);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(0.7f, 0.2f, 0.3f);

				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(0.8f, 0.0f, 0.3f);
				glEnd();
			}
		}
	}
	else if(user_appreciate == FALSE){//�ν��� �ȵǸ� ���� �����ش�.(����ڰ� ���� ���� ���õ� ���Ե�)
		if (ad_on == TRUE){
				adImage = cvLoadImage("./Image/ready.png");
				ad_on = FALSE;
				ad_off = TRUE;
				ad_reset == FALSE;
		}

		glMatrixMode(GL_PROJECTION);
		
		glBindTexture(GL_TEXTURE_2D, textures[6]);
		// make a texture
		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// specify a two-dimensional texture image
		// glTexiplTexture2D(<#GLenum target#>, <#GLint level#>, <#GLenum internalformat#>, <#GLsizei width#>, <#GLsizei height#>, <#GLint border#>, <#GLenum format#>, <#GLenum type#>, <#const GLvoid * pixels#>)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, adImage->width, adImage->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, adImage->imageData);//<#GLenum format#> 7��° ���ڸ� GL_RGB->GL_BGRA_EXT�� �����Ͽ����� ����

		// draw 2D square plane to be mapped by the texture
		glEnable(GL_TEXTURE_2D);

		glColor3f(1.0f, 1.0f, 1.0f);// the drawing color

		//�����ߴ� �̹���
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.3f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.3f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.3f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.3f);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		if (ad_reset == FALSE){
			glLoadIdentity();
		}
	}//�� �νĿ��� if-else()
	glutSwapBuffers(); // swap framebuffer 	
}


void myKeyboard(unsigned char key, int mousex, int mousey)
{
	switch (key) {
	case 27: // Press ESC to exit
		exit(0);
	}
}

void myReshape(int width, int height)
{
	glMatrixMode(GL_PROJECTION);//������� ����(3D������ ī�޶� ������ ���������� ǥ���� ���)
	glLoadIdentity();//��������� ���Ѵ�. ��� �ʱ�ȭ
	gluPerspective(60.0f, 1.0f, 1.0f, 1000.0f);  // �ְ� ������� ǥ��.
	glViewport(0, 0, width, height);
}

/*
Ű��Ʈ �� �ʱ�ȭ ����
*/
void InitializeKinect(){
	bool FailToConnect;//false = ���Ἲ��, true = �������

	do{
		HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX);//Ű��Ʈ�� �÷��� ���̷����� ����ϰڴٴ� ����

		if (FAILED(hr)){//���� ����� false��
			system("cls");
			cout << "\nFailed to Connect!\n\n";
			FailToConnect = true;//���� ����
			system("PAUSE");
		}
		else{
			cout << "\nConnection Established!\n\n";
			FailToConnect = false;//���� ����
		}
	} while (FailToConnect);//���� �����Ҷ����� while()�� ����
}