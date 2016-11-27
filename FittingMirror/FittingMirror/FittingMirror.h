// 최초 한번만 파일 확인
#pragma once

// 이미지출력을 위한 opencv용 헤더파일
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>

// Win32애플리케이션을 사용하기 위한 헤더파일
#include <Windows.h>
#include <tchar.h>

// 키넥트 Api사용을 위한 헤더파일
#include <NuiApi.h>

// 프로그램의 기본 헤더파일
#include <stdio.h>
#include <iostream>

#include <gl\GL.h>
// 이름공간 선언(범위지정식별자를 생략)
using namespace std;
using namespace cv;

// 출력창의 가로 폭은 640
#define COLOR_WIDTH 640
// 출력창의 세로 폭은 480
#define COLOR_HEIGHT 480