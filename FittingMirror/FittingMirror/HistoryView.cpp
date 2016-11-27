#include "HistoryView.h"
#include "CvOverlayImage.h"
#include "dirent.h"
#include <string>
#include <vector>
#include <string>

extern CvPoint points[NUI_SKELETON_POSITION_COUNT];//����� ���� ��ǥ��
extern IplImage *viewNowImage;//����� ������ �����ִ� �̹��� ����

//���� ������ ���� Ŭ���� ����
DIR *dir;
struct dirent *ent;

int index;//���� ���� ��ġ
string path = "D:/picture/";//���� ���� ���
string load;//���� ȣ�� �� ��μ��� ����
vector<string> list;//����� ���� �̸��迭

bool history_left_click;//���� Ŭ�� flag
bool history_right_click;//������ Ŭ�� flag

//list�� ũ�⸦ ����
int indexsize=0;

extern string str_user_id;//���� ����� ID

using namespace std;

void HistoryView::createHistoryView(){
	loadImage();
}

void HistoryView::loadImage(){
	//�������� ������ �����о� �ͼ� �迭�� ����
	
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
����, ���� ȭ��ǥ ���ý� �̺�Ʈ �޼ҵ�
*/
void HistoryView::moveImage(int way){
	if (way == 0){
		
		index--;
		if (index < 0){//���� ȭ��ǥ ��Ȱ��ȭ �� �̺�Ʈ ��ó��
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
		index++;//���� index�� ������Ų �Ŀ�
		if (index > list.size()-1){//������ �̹����� ��� ������ ȭ��ǥ ��Ȱ��ȭ �� �̺�Ʈ ��ó��
			history_right_click = FALSE;
			index = list.size()-1;
		}
		else{
			if (index == list.size() - 1)
				history_right_click = FALSE;

			history_left_click = TRUE;//���� ȭ��ǥ Ȱ��ȭ
			load = path + list[index];//��μ���
			cvReleaseImage(&viewNowImage);
			viewNowImage = cvLoadImage(load.c_str());//�̹��� �ε�
		}
	}	
}

/*������ ���� ���ε�*/
void HistoryView::saveServerImageUser(){
	system("javac FileUpload.java");
	cout << path + list[index] << endl;
	string pathtemp = "java FileUpload " + path + list[index] + " id="+ str_user_id;
	system(pathtemp.c_str());

}