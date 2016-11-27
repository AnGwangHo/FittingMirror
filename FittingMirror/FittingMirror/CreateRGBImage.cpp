#include "CreateRGBImage.h"
#include <direct.h>//���� ������ ���� �������
#include "HistoryView.h"//�����丮 ��ȸ �������

// ���ùڽ� �̹��� 

extern IplImage* iplTexture;//������� �����

extern bool history_choice, history_exit;
extern bool picture_save;
extern IplImage *viewNowImage;

extern bool user_coner_ok;//����� ��� ���� �Ϸ� flag
bool user_guide_ok_btn = TRUE;//����ڰ� �������̵带 Ȯ�� ���� �� flag
bool user_guide_coner_btn = FALSE;//����ڰ� ���� ��ư�� ���� �Ͽ����� flag
bool user_appreciate;// ����ڰ� ���� �νĵǴ��� ���θ� üũ�ϴ� flag
bool user_out_check;// ����ڰ� Ű��Ʈ���� ��������� �ð�üũ�� ���� flag
bool ad_on;//����ڰ� ������� �ٽ� ������ �̹����� load���������
bool ad_off;//���� ���� ���
float delete_time;//���� ���� ���� flag
clock_t begin_time;// ����ڰ� ����� ������ �ð� üũ
extern vector<string> list;
extern string path;
HistoryView historyview;//�����丮 ��ü ���
extern bool fitting_first_popup;
extern float picture_save_time;

// RGB���� ���� �� ���(���̷��� �������� �����Ѵ�.)
int CreateRGBImage::createRGBImage(HANDLE h, IplImage* Color){

	// RGB���� ������ �ʱ�ȭ
	const NUI_IMAGE_FRAME *pImageFrame = NULL;
	// ���̷��� ������ �ʱ�ȭ
	NUI_SKELETON_FRAME skeletonFrame = { 0 };

	// ���� ������ ������ �ð� ���ķ� ���� �����´�.
	HRESULT hr = NuiImageStreamGetNextFrame(h, 1000, &pImageFrame);

	// RGB���� ������ �������� ���
	if (FAILED(hr)){
		cout << "Create RGB Image Failed\n" << endl;
		return -1;
	}

	// ������ �����Ϳ� �׼���
	INuiFrameTexture *pTexture = pImageFrame->pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect(0, &LockedRect, NULL, 0);

	// ���� ������ ����Ʈ�� 0�� �ƴҰ�� ������ ����Ѵ�.
	if (LockedRect.Pitch != 0){
		// ����Ʈ�� ��ȯ
		BYTE * pBuffer = (BYTE*)LockedRect.pBits;
		// memcpy()�� ���� ������μ� BYTE* ���·� ����� �̹��� ���۸� IplImage* ������ �̹��� ���ۿ� �����͸� ä���.
		cvSetData(Color, pBuffer, LockedRect.Pitch);
		
		// ���̷��� ������ �������� ����
		hr = NuiSkeletonGetNextFrame(0, &skeletonFrame);

		// ���̷��� ������ Ȯ��
		for (int i = 0; i < NUI_SKELETON_COUNT; i++){

			// ���̷��� ������ �������� ���������� �޾ƿԴ��� Ȯ���� ���̷���Ʈ��ŷ ���º��� ����
			NUI_SKELETON_TRACKING_STATE state = skeletonFrame.SkeletonData[i].eTrackingState;
			
			// ���̷��� Ʈ��ŷ�� ���������� �Ǿ��� ���
			if (NUI_SKELETON_TRACKED == state){
				user_appreciate = TRUE;//����ڰ� �νĉ����� flag�� �ݿ�
				user_out_check = FALSE;
				Skeleton sk;// Ŭ���� ��ü ����(���̷���)
				sk.drawSkeleton(skeletonFrame.SkeletonData[i], Color);// �Լ� ȣ��(���̷����� �׸���.)

			}
			else {
				if (user_out_check == FALSE){//����� �� ������ �������� üũ�Ѵ�.
					begin_time = clock();//�� ��� ������ �ð��� �����Ѵ�.
					user_out_check = TRUE;
				}
				else{
					delete_time = float(clock() - begin_time) / CLOCKS_PER_SEC;//��� �ð��� üũ

					if (delete_time >= 10.0f){//����ڰ� 5���̻� ����� ��
						historyview.loadImage();//����� �����丮 ���� �̸���ȸ
						for (int i = 0; i < list.size(); i++){
							string temp = path + list[i];//���� ��θ� �����Ѵ�.
							if (remove(temp.c_str()) == 0){//���� ����
								cout << "�����丮 ���� �Ϸ�" << endl;
							}
							else{
								cout << "�����丮 ���� ����" << endl;
							}
						}

						user_appreciate = FALSE;//������ ����� ���� falg�� �ݿ�
						ad_on = TRUE;//���� on
					}
				}
			}
		}
		//�ʱ� ����� ��ü �������� ���̵����
		if (picture_save_time <3.0f && fitting_first_popup == true && user_coner_ok == false){
			cvCircle(Color, cvPoint(320, 50), 50, CV_RGB(255, 0, 0), 5); //�Ӹ�
			cvLine(Color, cvPoint(320, 100), cvPoint(320, 300), CV_RGB(255, 0, 0), 5, 8, 0);//���뼱
			cvLine(Color, cvPoint(320, 120), cvPoint(150, 280), CV_RGB(255, 0, 0), 5, 8, 0);//�����ȼ�
			cvLine(Color, cvPoint(320, 120), cvPoint(490, 280), CV_RGB(255, 0, 0), 5, 8, 0);//�������ȼ�
			cvLine(Color, cvPoint(320, 300), cvPoint(280, 460), CV_RGB(255, 0, 0), 5, 8, 0);//���ʴٸ���
			cvLine(Color, cvPoint(320, 300), cvPoint(360, 460), CV_RGB(255, 0, 0), 5, 8, 0);//�����ٸ���
		}

		iplTexture = Color;
	}
	// ������ �޸� ����
	NuiImageStreamReleaseFrame(h, pImageFrame);

	return 0;
}