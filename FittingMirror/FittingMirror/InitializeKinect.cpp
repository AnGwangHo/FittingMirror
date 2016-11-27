// ������� ����
#include "InitializeKinect.h"

// Ű��Ʈ ���� ���� �ʱ�ȭ
void InitializeKinect::initializeKinect(){

	// Ű��Ʈ ������ üũ�ϴ� �Ҹ��𺯼� ����
	bool failToConnect;

	// Ű��Ʈ�� ���������� ����Ǿ����� �˻�
	do{
		// HRESULT:������Ÿ��(Ű��Ʈ�� RGB����� ���̷��� ��� ����)
		HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);

		// Ű��Ʈ ������ �������� ���
		if (FAILED(hr)){
			// �ܼ�ȭ�� �ʱ�ȭ
			system("cls");
			// �ܼ�ȭ�鿡 ���� ���
			cout << "\nFailed to Connect!\n\n";
			// ������ üũ�ϴ� �Ҹ��𺯼��� true�� �ʱ�ȭ
			failToConnect = true;
			// �ߴ� ���� ���("����Ϸ��� �ƹ�Ű�� �����ʽÿ�")
			system("PAUSE");
		}
		// Ű��Ʈ ������ ���������� �� ���
		else{
			// �ܼ�ȭ�鿡 ���� ���
			cout << "\nConnection Established!\n\n";
			// ������ üũ�ϴ� �Ҹ��𺯼��� false�� �ʱ�ȭ
			failToConnect = false;
		}
		// ������ üũ�ϴ� �Ҹ��𺯼��� true�� ���� ��� �ݺ�
	} while (failToConnect);

}