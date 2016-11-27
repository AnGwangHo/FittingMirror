// 헤더파일 참조
#include "InitializeKinect.h"

// 키넥트 연결 설정 초기화
void InitializeKinect::initializeKinect(){

	// 키넥트 연결을 체크하는 불리언변수 선언
	bool failToConnect;

	// 키넥트가 정상적으로 연결되었는지 검사
	do{
		// HRESULT:데이터타입(키넥트의 RGB영상과 스켈레톤 사용 선언)
		HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);

		// 키넥트 연결이 실패했을 경우
		if (FAILED(hr)){
			// 콘솔화면 초기화
			system("cls");
			// 콘솔화면에 문장 출력
			cout << "\nFailed to Connect!\n\n";
			// 연결을 체크하는 불리언변수를 true로 초기화
			failToConnect = true;
			// 중단 문장 출력("계속하려면 아무키나 누르십시오")
			system("PAUSE");
		}
		// 키넥트 연결이 정상적으로 된 경우
		else{
			// 콘솔화면에 문장 출력
			cout << "\nConnection Established!\n\n";
			// 연결을 체크하는 불리언변수를 false로 초기화
			failToConnect = false;
		}
		// 연결을 체크하는 불리언변수가 true인 동안 계속 반복
	} while (failToConnect);

}