// ������� ����
#include "Skeleton.h"

// ���̷����� 20�� ��������Ʈ ����
extern CvPoint points[NUI_SKELETON_POSITION_COUNT];
USHORT depths[NUI_SKELETON_POSITION_COUNT];
USHORT depth_temp;

bool SKELETON_POSITION_SHOULDER_RIGHT = false;
bool SKELETON_POSITION_SHOULDER_LEFT = false;

// ������ ��ǥ�� ��ȯ�Ѵ�.
CvPoint Skeleton::skeletonToScreen(Vector4 skeletonPoint){

	// ������ ��ǥ�� ǥ���� x, y�� ����
	LONG x, y;
	// Ű��Ʈ�κ��� �󸶳� �������ִ��� Ȯ���� ����(z��) ����
	USHORT depth;

	// �ȼ�����Ʈ�� ��ȯ
	NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth, NUI_IMAGE_RESOLUTION_640x480);

	// cvPoint(x, y)�� x, y�� ���ϱ� ���� �� ����
	int screenPointX = static_cast<int>(x);
	int screenPointY = static_cast<int>(y);
	depth_temp = depth;
	// cvPoint(x, y)���� ��ȯ
	return cvPoint(screenPointX, screenPointY + 20);
}

// ���ڰ����� ���̷��� �����Ϳ� �׸� �̹����� �ּҸ� �޴´�. (���̷����� �׸��� �Լ�)
void Skeleton::drawSkeleton(const NUI_SKELETON_DATA &position, IplImage *skeleton){

	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i){
		
		// �� ������ ��ġ�� x,y��ǥ�� �迭�� �ִ´�.
		points[i] = skeletonToScreen(position.SkeletonPositions[i]);
		depths[i] = depth_temp;
		// Ʈ��ŷ�Ǵ� ������ ��ġ�� ����Ȯ�� ���
		if (position.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED){
			if (i == NUI_SKELETON_POSITION_SHOULDER_RIGHT){
				SKELETON_POSITION_SHOULDER_RIGHT = false;
			}
			if (i == NUI_SKELETON_POSITION_SHOULDER_LEFT){
				SKELETON_POSITION_SHOULDER_LEFT = false;
			}
		}
		// Ʈ��ŷ�Ǵ� ������ ��ġ�� ��Ȯ�� ���
		else if (position.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_TRACKED){
			
			if (i == NUI_SKELETON_POSITION_SHOULDER_RIGHT){
				SKELETON_POSITION_SHOULDER_RIGHT = true;
			}
			if (i == NUI_SKELETON_POSITION_SHOULDER_LEFT){
				SKELETON_POSITION_SHOULDER_LEFT = true;
			}
		}
	}
	
	EventHandle eh;
	eh.handtracking(skeleton);
}