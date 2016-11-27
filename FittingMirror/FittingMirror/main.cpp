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

extern CvPoint points[NUI_SKELETON_POSITION_COUNT];//스켈레톤의 총 관절갯수인 20개만큼 배열을 만듬
//CvCapture* capture;//카메라로 부터 프레임을 받아옴
extern IplImage* iplTexture = 0;//OpenCV 변수 카메라로부터 받은 프레임을 관리하는 변수
GLuint texture;  // array in which the generated texture names are stored

//영상의 사이즈
int screenW;
int screenH;

//카메라가 바라보는 좌표의 위치값
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 2.0f;

//동적 view size 조정을 위한 변수
long double view_size[2] = { 1.0f, 1.0f };

CreateRGBImage cr;//영상 클래스 선언


extern bool history_choice;//히스토리 조회 선택 여부 flag
int upclothsx, upclothsy;

extern bool user_appreciate;//skeleton의 전역변수로 사용자의 인식여부를 체크하는 flag
extern bool ad_on, ad_off;//광고 on/off flag

//히스토리 조회 사진 flag
extern bool history_left_click;
extern bool history_right_click;

// 3ds file loader class
Model_3DS	Suit3DS;
Model_3DS   GreenSuit3DS;
Model_3DS   GirlShirt;

//GL 텍스쳐 처리를 위한 배열
GLuint textures[12];

float rotation = 0.0f;//도형 회전각 설정

extern bool fitting_first_popup;//팝업 최초 실행 flag
extern bool history_exit;//히스토리조회 닫기 flag

IplImage *viewNowImage;//저장된 사진 보여주는 변수
IplImage *user_guide = cvLoadImage("./Image/user_guide.JPG");//가이드라인 팝업창 이미지
IplImage *adImage = cvLoadImage("./Image/ready.png");//광고 이미지

//피팅 히스토리 조회 선택시 보여줄 UI이미지
IplImage *hitstory_close = cvLoadImage("./Image/close.png");
IplImage *histroy_user_save = cvLoadImage("./Image/cloud.png");

//main UI 이미지
IplImage* ui_save = cvLoadImage("./Image/save.png");
IplImage* ui_history = cvLoadImage("./Image/history.png");
IplImage* ui_cloth_box = cvLoadImage("./Image/fittingBox.png");
IplImage* ui_login = cvLoadImage("./Image/login.png");//로그인 이미지
IplImage* ui_logout = cvLoadImage("./Image/logout.png");//로그아웃 이미지

//측정 UI 이미지
IplImage* ui_check = cvLoadImage("./Image/check.png");
IplImage* ui_qrcheck = cvLoadImage("./Image/QRCheck.png");

extern bool user_coner_ok;//코너 측정 완료 flag
extern bool picture_save;//사진 저장 flag
extern bool login;//로그인 flag
extern bool qrcode;//QR코드 측정 시작 flag
extern float picture_save_time;//사진 저장 시간
extern int currently_clothes_Num;//현재 선택된 옷의 넘버

bool textureChange = false;//옷 텍스쳐 변경 flag
bool ad_reset = false;//광고 재로드시

extern CvPoint conner_points[NUI_SKELETON_POSITION_COUNT];//검출된 코너 좌표 배열
extern int user_hip, user_shoulder_top, user_shoulder_max ;//측정되어 정제된 값

//옷의 회전 값 및 x,y,z 설정 변수
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

//실행 프로시저
int main(int argc, char * argv[])
{
	// initialize glut
	glutInit(&argc, argv);//OpenGL 사용을 위한 초기함수
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//창 출력영상 모드 설정

	cameraInit();//카메라 초기화

	// initialize the window
	glutInitWindowSize(screenW, screenH);//프로그램 창 크기
	glutInitWindowPosition(100, 100);//좌상단을 기점으로 나타날 위치를 지정
	glutCreateWindow("Fitting Mirror");//제목이름

	myInit();//초기설정 호출
	glGenTextures(12, &textures[0]);//한번만 호출시켜줘야함.

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
	glutDisplayFunc(drawGraphics);//디스플레이 이벤트에 대한 콜백함수로 인자값을 설정.
	glutReshapeFunc(myReshape);//창의 크기가 변할때 인자로 전달된 값으로 창의 크기를 변경
	
	glutIdleFunc(myIdle);//아무것도 하지 않을때 호출하는 메소드, 이 메소드를 호출하지 않으면 움직이는 것이 실행 되지 않음.
	
	glutMainLoop();//이벤트루프 호출 OpenGL의 가장 마지막부분으로 끝나는 부분

	/* Memory leaks detecting */
	_CrtDumpMemoryLeaks();

	return 0;
}

/*3DS 옷 초기화 메소드*/
void clothInit(){
	//정장
	Suit3DS.Load("3DS/suit/Suite N160211.3DS");

	Suit3DS.scale.x = 0.0009f;
	Suit3DS.scale.y = 0.001f;
	Suit3DS.scale.z = 0.001f;

	Suit3DS.pos.y = -2.8f;
	Suit3DS.pos.z = 0.25f;
	Suit3DS.pos.x = 0.0f;

	//녹색정장 마이
	GreenSuit3DS.Load("3DS/suit2/Costume N020410.3ds");
	GreenSuit3DS.scale.x = 0.032f;
	GreenSuit3DS.scale.y = 0.04f;
	GreenSuit3DS.scale.z = 0.03f;

	GreenSuit3DS.pos.x = -0.6f;
	GreenSuit3DS.pos.z = 0.3f;
	GreenSuit3DS.pos.y = -0.3f;

	//여자 셔츠(꽃 무늬)
	GirlShirt.Load("3DS/girlshirt/Suit 2 N240308.3ds");
	GirlShirt.Materials->tex.Load("3DS/girlshirt/flowers_2020130.JPG");
	GirlShirt.scale.x = 0.03f;
	GirlShirt.scale.y = 0.03f;
	GirlShirt.scale.z = 0.03f;

	GirlShirt.pos.z = 0.2f;

	//텍스쳐만 변경(딸기무늬)
	GirlShirt.Materials->tex.Load("3DS/girlshirt/fabric_strawberry_motif_9271276.JPG");
}

/*카메라 연결 및 초기화 메소드*/
void cameraInit()
{
	InitializeKinect();//키넥트 설정 초기화
	
	// grab and return a frame from a camera
	iplTexture = cvCreateImage(cvSize(COLOR_WIDTH, COLOR_HEIGHT), IPL_DEPTH_8U, 4);//openCV함수로 키넥트의 이미지를 관리할 변수
	
	cout << endl << "grab a frame!" << endl;//콘솔창에 결과를 알려준다.
	
	screenW = iplTexture->width;//영상의 가로 길이
	screenH = iplTexture->height;//영상의 세로 길이
}

/**OpenGL에 Texture를 Binding함
* OpenGL 초기화 메소드
*/
void myInit()
{
	glViewport(0, 0, screenW, screenH);//윈도우 창 기준으로 x,y로 부터 넓이가 screenW 높이가 screenH인 뷰포트
	glClearColor(1.0, 1.0, 1.0, 0.0);//R,G,B,Alpha(투영도) GL배경의 색깔
	glColor3f(1.0, 1.0, 0.0);//정점의 색깔 그후로 서로 섞임

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
*키넥트의 카메라 영상을 openCV로 가져와서 openGL 2d객체로 붙이고 그 위에 도형 텍스쳐를 한다.
*OpenGL좌표계에 그려서 사용자에게 실질적으로 보여주는 메소드
*/
void drawGraphics()
{
	HANDLE colorStreamHandle;//컬러 영상을 제어할 핸들러
	HANDLE nextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//현재 영상 다음에 붙일 컬러영상

	HANDLE nextSkeletonFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//현재 영상 다음에 붙일 스켈레톤 영상

	HRESULT hr;//결과를 저장할 변수(현재 영상이 사용이 가능한지 ture, false값 체크용
	
	//키넥트의 연결상태 검사 코드
	//트랙킹 활성화
	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, nextColorFrameEvent, &colorStreamHandle);//현재 영상을 가져오기 위하여 스트림 open

	if (FAILED(hr)){//연결이 실패시
		cout << "Could not open ImageStream" << endl;
	}

	hr = NuiSkeletonTrackingEnable(nextSkeletonFrameEvent, 0);//현재 스켈레톤 트래킹의 사용 가능여부 반환

	if (FAILED(hr)){//사용 불가능 할 시
		cout << "Could not open SkeletonStream" << endl;
	}

	WaitForSingleObject(nextColorFrameEvent, 0);//1번째 인자가 set될때까지 2번째 인자만큼 기다린다. 다음 영상이 들어올때까지 대기
	cr.createRGBImage(colorStreamHandle, iplTexture);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background color

	//사용자가 인식되면
	if (user_appreciate == TRUE){
		//광고 종료
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
		//시작=======================================현재 사용자의 모습을 보여주는 main View================================
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iplTexture->width, iplTexture->height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, iplTexture->imageData);//<#GLenum format#> 7번째 인자를 GL_RGB->GL_BGRA_EXT로 변경하였더니 됬음
		
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
		//끝=======================================현재 사용자의 모습을 보여주는 main View================================

		//시작=================사용자의 사이즈 측정을 위한 최초 Coner 인식====================
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

			//투명화(블렌드)시키는 메소드
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor3f(1.0f, 1.0f, 1.0f);// the drawing color
			//저장했던 이미지
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

			//추후 초시간이 3초이하일때로 대체
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
		//끝=================사용자의 사이즈 측정을 위한 최초 Coner 인식====================
		
		//시작======================사용자가 가상의 옷을 피팅 시의 원근감을 위한 메소드===============
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, 1.0f, 1.0f, 1000.0f);  // 멀고 가까움을 표현.
		//끝======================사용자가 가상의 옷을 피팅 시의 원근감을 위한 메소드===============

		/*=================================================================================
		|								Main UI 그리는 부분								  |
		==================================================================================*/
		if (user_coner_ok == true){//사용자 최초 측정이 끝났을 경우
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
			//로그인, 로그아웃 이미지
			if (login == true && picture_save == false && history_choice == false && qrcode == false){
				//로그아웃 이미지
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
				//로그인 이미지
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

			if (history_choice == true && picture_save == false && qrcode == false){//히스토리 조회를 선택 했을 경우
				//계정에 사진저장 이미지로 변경
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
			else if (history_choice == false && picture_save == false && qrcode == false){//히스토리 조회 선택을 하지 않았을 경우
				//피팅박스 이미지를 보여준다.
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

			//피팅히스토리에 사진 저장
			if (currently_clothes_Num != 0 && history_choice == false && picture_save == false && qrcode == false){//옷을 착용 하였고 히스토리 조회 선택을 하지 않았을 경우
				glMatrixMode(GL_PROJECTION);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_save->width, ui_save->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ui_save->imageData);//<#GLenum format#> 7번째 인자를 GL_RGB->GL_BGRA_EXT로 변경하였더니 됬음

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
		

			//히스토리 조회&닫기
			if (history_exit == true && picture_save == false && qrcode == false){//만약 히스토리 조회가 선택이 안된 경우라면 히스토리 조회이미지로 변경
				//히스토리 조회
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
			else if (history_exit == false && picture_save == false && qrcode == false){//히스토리 조회가 선택된 경우면 닫기 버튼이미지로 변경
				//닫기 이미지
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
		|								Main UI 그리는 부분								  |
		==================================================================================*/
		//옷이 선택 됬을 때
		if (currently_clothes_Num == 1){//정장
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glMatrixMode(GL_MODELVIEW);

			float gx = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat glx = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));
			
			float gy = (float)((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
			GLfloat gly = -((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));
			
			//도형을 회전시킨다.
			Suit3DS.rot.y = rotation;

			if (currently_clothes_Num - 130 < 0){//음수
				suit_pos_z = ((130 - currently_clothes_Num) / 30)*0.1f;
			}
			Suit3DS.pos.y = gly - 1.15f;
			Suit3DS.pos.x = glx;
			Suit3DS.Draw();
			glDisable(GL_TEXTURE20);
		}
		else if (currently_clothes_Num == 2){//녹색정장마이
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
		else if (currently_clothes_Num == 3){//여자셔츠(꽃무늬)
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
		else if (currently_clothes_Num == 4){//여자셔츠(딸기무늬)
			//텍스쳐 설정
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

		//피팅히스토리 조회 선택시
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewNowImage->width, viewNowImage->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, viewNowImage->imageData);//<#GLenum format#> 7번째 인자를 GL_RGB->GL_BGRA_EXT로 변경하였더니 됬음

			// draw 2D square plane to be mapped by the texture
			glEnable(GL_TEXTURE_2D);
			
			//투명화(블렌드)시키는 메소드
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor3f(1.0f, 1.0f, 1.0f);// the drawing color

			//저장했던 이미지
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
				//왼쪽 화살표
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
				//오른쪽 화살표
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
	else if(user_appreciate == FALSE){//인식이 안되면 광고 보여준다.(사용자가 임의 제거 선택도 포함됨)
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, adImage->width, adImage->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, adImage->imageData);//<#GLenum format#> 7번째 인자를 GL_RGB->GL_BGRA_EXT로 변경하였더니 됬음

		// draw 2D square plane to be mapped by the texture
		glEnable(GL_TEXTURE_2D);

		glColor3f(1.0f, 1.0f, 1.0f);// the drawing color

		//저장했던 이미지
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
	}//고객 인식여부 if-else()
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
	glMatrixMode(GL_PROJECTION);//투영모드 설정(3D공간에 카메라 설정을 수학적으로 표현한 행렬)
	glLoadIdentity();//단위행렬을 곱한다. 행렬 초기화
	gluPerspective(60.0f, 1.0f, 1.0f, 1000.0f);  // 멀고 가까움을 표현.
	glViewport(0, 0, width, height);
}

/*
키넥트 및 초기화 설정
*/
void InitializeKinect(){
	bool FailToConnect;//false = 연결성공, true = 연결실패

	do{
		HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX);//키넥트의 컬러와 스켈레톤을 사용하겠다는 변수

		if (FAILED(hr)){//연결 결과가 false시
			system("cls");
			cout << "\nFailed to Connect!\n\n";
			FailToConnect = true;//연결 실패
			system("PAUSE");
		}
		else{
			cout << "\nConnection Established!\n\n";
			FailToConnect = false;//연결 성공
		}
	} while (FailToConnect);//연결 성공할때까지 while()문 돌림
}