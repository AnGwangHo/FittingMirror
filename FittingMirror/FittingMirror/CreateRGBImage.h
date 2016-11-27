// 최초 한번만 파일 확인
#pragma once

// 헤더파일 참조
#include "FittingMirror.h"
#include "Skeleton.h"

// Class : RGB영상 생성 후 출력하는 클래스
class CreateRGBImage
{
public:
	// RGB영상 생성 후 출력(스켈레톤 프레임을 포함한다.)
	int createRGBImage(HANDLE h, IplImage* Color);
};