// 헤더파일 참조
#include "Skeleton.h"

// 스켈레톤의 20개 관절포인트 선언
extern CvPoint points[NUI_SKELETON_POSITION_COUNT];
USHORT depths[NUI_SKELETON_POSITION_COUNT];
USHORT depth_temp;

bool SKELETON_POSITION_SHOULDER_RIGHT = false;
bool SKELETON_POSITION_SHOULDER_LEFT = false;

// 관절의 좌표를 반환한다.
CvPoint Skeleton::skeletonToScreen(Vector4 skeletonPoint){

	// 관절의 좌표를 표현할 x, y축 변수
	LONG x, y;
	// 키넥트로부터 얼마나 떨어져있는지 확인할 깊이(z축) 변수
	USHORT depth;

	// 픽셀포인트로 변환
	NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth, NUI_IMAGE_RESOLUTION_640x480);

	// cvPoint(x, y)의 x, y를 구하기 위한 값 설정
	int screenPointX = static_cast<int>(x);
	int screenPointY = static_cast<int>(y);
	depth_temp = depth;
	// cvPoint(x, y)값을 반환
	return cvPoint(screenPointX, screenPointY + 20);
}

// 인자값으로 스켈레톤 데이터와 그릴 이미지의 주소를 받는다. (스켈레톤을 그리는 함수)
void Skeleton::drawSkeleton(const NUI_SKELETON_DATA &position, IplImage *skeleton){

	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i){
		
		// 각 관절의 위치를 x,y좌표로 배열에 넣는다.
		points[i] = skeletonToScreen(position.SkeletonPositions[i]);
		depths[i] = depth_temp;
		// 트래킹되는 관절의 위치가 부정확할 경우
		if (position.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED){
			if (i == NUI_SKELETON_POSITION_SHOULDER_RIGHT){
				SKELETON_POSITION_SHOULDER_RIGHT = false;
			}
			if (i == NUI_SKELETON_POSITION_SHOULDER_LEFT){
				SKELETON_POSITION_SHOULDER_LEFT = false;
			}
		}
		// 트래킹되는 관절의 위치가 정확할 경우
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