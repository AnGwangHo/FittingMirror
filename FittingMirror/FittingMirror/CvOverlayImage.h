// 최초 한번만 파일 확인
#pragma once

// 헤더파일 참조
#include "FittingMirror.h"

// Class : cv이미지를 생성 후 RGB영상에 출력하는 클래스
class CvOverlayImage
{
public:
	// cv이미지를 RGB영상에 출력(src = RGB영상, overlay = 옷 이미지, location = cv이미지 좌표, 해당 화소 값, 해당 화소 값)
	void cvOverlayImage(IplImage *src, IplImage *overlay, CvPoint location, CvScalar S, CvScalar D);
};