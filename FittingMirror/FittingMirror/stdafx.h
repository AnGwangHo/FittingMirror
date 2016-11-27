// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <tchar.h>
#include <math.h>
#include <iostream>
#include <string.h>   
#include <stdlib.h>
#include <assert.h>
using namespace std;



#pragma comment(lib, "GL/glew32.lib")   
#pragma comment(lib, "GL/GLAUX.lib")
#pragma comment(lib, "GL/glut32.lib")
#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/GLAUX.H"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#pragma comment(lib, "IL/DevIL.lib")   
#pragma comment(lib, "IL/ILU.lib")   
#pragma comment(lib, "IL/ILUT.lib")  

#include "GLTexture.h"
#include "Model_3DS.h"
#include "main2.h"