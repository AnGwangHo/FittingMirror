// ���� �ѹ��� ���� Ȯ��
#pragma once

// ������� ����
#include "FittingMirror.h"
#include "CvOverlayImage.h"

// Class : �̺�Ʈ ó���� ���� Ŭ����
class EventHandle
{
public:
	// �� �̹����� ���������� �巡�� �� ��� �̺�Ʈ ó��
	void right_Hand_Clothes_Handler(bool clothes_handler_check, int x, int y, IplImage *skeleton);
	// �ڵ�Ʈ��ŷ(���̷������� �νĵǴ� ����� �ִٸ� ����, ������ �� ������ �����Ͽ� �ش� �̺�Ʈ�� ó���Ѵ�.)
	void handtracking(IplImage *skeleton);
};