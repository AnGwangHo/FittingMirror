// 최초 한번만 파일 확인
#pragma once

// 헤더파일 참조
#include "FittingMirror.h"

// 스켈레톤의 20개 관절포인트 선언
CvPoint points[NUI_SKELETON_POSITION_COUNT];
// 옷 이미지를 담을 옷 이미지변수 선언(피팅박스, 현재 옷, 옷1, 옷2, 옷3...)
int currently_Clothes = 0;
int Suit_Num = 1, GreenSuit_Num = 2, GirlShirt1_Num = 3, GirlShirt2_Num = 4;
// 왼쪽 손 체크, 옷 체크, 옷 이벤트처리_체크, 옷 이벤트처리_착용에 사용할 변수 선언 및 초기화
bool left_Hand_Check, clothes_Check, clothes_EventHandle_Check, clothes_EventHandle_Put_On;
// 현재 옷의 번호, 옷의 총 갯수
int  currently_clothes_Num, clothes_Total_Num;
//사용자 아이디, 이름
char *user_id;
char *user_name;

//측정 값
CvPoint conner_points[NUI_SKELETON_POSITION_COUNT];
int user_hip = 0, user_shoulder_top = 0, user_shoulder_max = 0;