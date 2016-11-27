// 최초 한번만 파일 확인
#pragma once

// 헤더파일 참조
#include "FittingMirror.h"
#include "CvOverlayImage.h"

// Class : 이벤트 처리에 대한 클래스
class EventHandle
{
public:
	// 옷 이미지를 오른손으로 드래그 할 경우 이벤트 처리
	void right_Hand_Clothes_Handler(bool clothes_handler_check, int x, int y, IplImage *skeleton);
	// 핸드트래킹(스켈레톤으로 인식되는 사람이 있다면 왼쪽, 오른쪽 손 관절을 추적하여 해당 이벤트를 처리한다.)
	void handtracking(IplImage *skeleton);
};