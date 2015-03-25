/*-------------------
	̰����ͷ�ļ�
	-------------------*/
#pragma comment(lib,"msimg32.lib")

#define HORZ_BLK_NUMS		35	//��Ϸ��ˮƽ��λ��
#define VERT_BLK_NUMS		24	//��Ϸ����ֱ��λ��
#define LENGTH		((HORZ_BLK_NUMS)*(VERT_BLK_NUMS))	//�ߵ���󳤶�
#define ID_TIMER	1

//ʳ��
typedef struct {
	//λ��
	int x;
	int y;
	//�Ƿ���ڣ��ж��Ƿ���Ҫ�����µ�
	BOOL exist;
}FOOD;

//��
typedef struct{
	POINT body[LENGTH];//�ߵ�����
	int length;//�ߵĳ���
}SNAKE;

int GetScore(void);//ȡ�õ�ǰ�÷�
void FoodInit(void);//��ʼ��ʳ��
void SnakeInit(void);//��ʼ����
void DrawSnake(HDC hdc, RECT rect, int iBlockSize);//������
void DrawFoodAndScore(HDC hdc, RECT rect, int iBlockSize, HFONT hfont);//����ʳ��ͷ���
int SnakeWalk(HWND hwnd, RECT rect, int iBlockSize, HFONT hfont, int iDirection);//���ƶ�







//Ӧ��xpЧ��
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif