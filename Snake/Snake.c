#include <Windows.h>
#include "Snake.h"

static FOOD Food;
static SNAKE Snake;


int GetScore(void)
{
	return max(Snake.length - 2, 0);//���ص÷֣��ߵĳ���-��ʼ��ʱ�ĳ���
}
//��ʼ��ʳ�ʳ�ﻹ������
void FoodInit(void)
{
	Food.exist = FALSE;
}
//��ʼ����
void SnakeInit(void)
{	//��ͷ��λ��
	Snake.body[0].x = HORZ_BLK_NUMS - 2;
	Snake.body[0].y = VERT_BLK_NUMS - 2;
	//��β��λ��
	Snake.body[1].x = HORZ_BLK_NUMS - 1;
	Snake.body[1].y = VERT_BLK_NUMS - 2;
	Snake.length = 2;//�ߵĳ���
	return;
}

//����ʳ��
BOOL NewFood(void)
{
	int i, x, y;
	BOOL Success;

	if (!Food.exist && Snake.length != LENGTH)//�Ƿ���ʳ��,���Ƿ��пռ������ʾʳ��
	{
		for (;;)
		{	//�������ʳ���λ��
			Success = TRUE;
			x = rand() / (RAND_MAX / HORZ_BLK_NUMS);
			y = rand() / (RAND_MAX / VERT_BLK_NUMS);
			for (i = 0; i < Snake.length; i++)
			{	//ʳ��λ���Ƿ���������
				if (Snake.body[i].x == x && Snake.body[i].y == y)
				{	//λ�����ߵ����ϣ��������ɣ�
					Success = FALSE;
					break;
				}
			}
			if (Success)
			{	//���ɳɹ���
				Food.x = x;
				Food.y = y;
				Food.exist = TRUE;
				return TRUE;
			}
		}
	}
	if (Snake.length == LENGTH)
		return TRUE;//���޿ռ���ʾʳ��,������·���
	else
		return FALSE;//����ʳ�����
}
//����ʳ��ͷ���
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
	wsprintf(temp, TEXT("������%d"), max(Snake.length - 2, 0));
	TextOut(hdc, 0, 0, temp, wcslen(temp));
	SelectObject(hdc, hfont);

	return;
}

//����
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
//���ƶ�
int SnakeWalk(HWND hwnd, RECT rect, int iBlockSize, HFONT hfont, int iDirection)
{
	int i;
	HDC hdc;
	BOOL bCanWalk = TRUE;
	POINT Head, Tail;

	hdc = GetDC(hwnd);

	Head = Snake.body[0];//��ͷ
	Tail = Snake.body[Snake.length - 1];//��β

	switch (iDirection)
	{
	case VK_UP://������
		Head.y -= 1;
		break;
	case VK_DOWN://������
		Head.y += 1;
		break;
	case VK_LEFT://������
		Head.x -= 1;
		break;
	case VK_RIGHT://������
		Head.x += 1;
		break;
	}
	if (Head.x < 0 || Head.x > HORZ_BLK_NUMS - 1 || Head.y < 0 || Head.y > VERT_BLK_NUMS - 1)
		bCanWalk = FALSE;//�������ˣ�
	for (i = 3; bCanWalk && i< Snake.length - 1; i++)
	if (Head.x == Snake.body[i].x && Head.y == Snake.body[i].y)
		bCanWalk = FALSE;//��ҧ���Լ��ˣ�
	if (bCanWalk)
	{
		if (Head.x == Food.x && Head.y == Food.y)
		{//�߳Ե���ʳ��
			Food.exist = FALSE;
			Snake.length++;
		}
		for (i = Snake.length - 1; i>0; i--)
			Snake.body[i] = Snake.body[i - 1];//����ǰ�ƶ�һ��
		Snake.body[0] = Head;//��ͷ����λ��

		SelectObject(hdc, GetStockObject(WHITE_PEN));//�����ԭ������β
		Rectangle(hdc,
			rect.left + iBlockSize * Tail.x + 1,
			rect.top + iBlockSize * Tail.y + 1,
			rect.left + iBlockSize * (Tail.x + 1) - 1,
			rect.top + iBlockSize * (Tail.y + 1) - 1);
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		DrawSnake(hdc, rect, iBlockSize);//���»�����
	}
	if (NewFood())
		DrawFoodAndScore(hdc, rect, iBlockSize, hfont);//����ʳ�Ｐ����

	ReleaseDC(hwnd, hdc);
	if (bCanWalk) //�ɹ��ƶ������ر����ƶ�����
		return iDirection;
	return bCanWalk;
}