// ���� �ѹ��� ���� Ȯ��
#pragma once

// ������� ����
#include "FittingMirror.h"
#include "Skeleton.h"

// Class : RGB���� ���� �� ����ϴ� Ŭ����
class CreateRGBImage
{
public:
	// RGB���� ���� �� ���(���̷��� �������� �����Ѵ�.)
	int createRGBImage(HANDLE h, IplImage* Color);
};