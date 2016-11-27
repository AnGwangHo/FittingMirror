// ������� ����
#include "CvOverlayImage.h"

// cv�̹����� RGB���� ���(src = RGB����, overlay = �� �̹���, location = cv�̹��� ��ǥ, �ش� ȭ�� ��, �ش� ȭ�� ��)
void CvOverlayImage::cvOverlayImage(IplImage *src, IplImage *overlay, CvPoint location, CvScalar S, CvScalar D){

	// RGB���� ��ǥ ��꿡 ����� x,y������ for���� ����� i���� ����
	int x, y, i;

	try{
		// �� �̹����� ���� ��Ÿ���� ���� �ش� �̹����� ���� CvScalar���� �־��ش�.
		for (x = 0; x < overlay->width; x++)
		{
			// x��ǥ�� ���� ���� ������ ����
			if (x + location.x >= src->width || (src->width - src->width) > location.x) 	continue;
			for (y = 0; y < overlay->height; y++)
			{
				// y��ǥ�� ���� ���� ������ ����
				if (y + location.y >= src->height || (src->height - src->height) > location.y) continue;
				// ���ڰ����� ���� �̹��������� location(x, y)���� Į���� ���´�.
				CvScalar source = cvGet2D(src, y + location.y, x + location.x);
				// ���ڰ����� ���� �� �̹���(���� ��)���� Į���� ���´�.
				CvScalar over = cvGet2D(overlay, y, x);
				// ���� ����� �� �̹����� ���Ͱ��� ����� CvScalar����
				CvScalar merged;

				// �� �̹������� ȭ��Ʈ �ȼ��� �����ϴ� �κ�d
				if (over.val[0] == 255 && over.val[1] == 255 && over.val[2] == 255)
				{
					for (i = 0; i < 4; i++)
						merged.val[i] = (source.val[i]);
				}
				// �� ��
				else
				{
					for (i = 0; i < 4; i++)
						merged.val[i] = (over.val[i]);
				}
				// ����� ���� ó���Ѵ�.(�� �̹����� ���Ե� ������ ����Ѵ�.)
				cvSet2D(src, y + location.y, x + location.x, over);
			}
		}
	}
	// ����ó�� �κ�
	catch (Exception e){}
}