// ���� �ѹ��� ���� Ȯ��
#pragma once

// ������� ����
#include "FittingMirror.h"

// Class : cv�̹����� ���� �� RGB���� ����ϴ� Ŭ����
class CvOverlayImage
{
public:
	// cv�̹����� RGB���� ���(src = RGB����, overlay = �� �̹���, location = cv�̹��� ��ǥ, �ش� ȭ�� ��, �ش� ȭ�� ��)
	void cvOverlayImage(IplImage *src, IplImage *overlay, CvPoint location, CvScalar S, CvScalar D);
};