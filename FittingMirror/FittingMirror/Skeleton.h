// 최초 한번만 파일 확인
#pragma once

// 헤더파일 참조
#include "FittingMirror.h"
#include "EventHandle.h"

// Class : 스켈레톤에 대한 데이터프레임을 포함하는 클래스
class Skeleton
{
public:
	// 관절의 좌표를 반환한다.
	CvPoint skeletonToScreen(Vector4 skeletonPoint);
	// 영상속 사람에 대한 스켈레톤 관절 트래킹으로 뼈를 그려준다.
	void drawBone(const NUI_SKELETON_DATA &position, NUI_SKELETON_POSITION_INDEX j1, NUI_SKELETON_POSITION_INDEX j2, IplImage *skeleton);
	// 인자값으로 스켈레톤 데이터와 그릴 이미지의 주소를 받는다. (스켈레톤을 그리는 함수)
	void drawSkeleton(const NUI_SKELETON_DATA &position, IplImage *skeleton);
};