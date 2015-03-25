#include <Windows.h>
#include "Snake.h"

static FOOD Food;
static SNAKE Snake;


int GetScore(void)
{
	return max(Snake.length - 2, 0);//返回得分：蛇的长度-初始化时的长度
}
//初始化食物，食物还不存在
void FoodInit(void)
{
	Food.exist = FALSE;
}
//初始化蛇
void SnakeInit(void)
{	//蛇头的位置
	Snake.body[0].x = HORZ_BLK_NUMS - 2;
	Snake.body[0].y = VERT_BLK_NUMS - 2;
	//蛇尾的位置
	Snake.body[1].x = HORZ_BLK_NUMS - 1;
	Snake.body[1].y = VERT_BLK_NUMS - 2;
	Snake.length = 2;//蛇的长度
	return;
}

//生成食物
BOOL NewFood(void)
{
	int i, x, y;
	BOOL Success;

	if (!Food.exist && Snake.length != LENGTH)//是否还有食物,及是否还有空间可以显示食物
	{
		for (;;)
		{	//随机生成食物的位置
			Success = TRUE;
			x = rand() / (RAND_MAX / HORZ_BLK_NUMS);
			y = rand() / (RAND_MAX / VERT_BLK_NUMS);
			for (i = 0; i < Snake.length; i++)
			{	//食物位置是否在蛇身上
				if (Snake.body[i].x == x && Snake.body[i].y == y)
				{	//位置在蛇的身上，重新生成！
					Success = FALSE;
					break;
				}
			}
			if (Success)
			{	//生成成功！
				Food.x = x;
				Food.y = y;
				Food.exist = TRUE;
				return TRUE;
			}
		}
	}
	if (Snake.length == LENGTH)
		return TRUE;//已无空间显示食物,但需更新分数
	else
		return FALSE;//已有食物存在
}
//绘制食物和分数
void DrawFoodAndScore(HDC hdc, RECT rect, int iBlockSize, HFONT hfont)
{
	TCHAR temp[20];

	if (Food.exist)
		Ellipse(hdc,
		rect.left + iBlockSize * Food.x + 1,
		rect.top + iBlockSize * Food.y + 1,
		rect.left + iBlockSize * (Food.x + 1) - 1,
		rect.top + iBlockSize * (Food.y + 1) - 1);

	hfont = (HFONT)SelectObject(hdc, hfont);
	wsprintf(temp, TEXT("分数：%d"), max(Snake.length - 2, 0));
	TextOut(hdc, 0, 0, temp, wcslen(temp));
	SelectObject(hdc, hfont);

	return;
}

//画蛇
void DrawSnake(HDC hdc, RECT rect, int iBlockSize)
{
	int i;

	for (i = 0; i < Snake.length; i++)
		Rectangle(hdc,
		rect.left + iBlockSize * Snake.body[i].x + 1,
		rect.top + iBlockSize * Snake.body[i].y + 1,
		rect.left + iBlockSize * (Snake.body[i].x + 1) - 1,
		rect.top + iBlockSize * (Snake.body[i].y + 1) - 1);

	return;
}
//蛇移动
int SnakeWalk(HWND hwnd, RECT rect, int iBlockSize, HFONT hfont, int iDirection)
{
	int i;
	HDC hdc;
	BOOL bCanWalk = TRUE;
	POINT Head, Tail;

	hdc = GetDC(hwnd);

	Head = Snake.body[0];//蛇头
	Tail = Snake.body[Snake.length - 1];//蛇尾

	switch (iDirection)
	{
	case VK_UP://向上走
		Head.y -= 1;
		break;
	case VK_DOWN://向下走
		Head.y += 1;
		break;
	case VK_LEFT://向左走
		Head.x -= 1;
		break;
	case VK_RIGHT://向右走
		Head.x += 1;
		break;
	}
	if (Head.x < 0 || Head.x > HORZ_BLK_NUMS - 1 || Head.y < 0 || Head.y > VERT_BLK_NUMS - 1)
		bCanWalk = FALSE;//蛇碰壁了！
	for (i = 3; bCanWalk && i< Snake.length - 1; i++)
	if (Head.x == Snake.body[i].x && Head.y == Snake.body[i].y)
		bCanWalk = FALSE;//蛇咬到自己了！
	if (bCanWalk)
	{
		if (Head.x == Food.x && Head.y == Food.y)
		{//蛇吃到了食物
			Food.exist = FALSE;
			Snake.length++;
		}
		for (i = Snake.length - 1; i>0; i--)
			Snake.body[i] = Snake.body[i - 1];//蛇向前移动一步
		Snake.body[0] = Head;//蛇头的新位置

		SelectObject(hdc, GetStockObject(WHITE_PEN));//清除掉原来的蛇尾
		Rectangle(hdc,
			rect.left + iBlockSize * Tail.x + 1,
			rect.top + iBlockSize * Tail.y + 1,
			rect.left + iBlockSize * (Tail.x + 1) - 1,
			rect.top + iBlockSize * (Tail.y + 1) - 1);
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		DrawSnake(hdc, rect, iBlockSize);//重新绘制蛇
	}
	if (NewFood())
		DrawFoodAndScore(hdc, rect, iBlockSize, hfont);//绘制食物及分数

	ReleaseDC(hwnd, hdc);
	if (bCanWalk) //成功移动，返回本次移动方向
		return iDirection;
	return bCanWalk;
}