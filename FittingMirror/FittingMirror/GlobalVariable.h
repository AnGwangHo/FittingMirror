// ���� �ѹ��� ���� Ȯ��
#pragma once

// ������� ����
#include "FittingMirror.h"

// ���̷����� 20�� ��������Ʈ ����
CvPoint points[NUI_SKELETON_POSITION_COUNT];
// �� �̹����� ���� �� �̹������� ����(���ùڽ�, ���� ��, ��1, ��2, ��3...)
int currently_Clothes = 0;
int Suit_Num = 1, GreenSuit_Num = 2, GirlShirt1_Num = 3, GirlShirt2_Num = 4;
// ���� �� üũ, �� üũ, �� �̺�Ʈó��_üũ, �� �̺�Ʈó��_���뿡 ����� ���� ���� �� �ʱ�ȭ
bool left_Hand_Check, clothes_Check, clothes_EventHandle_Check, clothes_EventHandle_Put_On;
// ���� ���� ��ȣ, ���� �� ����
int  currently_clothes_Num, clothes_Total_Num;
//����� ���̵�, �̸�
char *user_id;
char *user_name;

//���� ��
CvPoint conner_points[NUI_SKELETON_POSITION_COUNT];
int user_hip = 0, user_shoulder_top = 0, user_shoulder_max = 0;