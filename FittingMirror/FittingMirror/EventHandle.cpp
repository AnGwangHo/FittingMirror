// 헤더파일 참조
#include "EventHandle.h"
#include "HistoryView.h"
#include <direct.h>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

// 스켈레톤의 20개 관절포인트 선언
extern CvPoint points[NUI_SKELETON_POSITION_COUNT];

extern int Suit_Num, Dress_Num, GreenSuit_Num, GirlShirt1_Num, GirlShirt2_Num;
// 왼쪽 손 체크, 옷 체크, 옷 이벤트처리_체크, 옷 이벤트처리_착용에 사용할 변수 선언 및 초기화
extern bool left_Hand_Check, clothes_Check, clothes_EventHandle_Check, clothes_EventHandle_Put_On;
// 현재 옷의 번호, 옷의 총 갯수
extern int currently_clothes_Num, clothes_Total_Num;

// 옷선택시 체크, 상의 선택 체크, 하의 선택 체크, 종료 선택, 이전 목록선택, 이후 목록선택
bool cloth_choice, cloth_up_choice, cloth_down_choice, cloth_close, cloth_before, cloth_after;
int cloth_index;//선택된 옷의 인덱스

// 사진저장 체크
bool picture_save;

// 히스토리 체크, 내 계정에 저장하기, 이전사진, 이후사진, 종료선택
bool history_choice, history_exit=true;
bool history_save, history_before, history_after;
bool history_chocie_ok = false;

// 히스토리에 보여줄 이미지의 배열
extern int history_picture_index;
extern int upclothsx, upclothsy;

// 클래스 객체 생성
CvOverlayImage coi;
HistoryView hv;

//영상 저장 관련 변수
string picture_nameset;
string picture_save_path = "D:/picture/";
time_t curr_time;
struct tm *curr_tm;
bool picture_save_time_check;
float picture_save_time;
clock_t picture_begin_time;

//히스토리 왼쪽 오른쪽 선택여부
extern bool history_left_click;
extern bool history_right_click;

//히스토리 조회시 사진 보여주는 이미지 변수
extern IplImage *viewNowImage;

extern bool user_guide_ok_btn;//사용자가 유저가이드를 확인 선택 시 flag
extern bool user_guide_coner_btn;//사용자가 측정 버튼을 선택 하였는지 flag
bool user_coner_ok = false;//사용자 경계 추출 완료 flag

//검출된 외곽선을 저장하기 위한 메모리 스토리지
IplImage *testtemppicture;
IplImage *Gray = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
IplImage *findCanny = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
CvMemStorage *storage = cvCreateMemStorage(0);
CvSeq *contour = 0;

extern USHORT depths[NUI_SKELETON_POSITION_COUNT];//사용자 DepthMap Data 배열
extern long double view_size[2];//영상 사이즈

//리사이징 x,y 증가계
float increaseWidth = 0.0578f;
float increaseHeight = 0.05764f;
extern float cameraZ;//영상을 바라보는 시점
extern float rotation;//옷 회전각도


//왼쪽, 오른쪽 어깨 추적상황
extern bool SKELETON_POSITION_SHOULDER_RIGHT;
extern bool SKELETON_POSITION_SHOULDER_LEFT;

//사용자 최초 인증 flag
bool fitting_first_popup_ok = false;
bool fitting_first_popup = false;

//피팅 히스토리에 저장된 사진의 갯수
extern int indexsize;

//코너 검출을 위한 현재영상 복사
IplImage *connercopy;

//계정에 사진 저장을 위한 list 참조선언
extern vector<string> list;

//로그인 관련 flag
bool login = false;//로그인 UI선택 유무
bool qrcode = false;//qr코드 인증 여부
bool qrcode_check = false;//qr코드 측정 여부 확인
bool logout = false;
extern char *user_id;
extern char *user_name;
char *test_id=NULL;
string str_user_id;
//사용자 최초 신체측정시의 관절값, 신체 사이즈
extern CvPoint conner_points[NUI_SKELETON_POSITION_COUNT];
extern int user_hip, user_shoulder_top, user_shoulder_max;

void ScreenCapture(const char *strFilePath);//현재 영상 캡쳐 메소드

// 옷 이미지를 오른손으로 드래그 할 경우 이벤트 처리
void EventHandle::right_Hand_Clothes_Handler(bool clothes_handler_check, int x, int y, IplImage *skeleton){

	// 오른손이 옷 영역안에 들어갔을 경우
	if (clothes_handler_check == true){

		// 아직 옷을 착용하지 않았고, 옷 드래그 활성화를 한 오른손이 사용자의 목 관절 영역안에 들어가면 옷을 착용할 준비가 되었음을 확인한다.
		if (clothes_EventHandle_Put_On == false &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - 30) &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x + 30) &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - 30) &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y + 30)){
			// 사용자가 옷을 입었다는 것을 확인
			clothes_EventHandle_Put_On = true;
		}
		//  옷을 착용
		else if (clothes_EventHandle_Put_On == true){

			//여기에 어깨의 위에 맞게 y좌표 수정
			upclothsx = points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x;
			upclothsy = points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y;
		}
		// 오른손으로 옷을 잡았지만 아직 착용하지 않았을 경우
		else {
			upclothsx = x;
			upclothsy = y;
		}
	}
	// 오른손이 옷 영역안에 들어가지 않았을 경우
	else if (clothes_handler_check == false){
		//옷을 처음 선택시 옷이 나타날 초기값 x,y값 위치 설정
		upclothsx = x;
		upclothsy = y;
	}
}

// 핸드트래킹(스켈레톤으로 인식되는 사람이 있다면 왼쪽, 오른쪽 손 관절을 추적하여 해당 이벤트를 처리한다.)
void EventHandle::handtracking(IplImage *skeleton){

	// 스켈레톤 관절의 좌표를 담을 정수형 변수(옷 이미지를 띄울 좌표로 사용할 변수)
	int x, y;

	// 스켈레톤으로 인식되는 사람이 있다면
	if (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x != 0 && points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y != 0){
		
		//리사이징 기능(View 시점 조정)
		if (depths[NUI_SKELETON_POSITION_SHOULDER_CENTER] >= 15760 && depths[NUI_SKELETON_POSITION_SHOULDER_CENTER] < 25760){
			//최대로 뒤로가면 0, 최소로 맨앞이면 100
			long double temp = (long double)((long double)(25760 - depths[NUI_SKELETON_POSITION_SHOULDER_CENTER]) / 10000) * 10;
			cameraZ = (float)(10.0f-temp)*0.1f + 2.0f;
			
			view_size[0] = (long double)(((long double)(10.0f - temp)*0.0578) + 1.0f);
			view_size[1] = (long double)(((long double)(10.0f - temp)*0.05764) + 1.0f);
		}
		else if (depths[NUI_SKELETON_POSITION_SHOULDER_CENTER] < 15760){
			cameraZ = 2.0f;
			view_size[0] = 1.0f;
			view_size[1] = 1.0f;
		}
		else{
			cameraZ = 3.0f;
			view_size[0] = 1.578f;
			view_size[1] = 1.5764f;
		}

		//경계값 추출이 완료 됬을 경우
		if (user_coner_ok == TRUE){
			//사용자가 옷을 착용했을 경우 회전기능을 위한 if문
			if (currently_clothes_Num != 0){//옷이 선택이 되었다면
				//상수 지정값 : rotation에서 -,+하는 값들
				//추적상태는 전역변수로 사용(Skeleton.cpp에서 전역변수 2개 선언)
				if (SKELETON_POSITION_SHOULDER_LEFT == true && SKELETON_POSITION_SHOULDER_RIGHT == false){//왼쪽 어깨를 앞, 오른쪽 어깨 뒤(우회전)
					//(최초 측정치의값*z에따른 비율)/2-(현재 왼쪽-가운데)/90*90
					rotation = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x) / 90.0f)*90.0f-20.0f;
				}
				else if (SKELETON_POSITION_SHOULDER_LEFT == false && SKELETON_POSITION_SHOULDER_RIGHT == true){//오른쪽 어깨를 앞, 왼쪽 어깨 뒤(좌회전)
					rotation = -((points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x - points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x) / 90.0f)*90.0f-10.0f;//300.0f-완전 90
				}
				else{//정면 일때 살짝만 틀었을때
					rotation = 0.0f;
				}//if-else if()-else
			}//if

			// 왼쪽 손 관절이 일정 x,y 좌표의 범위밖에 있을 경우의 해당 되는 이벤트 처리
			if (qrcode == false && picture_save == false && history_choice == false && cloth_choice == false && left_Hand_Check == true
				&& !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80 && points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0) && (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72	&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){

				// 옷 체크를 검사(오른손으로 옷을 드래그했는지 여부를 검사함. 오른손을 대지 않은 기본 값은 "false"임)
				clothes_Check = false;

				// 오른손으로 옷이미지를 건드렸을 경우(일정 x,y 좌표 범위를 설정 후 오른손 관절이 해당 영역으로 들어갈 때 이벤트 처리)
				if (clothes_EventHandle_Check == false &&
					points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 500 && points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 220)
				{
					// 오른손으로 옷을 드래그한 것을 활성화시킨다.
					clothes_EventHandle_Check = true;
				}
				// 오른손 옷 드래그가 비활성화 되있을 경우
				else if (clothes_EventHandle_Check == false){
					x = 500;
					y = 220;
					right_Hand_Clothes_Handler(clothes_EventHandle_Check, x, y, skeleton);
				}
				// 오른손으로 옷을 드래그한 것을 활성화한 경우에 else문을 실행한다.
				else
				{
					x = points[NUI_SKELETON_POSITION_HAND_RIGHT].x;
					y = points[NUI_SKELETON_POSITION_HAND_RIGHT].y;
					// 옷 착용 이벤트 처리 함수
					right_Hand_Clothes_Handler(clothes_EventHandle_Check, x, y, skeleton);
				}
			}
			// 왼쪽 손 관절이 일정 x,y 좌표의 일정 범위 안에 존재한다면 옷을 교환할 준비를 한다.
			else if (qrcode == false && picture_save == false && history_choice == false && cloth_choice == false && clothes_Check == false && points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0){

				// 옷 변수 초기화
				left_Hand_Check = true;
				clothes_Check = true;
				clothes_EventHandle_Check = false;
				clothes_EventHandle_Put_On = false;

				// 옷 교환 
				if (currently_clothes_Num == 0)
				{
					currently_clothes_Num = Suit_Num;
				}
				else if (currently_clothes_Num == 1)
				{
					currently_clothes_Num = GreenSuit_Num;
				}
				else if (currently_clothes_Num == 2){
					currently_clothes_Num = GirlShirt1_Num;
				}
				else if (currently_clothes_Num == 3){
					currently_clothes_Num = GirlShirt2_Num;
				}
				else if (currently_clothes_Num == 4){
					currently_clothes_Num = 0;
				}
			}

			//로그인 선택 시
			if (history_choice == false && picture_save == false && login == false && qrcode == false && ((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 340
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 261
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){
				qrcode = true;
			}
			//로그인 선택 후 손이 밖으로 나왔을 경우
			else if (history_choice == false && picture_save == false && login == false && qrcode == true && !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 340
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 261
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){

				//QR코드 측정버튼 선택 시
				if (qrcode_check == false && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 540)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 260
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 160))){
					qrcode_check = true;
					picture_begin_time = clock();//그 벗어난 순간의 시간을 저장한다.
					picture_save_time_check = TRUE;
				}
				//QR코드 측정버튼 선택 후 손이 밖으로 나왔을 경우
				else if (qrcode_check == true && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 540)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 260
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 160))){

					if (login == false){//로그인이 안되었을 경우에만
						//QR코드 측정 메소드
						picture_save_time = float(clock() - picture_begin_time) / CLOCKS_PER_SEC;//QR코드 인식시킬 시간을 체크
						if (picture_save_time_check == TRUE){//벗어나는 그 최초의 순간만을 체크한다.
							if (picture_save_time >= 5.0f){//5초이상 지났을 경우에 QR코드 체크
								
								//사진을 저장후
								IplImage* temp = cvCloneImage(skeleton);
								cvSetImageROI(temp, cvRect(125, 65, 390, 350));
								picture_nameset = "QRCode.jpg";
								cvSaveImage(picture_nameset.c_str(), temp);
								cvReleaseImage(&temp);

								//QR코드 분석 후 서버통신
								system("java -jar QR.jar");

								char inputString[1024];

								//서버로부터 로그인 결과 파일 읽어오기
								ifstream inFile("./java/ServerResponse.txt");

								while (!inFile.eof())
								{
									inFile.getline(inputString, 100);
									cout << inputString << endl;
								}
								inFile.close();

								//사용자 아이디를 문자열로 부터 추출
								user_id = strtok_s(inputString, "=", &user_name);
								str_user_id = user_id;

								picture_save_time_check = FALSE;//시간 체크 flag 초기화
								qrcode_check = false;
								qrcode = false;
								login = true;
							}
							else{
								//3,2,1 하고 이벤트표현
							}//if-else()
						}//if
					}//if
				}//if-else if()
			}//if-else if()
			
			//로그아웃 이벤트처리
			if (login == true && logout == false && ((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 340
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 261
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){
				logout = true;
			}
			else if (login == true && logout == true && !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 340
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 261
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){
				login = false;
				logout = false;
			}

			//사진 저장 선택 시
			if (qrcode == false && currently_clothes_Num != 0 && history_choice == false && picture_save == false &&
				((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 275
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 204))){
				picture_begin_time = clock();//그 벗어난 순간의 시간을 저장한다.
				picture_save_time_check = TRUE;
				picture_save = true;
			}
			else if (currently_clothes_Num != 0 && history_choice == false && picture_save == true &&
				!((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0)
				&& (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 275
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 204))){
			
				picture_save_time = float(clock() - picture_begin_time) / CLOCKS_PER_SEC;//포즈잡을 시간을 체크

				if (picture_save_time_check == TRUE){//벗어나는 그 최초의 순간만을 체크한다.
					if (picture_save_time >= 3.0f){//3초이상 지났을 경우에 사진저장
						curr_time = time(NULL);
						curr_tm = localtime(&curr_time);
						picture_nameset = picture_save_path + to_string(curr_tm->tm_mday) + "d" + to_string(curr_tm->tm_hour) + "h" + to_string(curr_tm->tm_min) + "m" + to_string(curr_tm->tm_sec) + ".jpg";
						ScreenCapture(picture_nameset.c_str());
						picture_save = false;
						picture_save_time_check = FALSE;//시간 체크 flag 초기화
					}
				}
			}
				
			
			//히스토리 선택 시
			if (qrcode == false && picture_save == false && history_choice == false && history_exit == true && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 558)
				&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 0))){
			
				hv.createHistoryView();
				history_choice = true;
			}
			else if (history_choice == true && history_exit == true && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 558)
				&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 0))){
			
				//히스토리 선택시 사진이 있는 경우에만 보여준다.
				if (indexsize != 0){
					history_exit = false;
					cloth_choice = true;
				}
				else{//사진이 없는 경우
					history_choice = false;
				}
			
			}
		
			//닫기 버튼 클릭시
			if (qrcode == false && picture_save == false && history_choice == true && history_exit == false && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 558)
				&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 0))){
				history_choice = false;
			}
			else if (picture_save == false && history_choice == false && history_exit == false && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 558)
				&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 0))){
				history_exit = true;
				
				cloth_choice = false;
				cvReleaseImage(&viewNowImage);
			}

			//닫기 버튼을 안했을 시
			if (qrcode == false && picture_save == false && history_choice == true && history_exit == false && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 558)
				&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 0))){

				//왼쪽 버튼클릭시
				if (history_left_click == true && history_before == false && ((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 70
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 20)
					&& (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 290
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 190))){
					history_before = true;
				
				}
				else if (history_before == true && !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 70
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 20)
					&& (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 290
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 190))){
					history_before = false;
					//배열에서 index 값에 따른 이미지 변경시키는 메소드
					hv.moveImage(0);
				}

				//오른쪽 버튼클릭시
				if (history_right_click == true && history_after == false && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 620
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 570)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 290
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 190))){
					history_after = true;
				}
				else if (history_after == true && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 620
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 570)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 290
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 190))){
					history_after = false;
					//배열에서 index 값에 따른 이미지 변경시키는 메소드
					hv.moveImage(1);
				}


				//서버에 사진저장버튼 클릭시
				if (history_save == false && ((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0)
					&& (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){
					history_save = true;

				}
				else if (history_save == true && !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0)
					&& (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
					&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){
					history_save = false;
					//서버에 보낼때 인자값 = (현재 보고있는 이미지, 내 계정정보)
					hv.saveServerImageUser();
				}
			}
		}
		else{//아직 측정을 안했다면
			//맨 처음 가이드 팝업을 클릭 완료시
			if (fitting_first_popup == false){
				//서버에 사진저장버튼 클릭시
				if (fitting_first_popup_ok == false && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 360
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 280)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 280
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 240))){
					fitting_first_popup_ok = true;
				}
				else if (fitting_first_popup_ok == true && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 360
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 280)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 280
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 240))){
					fitting_first_popup_ok = false;
					fitting_first_popup = true;
				}
			}
			else{//가이드 팝업 확인 완료시
				//측정 버튼 클릭시
				if (user_guide_coner_btn == false && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 540)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 260
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 160))){
					picture_begin_time = clock();

					user_guide_coner_btn = true;
				}
				else if (user_guide_coner_btn == true && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 540)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 260
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 160))){

					picture_save_time = float(clock() - picture_begin_time) / CLOCKS_PER_SEC;//포즈잡을 시간을 체크

					if (picture_save_time >= 4.0f){//4초이상 지났을 경우에 사진저장

						//현재 영상의 Img를 복사 하고
						//ROI설정하여 그 부분만 가져와서 저장한다.
						connercopy = cvCloneImage(skeleton);
						picture_nameset = "./conner.jpg";
						ScreenCapture(picture_nameset.c_str());

						//신체 측정시의 사용자의 왼손, 오른손의 x좌표를 기준으로 ROI처리 영역의 기준을 잡는다
						int temp_hand_right = points[NUI_SKELETON_POSITION_HAND_RIGHT].x;
						int temp_hand_left = points[NUI_SKELETON_POSITION_HAND_LEFT].x;

						//사진 측정 시의 사용자의 신체 좌표값을 저장한다.
						for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++){
							conner_points[i] = points[i];
						}

						//=======================사용자 외곽선 검출 메소드=============================
						IplImage *eig_img, *temp_img, *src_img_gray;
						CvPoint2D32f *corners;
						int i, corner_count = 150;
						CvPoint left_top_conner, left_max_conner, right_top_conner,right_max_conner, hip_right_conner, hip_left_conner;

						//초기화
						left_top_conner.x = 0, left_top_conner.y = 0;
						left_max_conner.x = 0, left_max_conner.y = 0;
						right_top_conner.x = 0, right_top_conner.y = 0;
						right_max_conner.x = 0, right_max_conner.y = 0;
						hip_right_conner.x = 0, hip_right_conner.y = 0;
						hip_left_conner.x = 0, hip_left_conner.y = 0;

						src_img_gray = cvLoadImage(picture_nameset.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
						eig_img = cvCreateImage(cvGetSize(src_img_gray), IPL_DEPTH_32F, 1);
						temp_img = cvCreateImage(cvGetSize(src_img_gray), IPL_DEPTH_32F, 1);
						corners = (CvPoint2D32f *)cvAlloc(corner_count * sizeof(CvPoint2D32f));

						//ROI설정
						cvSetImageROI(src_img_gray, cvRect(temp_hand_left, 0, temp_hand_right - temp_hand_left, 480));

						//코너 검출 메소드
						cvGoodFeaturesToTrack(src_img_gray, eig_img, temp_img, corners, &corner_count, 0.1, 15);
						cvFindCornerSubPix(src_img_gray, corners, corner_count,
							cvSize(3, 3), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
						
						cvResetImageROI(src_img_gray);
						
						//코너 검출 및 그리기
						for (i = 0; i < corner_count; i++){
							corners[i].x += temp_hand_left;

							//사용자의 신체에서 가장 가까운 외곽선 검출 좌표값을 찾아낸다.
							//사용자의 왼쪽어깨에서 가장 가까운 외곽선 좌표값을 찾아낸다.(왼어깨 위)
							if (corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x - 5 && corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x){
								if (corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y && corners[i].y > conner_points[NUI_SKELETON_POSITION_HEAD].y){//왼쪽 어깨좌표보다 낮은 y의 좌표를 찾는다(사용자의 어깨의 외곽선을 검출해낸다)
									if (left_top_conner.y == 0){
										left_top_conner.x = corners[i].x;
										left_top_conner.y = corners[i].y;
									}
									if (left_top_conner.y < corners[i].y){//현재 저장된 왼쪽 어깨의 외곽선보다 어깨에 더 근접한 경우
										left_top_conner.x = corners[i].x;
										left_top_conner.y = corners[i].y;
									}
								}
							}
							//사용자의 왼쪽어깨에서 가장 가까운 외곽선 좌표값을 찾아낸다.(어깨 맨 왼쪽)
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y + 5){
								if (corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x && corners[i].x > conner_points[NUI_SKELETON_POSITION_HAND_LEFT].x){//사용자의 왼쪽 어깨보다 x좌표가 왼쪽에 위치하는 경우
									if (left_max_conner.x == 0){
										left_max_conner.x = corners[i].x;
										left_max_conner.y = corners[i].y;
									}
									else if (left_max_conner.x < corners[i].x){//저장된 값보다 사용자의 신체에 더 가까운 경우
										left_max_conner.x = corners[i].x;
										left_max_conner.y = corners[i].y;
									}
								}
							}

							//사용자의 오른쪽 어깨에서 가장 가까운 위의 좌표값을 찾아낸다.(어깨위)
							if (corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x && corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x + 5){
								if (corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y && corners[i].y > conner_points[NUI_SKELETON_POSITION_HEAD].y){//오른쪽 어깨좌표보다 낮은 y의 좌표를 찾는다(사용자의 어깨의 외곽선을 검출해낸다)
									if (right_top_conner.y == 0){
										right_top_conner.x = corners[i].x;
										right_top_conner.y = corners[i].y;
									}
									else if (right_top_conner.y < corners[i].y){//현재 저장된 오른쪽 어깨의 외곽선보다 어깨에 더 근접한 경우
										right_top_conner.x = corners[i].x;
										right_top_conner.y = corners[i].y;
									}
								}
							}

							//사용자의 오른쪽 어깨에서 가장 가까운 밖의 좌표값을 찾아낸다.(어깨 맨 오른쪽)
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y + 5){
								if (corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x && corners[i].x < conner_points[NUI_SKELETON_POSITION_HAND_RIGHT].x){//사용자의 오른쪽 어깨보다 x좌표가 오른쪽에 위치하는 경우
									if (right_max_conner.x == 0){
										right_max_conner.x = corners[i].x;
										right_max_conner.y = corners[i].y;
									}
									else if (right_max_conner.x > corners[i].x){//저장된 값보다 사용자의 신체에 더 가까운 경우
										right_max_conner.x = corners[i].x;
										right_max_conner.y = corners[i].y;
									}
								}
							}

							//사용자의 골반 사이즈값을 찾아낸다.
							//왼쪽 골반을 기준으로 사용자의 외곽선을 찾아낸다.
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_HIP_LEFT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_HIP_LEFT].y + 10){
								if (corners[i].x < conner_points[NUI_SKELETON_POSITION_HIP_LEFT].x && corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x - 5){
									if (hip_left_conner.x == 0){
										hip_left_conner.x = corners[i].x;
										hip_left_conner.y = corners[i].y;
									}
									else if (hip_left_conner.x < corners[i].x){//저장된 값보다 사용자의 신체에 더 가까운 경우
										hip_left_conner.x = corners[i].x;
										hip_left_conner.y = corners[i].y;
									}
								}
							}
							//오른쪽 골반을 기준으로 사용자의 외곽선을 찾아낸다.
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].y + 10){
								if (corners[i].x > conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].x && corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x+5){
									if (hip_right_conner.x == 0){
										hip_right_conner.x = corners[i].x;
										hip_right_conner.y = corners[i].y;
									}
									else if (hip_right_conner.x > corners[i].x){//저장된 값보다 사용자의 신체에 더 가까운 경우
										hip_right_conner.x = corners[i].x;
										hip_right_conner.y = corners[i].y;
									}
								}
							}
						}

						//왼쪽 검증, 오른쪽 검증, 힙검증
						if (left_top_conner.x == 0 || left_top_conner.y == 0){//왼쪽 어깨위의 값이 측정X
							if (right_top_conner.x == 0 || right_top_conner.y == 0){//오른쪽 어깨 위의 값이 측정X
								//둘다 측정 안됬을 경우 기본값 사용
								user_shoulder_top = conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
							}
							else{//오른쪽 어깨의 값만 측정O
								user_shoulder_top = right_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
							}
						}
						else{//왼쪽 어깨 위의 값 측정O
							if (right_top_conner.x == 0 || right_top_conner.y == 0){//오른쪽 어깨 위의 값이 측정X
								//왼쪽 어깨 위의 값을 사용
								user_shoulder_top = conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y - left_top_conner.y;
							}
							else{//둘다 측정O
								if (left_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y>right_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y){//오른쪽 어깨 위의 값이 더 신체에 가까우면
									user_shoulder_top = right_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
								}
								else{//왼쪽 어깨 위의 값이 더 신체에 가까우면
									user_shoulder_top = left_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y;
								}
							}
						}

						if (left_max_conner.x == 0 || left_max_conner.y == 0){//왼쪽 어깨 바깥의 값이 측정X
							if (right_max_conner.x == 0 || right_max_conner.y == 0){//둘다 측정X
								//기본값 사용
								user_shoulder_max = (conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x - conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x)*2;
							}
							else{//오른쪽 어깨 바깥 측정O
								user_shoulder_max = (right_max_conner.x - conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x)*2;
							}
						}
						else{//왼쪽 어깨 바깥의 값 측정O
							if (right_max_conner.x == 0 || right_max_conner.y == 0){//오른쪽 어깨 바깥의 값이 측정X
								//왼쪽 어깨 바깥의 값 사용
								user_shoulder_max = (conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - left_max_conner.x) * 2;
							}
							else{//둘다 측정O
								if (conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x - left_max_conner.x > right_max_conner.x - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x){//오른쪽 어깨 밖의 값이 더 신체에 가까우면
									user_shoulder_max = (right_max_conner.x - conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x) * 2;
								}
								else{//왼쪽 어깨의 밖이 더 신체에 가까울 경우
									user_shoulder_max = (conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - left_max_conner.x) * 2;
								}
							}
						}

						if (hip_left_conner.x == 0 || hip_left_conner.y == 0){//왼쪽 힙의 값이 제대로 측정되지 않음.
							if (hip_right_conner.x == 0 || hip_right_conner.y == 0){//둘다 측정X
								user_hip = (conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].x - conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x) * 2;//기본값
							}
							else{//오른쪽 힙만 측정 됬을 경우
								user_hip = (hip_right_conner.x - conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x) * 2;
							}
						}
						else{//왼쪽 힙의 값이 제대로 측정 완료시
							if (hip_right_conner.x == 0 || hip_right_conner.y == 0){//오른쪽은 제대로 안됬을 경우 왼쪽만 살린다.
								//왼쪽의 값을 저장
								user_hip = (conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x - hip_left_conner.x) * 2;
							}
							else{//오른쪽도 제대로 측정 된경우
								if (hip_right_conner.x - conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].x > conner_points[NUI_SKELETON_POSITION_HIP_LEFT].x - hip_left_conner.x){//왼쪽 값이 더 신체에 가까운 경우
									user_hip = (conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x - hip_left_conner.x)*2;
								}
								else{//오른쪽 값이 더 신체에 가까운 경우
									user_hip = (hip_right_conner.x - conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x) * 2;
								}
							}
						}
						picture_nameset = "./canny.jpg";

						cvSaveImage(picture_nameset.c_str(),connercopy);

						//이미지 릴리즈
						cvReleaseImage(&eig_img);
						cvReleaseImage(&src_img_gray);
						cvReleaseImage(&temp_img);
						//=======================사용자 외곽선 검출 메소드=============================

					user_guide_coner_btn = true;
					user_coner_ok = true;
					}//if
				}
			}
			
		}
	}
}

void ScreenCapture(const char *strFilePath)
{
	//비트맵 파일 처리를 위한 헤더 구조체
	BITMAPFILEHEADER    BMFH;
	BITMAPINFOHEADER    BMIH;

	int nWidth = 0;
	int nHeight = 0;
	unsigned long dwQuadrupleWidth = 0;     //LJH 추가, 가로 사이즈가 4의 배수가 아니라면 4의 배수로 만들어서 저장

	GLbyte *pPixelData = NULL;              //front buffer의 픽셀 값들을 얻어 오기 위한 버퍼의 포인터

#ifdef WIN32
	//윈도우의 클라이언트 영역 좌표
	RECT ImageRect;
	GetClientRect(FindWindow(NULL, TEXT("Fitting Mirror")), &ImageRect);

	//이미지 영역 좌표를 이용하여 실제 이미지의 사이즈를 계산
	nWidth = ImageRect.right - ImageRect.left;     //윈도우 버전의 경우 사이즈 변경이 되므로 그때그때 조사
	nHeight = ImageRect.bottom - ImageRect.top;

#else
	nWidth = 1024;
	nHeight = 768;

#endif

	//4의 배수인지 아닌지 확인해서 4의 배수가 아니라면 4의 배수로 맞춰준다.
	dwQuadrupleWidth = (nWidth % 4) ? ((nWidth)+(4 - (nWidth % 4))) : (nWidth);

	//비트맵 파일 헤더 처리
	BMFH.bfType = 0x4D42;      //B(42)와 M(4D)에 해당하는 ASCII 값을 넣어준다.
	//바이트 단위로 전체파일 크기
	BMFH.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dwQuadrupleWidth * 3 * nHeight);
	//영상 데이터 위치까지의 거리
	BMFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//비트맵 인포 헤더 처리
	BMIH.biSize = sizeof(BITMAPINFOHEADER);       //이 구조체의 크기
	BMIH.biWidth = nWidth;                           //픽셀 단위로 영상의 폭
	BMIH.biHeight = nHeight;                          //영상의 높이
	BMIH.biPlanes = 1;                                //비트 플레인 수(항상 1)
	BMIH.biBitCount = 24;                               //픽셀당 비트수(컬러, 흑백 구별)
	BMIH.biCompression = BI_RGB;                           //압축 유무
	BMIH.biSizeImage = dwQuadrupleWidth * 3 * nHeight;   //영상의 크기
	BMIH.biXPelsPerMeter = 0;                                //가로 해상도
	BMIH.biYPelsPerMeter = 0;                                //세로 해상도
	BMIH.biClrUsed = 0;                                //실제 사용 색상수
	BMIH.biClrImportant = 0;                                //중요한 색상 인덱스

	pPixelData = new GLbyte[dwQuadrupleWidth * 3 * nHeight];  //LJH 수정

	//프런트 버퍼로 부터 픽셀 정보들을 얻어온다.
	glReadPixels(
		0, 0,                   //캡처할 영역의 좌측상단 좌표
		nWidth, nHeight,        //캡처할 영역의 크기
		GL_BGR_EXT,                 //캡처한 이미지의 픽셀 포맷
		GL_UNSIGNED_BYTE,       //캡처한 이미지의 데이터 포맷
		pPixelData              //캡처한 이미지의 정보를 담아둘 버퍼 포인터
		);

	{//저장 부분
		_mkdir("D:/picture/");

		FILE *outFile = fopen(strFilePath, "wb");

		if (outFile == NULL)
		{
			//에러 처리
			//printf( "에러" );
			//fclose( outFile );
		}

		fwrite(&BMFH, sizeof(char), sizeof(BITMAPFILEHEADER), outFile);         //파일 헤더 쓰기
		fwrite(&BMIH, sizeof(char), sizeof(BITMAPINFOHEADER), outFile);         //인포 헤더 쓰기
		fwrite(pPixelData, sizeof(unsigned char), BMIH.biSizeImage, outFile);   //glReadPixels로 읽은 데이터 쓰기

		fclose(outFile);  //파일 닫기
	}

	if (pPixelData != NULL)
	{
		delete pPixelData;
	}
}