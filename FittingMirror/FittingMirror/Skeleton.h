// ���� �ѹ��� ���� Ȯ��
#pragma once

// ������� ����
#include "FittingMirror.h"
#include "EventHandle.h"

// Class : ���̷��濡 ���� �������������� �����ϴ� Ŭ����
class Skeleton
{
public:
	// ������ ��ǥ�� ��ȯ�Ѵ�.
	CvPoint skeletonToScreen(Vector4 skeletonPoint);
	// ����� ����� ���� ���̷��� ���� Ʈ��ŷ���� ���� �׷��ش�.
	void drawBone(const NUI_SKELETON_DATA &position, NUI_SKELETON_POSITION_INDEX j1, NUI_SKELETON_POSITION_INDEX j2, IplImage *skeleton);
	// ���ڰ����� ���̷��� �����Ϳ� �׸� �̹����� �ּҸ� �޴´�. (���̷����� �׸��� �Լ�)
	void drawSkeleton(const NUI_SKELETON_DATA &position, IplImage *skeleton);
};