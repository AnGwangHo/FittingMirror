// ������� ����
#include "ClothesOption.h"

// ���̷����� 20�� ��������Ʈ ����
extern CvPoint points[NUI_SKELETON_POSITION_COUNT];
// �� �̹����� ���� �� �̹������� ����(���� ��, ��1, ��2, ��3)
extern int Suit_Num, Dress_Num, GreenSuit_Num, GirlShirt1_Num, GirlShirt2_Num, GirlNasi1_Num, GirlNasi2_Num;
// ���� �� üũ, �� üũ, �� �̺�Ʈó��_üũ, �� �̺�Ʈó��_���뿡 ����� ���� ���� �� �ʱ�ȭ
extern bool left_Hand_Check, clothes_Check, clothes_EventHandle_Check, clothes_EventHandle_Put_On;
// ���� ���� ��ȣ, ���� �� ����
extern int  currently_clothes_Num, clothes_Total_Num;

// �� ���� ����
void ClothesOption::clothesOption(){


	// ���ʼ� üũ, �� üũ, �� �̺�Ʈó��_üũ, �� �̺�Ʈó��_���� ���� �ʱ�ȭ
	left_Hand_Check = false, clothes_Check = false, clothes_EventHandle_Check = false, clothes_EventHandle_Put_On = false;
	// ���� ���� ��ȣ
	currently_clothes_Num = 0;
	// ���� �� ����
	clothes_Total_Num = 6;

}