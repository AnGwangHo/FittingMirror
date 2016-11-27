// 헤더파일 참조
#include "CvOverlayImage.h"

// cv이미지를 RGB영상에 출력(src = RGB영상, overlay = 옷 이미지, location = cv이미지 좌표, 해당 화소 값, 해당 화소 값)
void CvOverlayImage::cvOverlayImage(IplImage *src, IplImage *overlay, CvPoint location, CvScalar S, CvScalar D){

	// RGB영상 좌표 계산에 사용할 x,y변수와 for문에 사용할 i변수 선언
	int x, y, i;

	try{
		// 옷 이미지를 영상에 나타내기 위해 해당 이미지에 대한 CvScalar값을 넣어준다.
		for (x = 0; x < overlay->width; x++)
		{
			// x좌표에 대한 범위 오류를 방지
			if (x + location.x >= src->width || (src->width - src->width) > location.x) 	continue;
			for (y = 0; y < overlay->height; y++)
			{
				// y좌표에 대한 범위 오류를 방지
				if (y + location.y >= src->height || (src->height - src->height) > location.y) continue;
				// 인자값으로 받은 이미지영상의 location(x, y)에서 칼라값을 얻어온다.
				CvScalar source = cvGet2D(src, y + location.y, x + location.x);
				// 인자값으로 받은 옷 이미지(현재 옷)에서 칼라값을 얻어온다.
				CvScalar over = cvGet2D(overlay, y, x);
				// 영상에 출력할 옷 이미지의 벡터값을 계산할 CvScalar변수
				CvScalar merged;

				// 옷 이미지에서 화이트 픽셀은 제외하는 부분d
				if (over.val[0] == 255 && over.val[1] == 255 && over.val[2] == 255)
				{
					for (i = 0; i < 4; i++)
						merged.val[i] = (source.val[i]);
				}
				// 그 외
				else
				{
					for (i = 0; i < 4; i++)
						merged.val[i] = (over.val[i]);
				}
				// 변경된 값을 처리한다.(옷 이미지가 포함된 영상을 출력한다.)
				cvSet2D(src, y + location.y, x + location.x, over);
			}
		}
	}
	// 예외처리 부분
	catch (Exception e){}
}