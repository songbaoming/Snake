/*-------------------
	贪吃蛇头文件
	-------------------*/
#pragma comment(lib,"msimg32.lib")

#define HORZ_BLK_NUMS		35	//游戏区水平单位数
#define VERT_BLK_NUMS		24	//游戏区垂直单位数
#define LENGTH		((HORZ_BLK_NUMS)*(VERT_BLK_NUMS))	//蛇的最大长度
#define ID_TIMER	1

//食物
typedef struct {
	//位置
	int x;
	int y;
	//是否存在，判断是否需要生成新的
	BOOL exist;
}FOOD;

//蛇
typedef struct{
	POINT body[LENGTH];//蛇的身体
	int length;//蛇的长度
}SNAKE;

int GetScore(void);//取得当前得分
void FoodInit(void);//初始化食物
void SnakeInit(void);//初始化蛇
void DrawSnake(HDC hdc, RECT rect, int iBlockSize);//绘制蛇
void DrawFoodAndScore(HDC hdc, RECT rect, int iBlockSize, HFONT hfont);//绘制食物和分数
int SnakeWalk(HWND hwnd, RECT rect, int iBlockSize, HFONT hfont, int iDirection);//蛇移动







//应用xp效果
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