// 최초 한번만 파일 확인
#pragma once

// 헤더파일 참조
#include "FittingMirror.h"
#include "GlobalVariable.h"
#include "InitializeKinect.h"
#include "CreateRGBImage.h"
#include "ClothesOption.h"
#include "polygon.h"

// Class : 프로그램의 시작 부분인 메인 클래스(각 기능의 클래스를 실행 후 받아오는 데이터프레임값에 대한 실시간 영상을 보여준다.)
class Main
{
public:
	// 메인 함수("_tmain()" = Win32 콘솔 응용 프로그램의 메인함수)
	int _tmain(int argc, _TCHAR* argv[]);
};