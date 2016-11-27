// 헤더파일 참조
#include "ClothesOption.h"

// 스켈레톤의 20개 관절포인트 선언
extern CvPoint points[NUI_SKELETON_POSITION_COUNT];
// 옷 이미지를 담을 옷 이미지변수 선언(현재 옷, 옷1, 옷2, 옷3)
extern int Suit_Num, Dress_Num, GreenSuit_Num, GirlShirt1_Num, GirlShirt2_Num, GirlNasi1_Num, GirlNasi2_Num;
// 왼쪽 손 체크, 옷 체크, 옷 이벤트처리_체크, 옷 이벤트처리_착용에 사용할 변수 선언 및 초기화
extern bool left_Hand_Check, clothes_Check, clothes_EventHandle_Check, clothes_EventHandle_Put_On;
// 현재 옷의 번호, 옷의 총 갯수
extern int  currently_clothes_Num, clothes_Total_Num;

// 옷 설정 정의
void ClothesOption::clothesOption(){


	// 왼쪽손 체크, 옷 체크, 옷 이벤트처리_체크, 옷 이벤트처리_착용 변수 초기화
	left_Hand_Check = false, clothes_Check = false, clothes_EventHandle_Check = false, clothes_EventHandle_Put_On = false;
	// 현재 옷의 번호
	currently_clothes_Num = 0;
	// 옷의 총 갯수
	clothes_Total_Num = 6;

}