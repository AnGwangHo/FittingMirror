// ������� ����
#include "EventHandle.h"
#include "HistoryView.h"
#include <direct.h>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

// ���̷����� 20�� ��������Ʈ ����
extern CvPoint points[NUI_SKELETON_POSITION_COUNT];

extern int Suit_Num, Dress_Num, GreenSuit_Num, GirlShirt1_Num, GirlShirt2_Num;
// ���� �� üũ, �� üũ, �� �̺�Ʈó��_üũ, �� �̺�Ʈó��_���뿡 ����� ���� ���� �� �ʱ�ȭ
extern bool left_Hand_Check, clothes_Check, clothes_EventHandle_Check, clothes_EventHandle_Put_On;
// ���� ���� ��ȣ, ���� �� ����
extern int currently_clothes_Num, clothes_Total_Num;

// �ʼ��ý� üũ, ���� ���� üũ, ���� ���� üũ, ���� ����, ���� ��ϼ���, ���� ��ϼ���
bool cloth_choice, cloth_up_choice, cloth_down_choice, cloth_close, cloth_before, cloth_after;
int cloth_index;//���õ� ���� �ε���

// �������� üũ
bool picture_save;

// �����丮 üũ, �� ������ �����ϱ�, ��������, ���Ļ���, ���ἱ��
bool history_choice, history_exit=true;
bool history_save, history_before, history_after;
bool history_chocie_ok = false;

// �����丮�� ������ �̹����� �迭
extern int history_picture_index;
extern int upclothsx, upclothsy;

// Ŭ���� ��ü ����
CvOverlayImage coi;
HistoryView hv;

//���� ���� ���� ����
string picture_nameset;
string picture_save_path = "D:/picture/";
time_t curr_time;
struct tm *curr_tm;
bool picture_save_time_check;
float picture_save_time;
clock_t picture_begin_time;

//�����丮 ���� ������ ���ÿ���
extern bool history_left_click;
extern bool history_right_click;

//�����丮 ��ȸ�� ���� �����ִ� �̹��� ����
extern IplImage *viewNowImage;

extern bool user_guide_ok_btn;//����ڰ� �������̵带 Ȯ�� ���� �� flag
extern bool user_guide_coner_btn;//����ڰ� ���� ��ư�� ���� �Ͽ����� flag
bool user_coner_ok = false;//����� ��� ���� �Ϸ� flag

//����� �ܰ����� �����ϱ� ���� �޸� ���丮��
IplImage *testtemppicture;
IplImage *Gray = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
IplImage *findCanny = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
CvMemStorage *storage = cvCreateMemStorage(0);
CvSeq *contour = 0;

extern USHORT depths[NUI_SKELETON_POSITION_COUNT];//����� DepthMap Data �迭
extern long double view_size[2];//���� ������

//������¡ x,y ������
float increaseWidth = 0.0578f;
float increaseHeight = 0.05764f;
extern float cameraZ;//������ �ٶ󺸴� ����
extern float rotation;//�� ȸ������


//����, ������ ��� ������Ȳ
extern bool SKELETON_POSITION_SHOULDER_RIGHT;
extern bool SKELETON_POSITION_SHOULDER_LEFT;

//����� ���� ���� flag
bool fitting_first_popup_ok = false;
bool fitting_first_popup = false;

//���� �����丮�� ����� ������ ����
extern int indexsize;

//�ڳ� ������ ���� ���翵�� ����
IplImage *connercopy;

//������ ���� ������ ���� list ��������
extern vector<string> list;

//�α��� ���� flag
bool login = false;//�α��� UI���� ����
bool qrcode = false;//qr�ڵ� ���� ����
bool qrcode_check = false;//qr�ڵ� ���� ���� Ȯ��
bool logout = false;
extern char *user_id;
extern char *user_name;
char *test_id=NULL;
string str_user_id;
//����� ���� ��ü�������� ������, ��ü ������
extern CvPoint conner_points[NUI_SKELETON_POSITION_COUNT];
extern int user_hip, user_shoulder_top, user_shoulder_max;

void ScreenCapture(const char *strFilePath);//���� ���� ĸ�� �޼ҵ�

// �� �̹����� ���������� �巡�� �� ��� �̺�Ʈ ó��
void EventHandle::right_Hand_Clothes_Handler(bool clothes_handler_check, int x, int y, IplImage *skeleton){

	// �������� �� �����ȿ� ���� ���
	if (clothes_handler_check == true){

		// ���� ���� �������� �ʾҰ�, �� �巡�� Ȱ��ȭ�� �� �������� ������� �� ���� �����ȿ� ���� ���� ������ �غ� �Ǿ����� Ȯ���Ѵ�.
		if (clothes_EventHandle_Put_On == false &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - 30) &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x + 30) &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - 30) &&
			points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y + 30)){
			// ����ڰ� ���� �Ծ��ٴ� ���� Ȯ��
			clothes_EventHandle_Put_On = true;
		}
		//  ���� ����
		else if (clothes_EventHandle_Put_On == true){

			//���⿡ ����� ���� �°� y��ǥ ����
			upclothsx = points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x;
			upclothsy = points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y;
		}
		// ���������� ���� ������� ���� �������� �ʾ��� ���
		else {
			upclothsx = x;
			upclothsy = y;
		}
	}
	// �������� �� �����ȿ� ���� �ʾ��� ���
	else if (clothes_handler_check == false){
		//���� ó�� ���ý� ���� ��Ÿ�� �ʱⰪ x,y�� ��ġ ����
		upclothsx = x;
		upclothsy = y;
	}
}

// �ڵ�Ʈ��ŷ(���̷������� �νĵǴ� ����� �ִٸ� ����, ������ �� ������ �����Ͽ� �ش� �̺�Ʈ�� ó���Ѵ�.)
void EventHandle::handtracking(IplImage *skeleton){

	// ���̷��� ������ ��ǥ�� ���� ������ ����(�� �̹����� ��� ��ǥ�� ����� ����)
	int x, y;

	// ���̷������� �νĵǴ� ����� �ִٸ�
	if (points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x != 0 && points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y != 0){
		
		//������¡ ���(View ���� ����)
		if (depths[NUI_SKELETON_POSITION_SHOULDER_CENTER] >= 15760 && depths[NUI_SKELETON_POSITION_SHOULDER_CENTER] < 25760){
			//�ִ�� �ڷΰ��� 0, �ּҷ� �Ǿ��̸� 100
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

		//��谪 ������ �Ϸ� ���� ���
		if (user_coner_ok == TRUE){
			//����ڰ� ���� �������� ��� ȸ������� ���� if��
			if (currently_clothes_Num != 0){//���� ������ �Ǿ��ٸ�
				//��� ������ : rotation���� -,+�ϴ� ����
				//�������´� ���������� ���(Skeleton.cpp���� �������� 2�� ����)
				if (SKELETON_POSITION_SHOULDER_LEFT == true && SKELETON_POSITION_SHOULDER_RIGHT == false){//���� ����� ��, ������ ��� ��(��ȸ��)
					//(���� ����ġ�ǰ�*z������ ����)/2-(���� ����-���)/90*90
					rotation = ((points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x) / 90.0f)*90.0f-20.0f;
				}
				else if (SKELETON_POSITION_SHOULDER_LEFT == false && SKELETON_POSITION_SHOULDER_RIGHT == true){//������ ����� ��, ���� ��� ��(��ȸ��)
					rotation = -((points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x - points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x) / 90.0f)*90.0f-10.0f;//300.0f-���� 90
				}
				else{//���� �϶� ��¦�� Ʋ������
					rotation = 0.0f;
				}//if-else if()-else
			}//if

			// ���� �� ������ ���� x,y ��ǥ�� �����ۿ� ���� ����� �ش� �Ǵ� �̺�Ʈ ó��
			if (qrcode == false && picture_save == false && history_choice == false && cloth_choice == false && left_Hand_Check == true
				&& !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80 && points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0) && (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72	&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){

				// �� üũ�� �˻�(���������� ���� �巡���ߴ��� ���θ� �˻���. �������� ���� ���� �⺻ ���� "false"��)
				clothes_Check = false;

				// ���������� ���̹����� �ǵ���� ���(���� x,y ��ǥ ������ ���� �� ������ ������ �ش� �������� �� �� �̺�Ʈ ó��)
				if (clothes_EventHandle_Check == false &&
					points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 500 && points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 220)
				{
					// ���������� ���� �巡���� ���� Ȱ��ȭ��Ų��.
					clothes_EventHandle_Check = true;
				}
				// ������ �� �巡�װ� ��Ȱ��ȭ ������ ���
				else if (clothes_EventHandle_Check == false){
					x = 500;
					y = 220;
					right_Hand_Clothes_Handler(clothes_EventHandle_Check, x, y, skeleton);
				}
				// ���������� ���� �巡���� ���� Ȱ��ȭ�� ��쿡 else���� �����Ѵ�.
				else
				{
					x = points[NUI_SKELETON_POSITION_HAND_RIGHT].x;
					y = points[NUI_SKELETON_POSITION_HAND_RIGHT].y;
					// �� ���� �̺�Ʈ ó�� �Լ�
					right_Hand_Clothes_Handler(clothes_EventHandle_Check, x, y, skeleton);
				}
			}
			// ���� �� ������ ���� x,y ��ǥ�� ���� ���� �ȿ� �����Ѵٸ� ���� ��ȯ�� �غ� �Ѵ�.
			else if (qrcode == false && picture_save == false && history_choice == false && cloth_choice == false && clothes_Check == false && points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0){

				// �� ���� �ʱ�ȭ
				left_Hand_Check = true;
				clothes_Check = true;
				clothes_EventHandle_Check = false;
				clothes_EventHandle_Put_On = false;

				// �� ��ȯ 
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

			//�α��� ���� ��
			if (history_choice == false && picture_save == false && login == false && qrcode == false && ((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 340
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 261
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){
				qrcode = true;
			}
			//�α��� ���� �� ���� ������ ������ ���
			else if (history_choice == false && picture_save == false && login == false && qrcode == true && !((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 340
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 261
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 0))){

				//QR�ڵ� ������ư ���� ��
				if (qrcode_check == false && ((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 540)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 260
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 160))){
					qrcode_check = true;
					picture_begin_time = clock();//�� ��� ������ �ð��� �����Ѵ�.
					picture_save_time_check = TRUE;
				}
				//QR�ڵ� ������ư ���� �� ���� ������ ������ ���
				else if (qrcode_check == true && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 540)
					&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 260
					&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 160))){

					if (login == false){//�α����� �ȵǾ��� ��쿡��
						//QR�ڵ� ���� �޼ҵ�
						picture_save_time = float(clock() - picture_begin_time) / CLOCKS_PER_SEC;//QR�ڵ� �νĽ�ų �ð��� üũ
						if (picture_save_time_check == TRUE){//����� �� ������ �������� üũ�Ѵ�.
							if (picture_save_time >= 5.0f){//5���̻� ������ ��쿡 QR�ڵ� üũ
								
								//������ ������
								IplImage* temp = cvCloneImage(skeleton);
								cvSetImageROI(temp, cvRect(125, 65, 390, 350));
								picture_nameset = "QRCode.jpg";
								cvSaveImage(picture_nameset.c_str(), temp);
								cvReleaseImage(&temp);

								//QR�ڵ� �м� �� �������
								system("java -jar QR.jar");

								char inputString[1024];

								//�����κ��� �α��� ��� ���� �о����
								ifstream inFile("./java/ServerResponse.txt");

								while (!inFile.eof())
								{
									inFile.getline(inputString, 100);
									cout << inputString << endl;
								}
								inFile.close();

								//����� ���̵� ���ڿ��� ���� ����
								user_id = strtok_s(inputString, "=", &user_name);
								str_user_id = user_id;

								picture_save_time_check = FALSE;//�ð� üũ flag �ʱ�ȭ
								qrcode_check = false;
								qrcode = false;
								login = true;
							}
							else{
								//3,2,1 �ϰ� �̺�Ʈǥ��
							}//if-else()
						}//if
					}//if
				}//if-else if()
			}//if-else if()
			
			//�α׾ƿ� �̺�Ʈó��
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

			//���� ���� ���� ��
			if (qrcode == false && currently_clothes_Num != 0 && history_choice == false && picture_save == false &&
				((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 275
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 204))){
				picture_begin_time = clock();//�� ��� ������ �ð��� �����Ѵ�.
				picture_save_time_check = TRUE;
				picture_save = true;
			}
			else if (currently_clothes_Num != 0 && history_choice == false && picture_save == true &&
				!((points[NUI_SKELETON_POSITION_HAND_LEFT].x <= 80
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].x >= 0)
				&& (points[NUI_SKELETON_POSITION_HAND_LEFT].y <= 275
				&& points[NUI_SKELETON_POSITION_HAND_LEFT].y >= 204))){
			
				picture_save_time = float(clock() - picture_begin_time) / CLOCKS_PER_SEC;//�������� �ð��� üũ

				if (picture_save_time_check == TRUE){//����� �� ������ �������� üũ�Ѵ�.
					if (picture_save_time >= 3.0f){//3���̻� ������ ��쿡 ��������
						curr_time = time(NULL);
						curr_tm = localtime(&curr_time);
						picture_nameset = picture_save_path + to_string(curr_tm->tm_mday) + "d" + to_string(curr_tm->tm_hour) + "h" + to_string(curr_tm->tm_min) + "m" + to_string(curr_tm->tm_sec) + ".jpg";
						ScreenCapture(picture_nameset.c_str());
						picture_save = false;
						picture_save_time_check = FALSE;//�ð� üũ flag �ʱ�ȭ
					}
				}
			}
				
			
			//�����丮 ���� ��
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
			
				//�����丮 ���ý� ������ �ִ� ��쿡�� �����ش�.
				if (indexsize != 0){
					history_exit = false;
					cloth_choice = true;
				}
				else{//������ ���� ���
					history_choice = false;
				}
			
			}
		
			//�ݱ� ��ư Ŭ����
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

			//�ݱ� ��ư�� ������ ��
			if (qrcode == false && picture_save == false && history_choice == true && history_exit == false && !((points[NUI_SKELETON_POSITION_HAND_RIGHT].x <= 640
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].x >= 558)
				&& (points[NUI_SKELETON_POSITION_HAND_RIGHT].y <= 72
				&& points[NUI_SKELETON_POSITION_HAND_RIGHT].y >= 0))){

				//���� ��ưŬ����
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
					//�迭���� index ���� ���� �̹��� �����Ű�� �޼ҵ�
					hv.moveImage(0);
				}

				//������ ��ưŬ����
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
					//�迭���� index ���� ���� �̹��� �����Ű�� �޼ҵ�
					hv.moveImage(1);
				}


				//������ ���������ư Ŭ����
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
					//������ ������ ���ڰ� = (���� �����ִ� �̹���, �� ��������)
					hv.saveServerImageUser();
				}
			}
		}
		else{//���� ������ ���ߴٸ�
			//�� ó�� ���̵� �˾��� Ŭ�� �Ϸ��
			if (fitting_first_popup == false){
				//������ ���������ư Ŭ����
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
			else{//���̵� �˾� Ȯ�� �Ϸ��
				//���� ��ư Ŭ����
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

					picture_save_time = float(clock() - picture_begin_time) / CLOCKS_PER_SEC;//�������� �ð��� üũ

					if (picture_save_time >= 4.0f){//4���̻� ������ ��쿡 ��������

						//���� ������ Img�� ���� �ϰ�
						//ROI�����Ͽ� �� �κи� �����ͼ� �����Ѵ�.
						connercopy = cvCloneImage(skeleton);
						picture_nameset = "./conner.jpg";
						ScreenCapture(picture_nameset.c_str());

						//��ü �������� ������� �޼�, �������� x��ǥ�� �������� ROIó�� ������ ������ ��´�
						int temp_hand_right = points[NUI_SKELETON_POSITION_HAND_RIGHT].x;
						int temp_hand_left = points[NUI_SKELETON_POSITION_HAND_LEFT].x;

						//���� ���� ���� ������� ��ü ��ǥ���� �����Ѵ�.
						for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++){
							conner_points[i] = points[i];
						}

						//=======================����� �ܰ��� ���� �޼ҵ�=============================
						IplImage *eig_img, *temp_img, *src_img_gray;
						CvPoint2D32f *corners;
						int i, corner_count = 150;
						CvPoint left_top_conner, left_max_conner, right_top_conner,right_max_conner, hip_right_conner, hip_left_conner;

						//�ʱ�ȭ
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

						//ROI����
						cvSetImageROI(src_img_gray, cvRect(temp_hand_left, 0, temp_hand_right - temp_hand_left, 480));

						//�ڳ� ���� �޼ҵ�
						cvGoodFeaturesToTrack(src_img_gray, eig_img, temp_img, corners, &corner_count, 0.1, 15);
						cvFindCornerSubPix(src_img_gray, corners, corner_count,
							cvSize(3, 3), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
						
						cvResetImageROI(src_img_gray);
						
						//�ڳ� ���� �� �׸���
						for (i = 0; i < corner_count; i++){
							corners[i].x += temp_hand_left;

							//������� ��ü���� ���� ����� �ܰ��� ���� ��ǥ���� ã�Ƴ���.
							//������� ���ʾ������ ���� ����� �ܰ��� ��ǥ���� ã�Ƴ���.(�޾�� ��)
							if (corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x - 5 && corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x){
								if (corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y && corners[i].y > conner_points[NUI_SKELETON_POSITION_HEAD].y){//���� �����ǥ���� ���� y�� ��ǥ�� ã�´�(������� ����� �ܰ����� �����س���)
									if (left_top_conner.y == 0){
										left_top_conner.x = corners[i].x;
										left_top_conner.y = corners[i].y;
									}
									if (left_top_conner.y < corners[i].y){//���� ����� ���� ����� �ܰ������� ����� �� ������ ���
										left_top_conner.x = corners[i].x;
										left_top_conner.y = corners[i].y;
									}
								}
							}
							//������� ���ʾ������ ���� ����� �ܰ��� ��ǥ���� ã�Ƴ���.(��� �� ����)
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y + 5){
								if (corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x && corners[i].x > conner_points[NUI_SKELETON_POSITION_HAND_LEFT].x){//������� ���� ������� x��ǥ�� ���ʿ� ��ġ�ϴ� ���
									if (left_max_conner.x == 0){
										left_max_conner.x = corners[i].x;
										left_max_conner.y = corners[i].y;
									}
									else if (left_max_conner.x < corners[i].x){//����� ������ ������� ��ü�� �� ����� ���
										left_max_conner.x = corners[i].x;
										left_max_conner.y = corners[i].y;
									}
								}
							}

							//������� ������ ������� ���� ����� ���� ��ǥ���� ã�Ƴ���.(�����)
							if (corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x && corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x + 5){
								if (corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y && corners[i].y > conner_points[NUI_SKELETON_POSITION_HEAD].y){//������ �����ǥ���� ���� y�� ��ǥ�� ã�´�(������� ����� �ܰ����� �����س���)
									if (right_top_conner.y == 0){
										right_top_conner.x = corners[i].x;
										right_top_conner.y = corners[i].y;
									}
									else if (right_top_conner.y < corners[i].y){//���� ����� ������ ����� �ܰ������� ����� �� ������ ���
										right_top_conner.x = corners[i].x;
										right_top_conner.y = corners[i].y;
									}
								}
							}

							//������� ������ ������� ���� ����� ���� ��ǥ���� ã�Ƴ���.(��� �� ������)
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y + 5){
								if (corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x && corners[i].x < conner_points[NUI_SKELETON_POSITION_HAND_RIGHT].x){//������� ������ ������� x��ǥ�� �����ʿ� ��ġ�ϴ� ���
									if (right_max_conner.x == 0){
										right_max_conner.x = corners[i].x;
										right_max_conner.y = corners[i].y;
									}
									else if (right_max_conner.x > corners[i].x){//����� ������ ������� ��ü�� �� ����� ���
										right_max_conner.x = corners[i].x;
										right_max_conner.y = corners[i].y;
									}
								}
							}

							//������� ��� ������� ã�Ƴ���.
							//���� ����� �������� ������� �ܰ����� ã�Ƴ���.
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_HIP_LEFT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_HIP_LEFT].y + 10){
								if (corners[i].x < conner_points[NUI_SKELETON_POSITION_HIP_LEFT].x && corners[i].x > conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x - 5){
									if (hip_left_conner.x == 0){
										hip_left_conner.x = corners[i].x;
										hip_left_conner.y = corners[i].y;
									}
									else if (hip_left_conner.x < corners[i].x){//����� ������ ������� ��ü�� �� ����� ���
										hip_left_conner.x = corners[i].x;
										hip_left_conner.y = corners[i].y;
									}
								}
							}
							//������ ����� �������� ������� �ܰ����� ã�Ƴ���.
							if (corners[i].y > conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].y - 10 && corners[i].y < conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].y + 10){
								if (corners[i].x > conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].x && corners[i].x < conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x+5){
									if (hip_right_conner.x == 0){
										hip_right_conner.x = corners[i].x;
										hip_right_conner.y = corners[i].y;
									}
									else if (hip_right_conner.x > corners[i].x){//����� ������ ������� ��ü�� �� ����� ���
										hip_right_conner.x = corners[i].x;
										hip_right_conner.y = corners[i].y;
									}
								}
							}
						}

						//���� ����, ������ ����, ������
						if (left_top_conner.x == 0 || left_top_conner.y == 0){//���� ������� ���� ����X
							if (right_top_conner.x == 0 || right_top_conner.y == 0){//������ ��� ���� ���� ����X
								//�Ѵ� ���� �ȉ��� ��� �⺻�� ���
								user_shoulder_top = conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
							}
							else{//������ ����� ���� ����O
								user_shoulder_top = right_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
							}
						}
						else{//���� ��� ���� �� ����O
							if (right_top_conner.x == 0 || right_top_conner.y == 0){//������ ��� ���� ���� ����X
								//���� ��� ���� ���� ���
								user_shoulder_top = conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y - left_top_conner.y;
							}
							else{//�Ѵ� ����O
								if (left_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y>right_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y){//������ ��� ���� ���� �� ��ü�� ������
									user_shoulder_top = right_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
								}
								else{//���� ��� ���� ���� �� ��ü�� ������
									user_shoulder_top = left_top_conner.y - conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y;
								}
							}
						}

						if (left_max_conner.x == 0 || left_max_conner.y == 0){//���� ��� �ٱ��� ���� ����X
							if (right_max_conner.x == 0 || right_max_conner.y == 0){//�Ѵ� ����X
								//�⺻�� ���
								user_shoulder_max = (conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x - conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x)*2;
							}
							else{//������ ��� �ٱ� ����O
								user_shoulder_max = (right_max_conner.x - conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x)*2;
							}
						}
						else{//���� ��� �ٱ��� �� ����O
							if (right_max_conner.x == 0 || right_max_conner.y == 0){//������ ��� �ٱ��� ���� ����X
								//���� ��� �ٱ��� �� ���
								user_shoulder_max = (conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - left_max_conner.x) * 2;
							}
							else{//�Ѵ� ����O
								if (conner_points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x - left_max_conner.x > right_max_conner.x - conner_points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x){//������ ��� ���� ���� �� ��ü�� ������
									user_shoulder_max = (right_max_conner.x - conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x) * 2;
								}
								else{//���� ����� ���� �� ��ü�� ����� ���
									user_shoulder_max = (conner_points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - left_max_conner.x) * 2;
								}
							}
						}

						if (hip_left_conner.x == 0 || hip_left_conner.y == 0){//���� ���� ���� ����� �������� ����.
							if (hip_right_conner.x == 0 || hip_right_conner.y == 0){//�Ѵ� ����X
								user_hip = (conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].x - conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x) * 2;//�⺻��
							}
							else{//������ ���� ���� ���� ���
								user_hip = (hip_right_conner.x - conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x) * 2;
							}
						}
						else{//���� ���� ���� ����� ���� �Ϸ��
							if (hip_right_conner.x == 0 || hip_right_conner.y == 0){//�������� ����� �ȉ��� ��� ���ʸ� �츰��.
								//������ ���� ����
								user_hip = (conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x - hip_left_conner.x) * 2;
							}
							else{//�����ʵ� ����� ���� �Ȱ��
								if (hip_right_conner.x - conner_points[NUI_SKELETON_POSITION_HIP_RIGHT].x > conner_points[NUI_SKELETON_POSITION_HIP_LEFT].x - hip_left_conner.x){//���� ���� �� ��ü�� ����� ���
									user_hip = (conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x - hip_left_conner.x)*2;
								}
								else{//������ ���� �� ��ü�� ����� ���
									user_hip = (hip_right_conner.x - conner_points[NUI_SKELETON_POSITION_HIP_CENTER].x) * 2;
								}
							}
						}
						picture_nameset = "./canny.jpg";

						cvSaveImage(picture_nameset.c_str(),connercopy);

						//�̹��� ������
						cvReleaseImage(&eig_img);
						cvReleaseImage(&src_img_gray);
						cvReleaseImage(&temp_img);
						//=======================����� �ܰ��� ���� �޼ҵ�=============================

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
	//��Ʈ�� ���� ó���� ���� ��� ����ü
	BITMAPFILEHEADER    BMFH;
	BITMAPINFOHEADER    BMIH;

	int nWidth = 0;
	int nHeight = 0;
	unsigned long dwQuadrupleWidth = 0;     //LJH �߰�, ���� ����� 4�� ����� �ƴ϶�� 4�� ����� ���� ����

	GLbyte *pPixelData = NULL;              //front buffer�� �ȼ� ������ ��� ���� ���� ������ ������

#ifdef WIN32
	//�������� Ŭ���̾�Ʈ ���� ��ǥ
	RECT ImageRect;
	GetClientRect(FindWindow(NULL, TEXT("Fitting Mirror")), &ImageRect);

	//�̹��� ���� ��ǥ�� �̿��Ͽ� ���� �̹����� ����� ���
	nWidth = ImageRect.right - ImageRect.left;     //������ ������ ��� ������ ������ �ǹǷ� �׶��׶� ����
	nHeight = ImageRect.bottom - ImageRect.top;

#else
	nWidth = 1024;
	nHeight = 768;

#endif

	//4�� ������� �ƴ��� Ȯ���ؼ� 4�� ����� �ƴ϶�� 4�� ����� �����ش�.
	dwQuadrupleWidth = (nWidth % 4) ? ((nWidth)+(4 - (nWidth % 4))) : (nWidth);

	//��Ʈ�� ���� ��� ó��
	BMFH.bfType = 0x4D42;      //B(42)�� M(4D)�� �ش��ϴ� ASCII ���� �־��ش�.
	//����Ʈ ������ ��ü���� ũ��
	BMFH.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dwQuadrupleWidth * 3 * nHeight);
	//���� ������ ��ġ������ �Ÿ�
	BMFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//��Ʈ�� ���� ��� ó��
	BMIH.biSize = sizeof(BITMAPINFOHEADER);       //�� ����ü�� ũ��
	BMIH.biWidth = nWidth;                           //�ȼ� ������ ������ ��
	BMIH.biHeight = nHeight;                          //������ ����
	BMIH.biPlanes = 1;                                //��Ʈ �÷��� ��(�׻� 1)
	BMIH.biBitCount = 24;                               //�ȼ��� ��Ʈ��(�÷�, ��� ����)
	BMIH.biCompression = BI_RGB;                           //���� ����
	BMIH.biSizeImage = dwQuadrupleWidth * 3 * nHeight;   //������ ũ��
	BMIH.biXPelsPerMeter = 0;                                //���� �ػ�
	BMIH.biYPelsPerMeter = 0;                                //���� �ػ�
	BMIH.biClrUsed = 0;                                //���� ��� �����
	BMIH.biClrImportant = 0;                                //�߿��� ���� �ε���

	pPixelData = new GLbyte[dwQuadrupleWidth * 3 * nHeight];  //LJH ����

	//����Ʈ ���۷� ���� �ȼ� �������� ���´�.
	glReadPixels(
		0, 0,                   //ĸó�� ������ ������� ��ǥ
		nWidth, nHeight,        //ĸó�� ������ ũ��
		GL_BGR_EXT,                 //ĸó�� �̹����� �ȼ� ����
		GL_UNSIGNED_BYTE,       //ĸó�� �̹����� ������ ����
		pPixelData              //ĸó�� �̹����� ������ ��Ƶ� ���� ������
		);

	{//���� �κ�
		_mkdir("D:/picture/");

		FILE *outFile = fopen(strFilePath, "wb");

		if (outFile == NULL)
		{
			//���� ó��
			//printf( "����" );
			//fclose( outFile );
		}

		fwrite(&BMFH, sizeof(char), sizeof(BITMAPFILEHEADER), outFile);         //���� ��� ����
		fwrite(&BMIH, sizeof(char), sizeof(BITMAPINFOHEADER), outFile);         //���� ��� ����
		fwrite(pPixelData, sizeof(unsigned char), BMIH.biSizeImage, outFile);   //glReadPixels�� ���� ������ ����

		fclose(outFile);  //���� �ݱ�
	}

	if (pPixelData != NULL)
	{
		delete pPixelData;
	}
}