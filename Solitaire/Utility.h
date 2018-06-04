#pragma once
#include <windows.h>
#include <d2d1.h>

class GameUtility
{
	static float TimeValue;
public:
	static void UpdateTime();
	static float GetGameTime();
	static char GetUninitedCard(CardSuit& Suit);
	static char GetUninitedCardRandom(CardSuit& Suit);
	static char CardSuitToChar(CardSuit Suit);
	static bool LoadD2DImage(WCHAR ImagePath[], ID2D1Bitmap ** BitMapMayClear);
	static bool IsPointInRect(D2D1_POINT_2F Point, D2D1_RECT_F Rect);
	static char GetParentCard(Card* ChildCard);
	//-12~-9:�ĸ�Ŀ��λ��
	//-8:�ƶѵ�
	//-7~-1:�����ϴ�������7��λ��
	//0:���û�����κζ����ϣ�����հ״����ڴ����ڣ�
	//1-52:�Ƶ�REF����
	static char GetObjectUnderCursor();
};