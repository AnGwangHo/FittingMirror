#include "CreateRGBImage.h"
#include <direct.h>//파일 삭제를 위한 헤더파일
#include "HistoryView.h"//히스토리 조회 헤더파일

// 피팅박스 이미지 

extern IplImage* iplTexture;//사용자의 모습을

extern bool history_choice, history_exit;
extern bool picture_save;
extern IplImage *viewNowImage;

extern bool user_coner_ok;//사용자 경계 추출 완료 flag
bool user_guide_ok_btn = TRUE;//사용자가 유저가이드를 확인 선택 시 flag
bool user_guide_coner_btn = FALSE;//사용자가 측정 버튼을 선택 하였는지 flag
bool user_appreciate;// 사용자가 현재 인식되는지 여부를 체크하는 flag
bool user_out_check;// 사용자가 키넥트앞을 벗어났는지를 시간체크를 위한 flag
bool ad_on;//사용자가 사라져서 다시 광고의 이미지를 load시켜줘야함
bool ad_off;//광고가 꺼진 경우
float delete_time;//파일 삭제 여부 flag
clock_t begin_time;// 사용자가 사라진 시점의 시간 체크
extern vector<string> list;
extern string path;
HistoryView historyview;//히스토리 객체 사용
extern bool fitting_first_popup;
extern float picture_save_time;

// RGB영상 생성 후 출력(스켈레톤 프레임을 포함한다.)
int CreateRGBImage::createRGBImage(HANDLE h, IplImage* Color){

	// RGB영상 프레임 초기화
	const NUI_IMAGE_FRAME *pImageFrame = NULL;
	// 스켈레톤 프레임 초기화
	NUI_SKELETON_FRAME skeletonFrame = { 0 };

	// 다음 영상을 지정된 시간 이후로 값을 가져온다.
	HRESULT hr = NuiImageStreamGetNextFrame(h, 1000, &pImageFrame);

	// RGB영상 생성이 실패했을 경우
	if (FAILED(hr)){
		cout << "Create RGB Image Failed\n" << endl;
		return -1;
	}

	// 프레임 데이터에 액세스
	INuiFrameTexture *pTexture = pImageFrame->pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect(0, &LockedRect, NULL, 0);

	// 행의 데이터 바이트가 0이 아닐경우 영상을 출력한다.
	if (LockedRect.Pitch != 0){
		// 바이트로 변환
		BYTE * pBuffer = (BYTE*)LockedRect.pBits;
		// memcpy()와 같은 기능으로서 BYTE* 형태로 저장된 이미지 버퍼를 IplImage* 형태의 이미지 버퍼에 데이터를 채운다.
		cvSetData(Color, pBuffer, LockedRect.Pitch);
		
		// 스켈레톤 데이터 프레임을 얻음
		hr = NuiSkeletonGetNextFrame(0, &skeletonFrame);

		// 스켈레톤 데이터 확인
		for (int i = 0; i < NUI_SKELETON_COUNT; i++){

			// 스켈레톤 데이터 프레임을 정상적으로 받아왔는지 확인할 스켈레톤트래킹 상태변수 선언
			NUI_SKELETON_TRACKING_STATE state = skeletonFrame.SkeletonData[i].eTrackingState;
			
			// 스켈레톤 트래킹이 정상적으로 되었을 경우
			if (NUI_SKELETON_TRACKED == state){
				user_appreciate = TRUE;//사용자가 인식됬음을 flag에 반영
				user_out_check = FALSE;
				Skeleton sk;// 클래스 객체 생성(스켈레톤)
				sk.drawSkeleton(skeletonFrame.SkeletonData[i], Color);// 함수 호출(스켈레톤을 그린다.)

			}
			else {
				if (user_out_check == FALSE){//벗어나는 그 최초의 순간만을 체크한다.
					begin_time = clock();//그 벗어난 순간의 시간을 저장한다.
					user_out_check = TRUE;
				}
				else{
					delete_time = float(clock() - begin_time) / CLOCKS_PER_SEC;//벗어난 시간을 체크

					if (delete_time >= 10.0f){//사용자가 5초이상 벗어났을 시
						historyview.loadImage();//저장된 히스토리 사진 이름조회
						for (int i = 0; i < list.size(); i++){
							string temp = path + list[i];//사진 경로를 설정한다.
							if (remove(temp.c_str()) == 0){//사진 삭제
								cout << "히스토리 삭제 완료" << endl;
							}
							else{
								cout << "히스토리 삭제 실패" << endl;
							}
						}

						user_appreciate = FALSE;//유저가 사라진 것을 falg에 반영
						ad_on = TRUE;//광고 on
					}
				}
			}
		}
		//초기 사용자 신체 측정시의 가이드라인
		if (picture_save_time <3.0f && fitting_first_popup == true && user_coner_ok == false){
			cvCircle(Color, cvPoint(320, 50), 50, CV_RGB(255, 0, 0), 5); //머리
			cvLine(Color, cvPoint(320, 100), cvPoint(320, 300), CV_RGB(255, 0, 0), 5, 8, 0);//몸통선
			cvLine(Color, cvPoint(320, 120), cvPoint(150, 280), CV_RGB(255, 0, 0), 5, 8, 0);//왼쪽팔선
			cvLine(Color, cvPoint(320, 120), cvPoint(490, 280), CV_RGB(255, 0, 0), 5, 8, 0);//오른쪽팔선
			cvLine(Color, cvPoint(320, 300), cvPoint(280, 460), CV_RGB(255, 0, 0), 5, 8, 0);//왼쪽다리선
			cvLine(Color, cvPoint(320, 300), cvPoint(360, 460), CV_RGB(255, 0, 0), 5, 8, 0);//오른다리선
		}

		iplTexture = Color;
	}
	// 프레임 메모리 해제
	NuiImageStreamReleaseFrame(h, pImageFrame);

	return 0;
}