#include "HistoryView.h"
#include "CvOverlayImage.h"
#include "dirent.h"
#include <string>
#include <vector>
#include <string>

extern CvPoint points[NUI_SKELETON_POSITION_COUNT];//사용자 관절 좌표값
extern IplImage *viewNowImage;//저장된 사진을 보여주는 이미지 변수

//사진 저장을 위한 클래스 생성
DIR *dir;
struct dirent *ent;

int index;//현재 사진 위치
string path = "D:/picture/";//사진 저장 경로
string load;//사진 호출 시 경로설정 변수
vector<string> list;//저장된 사진 이름배열

bool history_left_click;//왼쪽 클릭 flag
bool history_right_click;//오른쪽 클릭 flag

//list의 크기를 저장
int indexsize=0;

extern string str_user_id;//현재 사용자 ID

using namespace std;

void HistoryView::createHistoryView(){
	loadImage();
}

void HistoryView::loadImage(){
	//폴더안의 파일을 전부읽어 와서 배열로 저장
	
	list.clear();
	if ((dir = opendir(path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == 32768){
				list.push_back(ent->d_name);
				index++;
			}
		}
		if (list.size() != 0){
			indexsize = index;
			index = 0;
			load = path + list[index];
			closedir(dir);
			viewNowImage = cvLoadImage(load.c_str());
			history_left_click = FALSE;
			if (list.size() == 1)
				history_right_click = FALSE;
			else
				history_right_click = TRUE;
		}
	}
	else {
		perror("");
	}
}

/*
이전, 다음 화살표 선택시 이벤트 메소드
*/
void HistoryView::moveImage(int way){
	if (way == 0){
		
		index--;
		if (index < 0){//왼쪽 화살표 비활성화 및 이벤트 비처리
			history_left_click = FALSE;
			index=0;
		}
		else{
			if (index == 0){
				history_left_click = FALSE;
			}
			
			history_right_click = TRUE;

			load = path + list[index];
			cvReleaseImage(&viewNowImage);
			viewNowImage = cvLoadImage(load.c_str());
		}
	}
	else{
		index++;//먼저 index를 증가시킨 후에
		if (index > list.size()-1){//마지막 이미지일 경우 오른쪽 화살표 비활성화 및 이벤트 비처리
			history_right_click = FALSE;
			index = list.size()-1;
		}
		else{
			if (index == list.size() - 1)
				history_right_click = FALSE;

			history_left_click = TRUE;//좌측 화살표 활성화
			load = path + list[index];//경로설정
			cvReleaseImage(&viewNowImage);
			viewNowImage = cvLoadImage(load.c_str());//이미지 로드
		}
	}	
}

/*서버에 사진 업로드*/
void HistoryView::saveServerImageUser(){
	system("javac FileUpload.java");
	cout << path + list[index] << endl;
	string pathtemp = "java FileUpload " + path + list[index] + " id="+ str_user_id;
	system(pathtemp.c_str());

}