#pragma once
#include <d2d1.h>
#include <vector>

#define CARD_WIDTH 90
#define CARD_HEIGHT 145
#define CARDSTACK_X 90
#define CARDSTACK_Y 100
#define CARDSTACK_SPACE 105
#define TARGETSLOT_X 405
#define TARGETSLOT_Y 100
#define TARGETSLOT_SPACE 105
#define DESKSLOT_X 90
#define DESKSLOT_Y 300
#define DESKSLOT_SPACE 105


namespace CardSlots
{
	extern std::vector<char> CardStack;
	extern char ActivingCardNumber;
	extern char TargetSlots[4];
	extern char DeskSlots[7];
};

enum class CardSuit
{
	Spade=0, //����
	Heart=1,  //����
	Diamond=2, //��Ƭ
	Club=3 //÷��
};

class CardAnimator
{
	 class Card* TargetCard;
	D2D1_RECT_F  From;
	D2D1_RECT_F To;
	D2D1_POINT_2F PositionTarget;
	//0-1
	float Value=0;
	float RequestTime;
public:
	CardAnimator(class Card* TargetCard, D2D1_RECT_F  From, D2D1_POINT_2F PositionTarget, float RequestTime);

	//���Animator�Ѳ�����β��������true
	//����true��ζ�Ŵ�Animator�ѿ�����
	bool UpdateAnimationAndRender();
	void ForceEnd();
};

class Card
{
	//S->"����(Spade)"
	//H->"����(Heart)"
	//D->"��Ƭ(Diamond)"
	//C->"÷��(Club)"
	CardSuit Suit;

	//���֣�A-K:1-13
	char Number=0;
	ID2D1Bitmap* FrontImage=NULL;

	//-1��-4���ң����ң���Ƭ��÷����Ŀ��λ��
	//0:�ƶ�
	//1-6:��������ҵ�λ��
	char Place=0;

	//�����Ƿ�ɼ�
	bool Showing=false;

	//����(�ѵ���������֮�ϵ���)
	//��Ϊֻ��56���ƣ�����Ӧ������Ҫ�����ϲ��Ƶ�ָ�룬ȡ�ϲ��Ʊ�������
	Card* ChildCard=NULL;

	//��һ�ε����ʱ�䣬���ڷ���˫���¼�
	float LastClickTime=0;

	//�Ƶ�ǰλ��
	D2D1_POINT_2F Position;
	
	//�Ƶ���һ����ȷλ��
	D2D1_POINT_2F LastPosition;

	//Z��
	char ZOrder;
	
	friend CardAnimator;
	//���ƶ�����ÿ����ֻ�ܸ���һ������ʵ��
	CardAnimator* CardAnimation=NULL;
	void DoubleClick();//˫���߼�
public:
	//S->"����(Spade)"
	//H->"����(Heart)"
	//D->"��Ƭ(Diamond)"
	//C->"÷��(Club)"
	Card(CardSuit Suit, char Number, WCHAR* ImagePath,char Place, char ZOrder);
	
	void Render();
	void Move(D2D1_POINT_2F Target);
	
	bool CanDrag();
	void TryShow();

	//��Ҫ�����Ƿ���ԭ��λ��
	//����Ƿ���˫���¼�
	void Release();

	bool ApplyChildTo(Card* CardToApply);
	void ApplyAnimator(D2D1_POINT_2F Target);
	void RecordPosition();
	//Set
	//////////////////////////////////////////////////////////////////////////
	void SetZOrder(char NewZOrder);
	void SetShowing(bool NewShowing);
	void SetChild(Card* NewChildCard);
	void SetPlace(char NewPlace);
	void SetPosition(D2D1_POINT_2F NewPosition);
	//////////////////////////////////////////////////////////////////////////

	//Get
	//////////////////////////////////////////////////////////////////////////
	char GetZOrder();
	D2D1_POINT_2F GetPosition();
	char GetNumber();
	CardSuit GetSuit();
	bool IsShowing();
	Card* GetChildCard();
	char GetPlace();
	//////////////////////////////////////////////////////////////////////////
};

class PlayerController
{
	//���е���
	Card* HoldingCard;
	//����һ�μ�¼ʱ�����ָ���λ��
	D2D1_POINT_2F Position;
public:
	void UpdatePosition(D2D1_POINT_2F CurrentPosition);
	void MouseDown();
	void MouseUp();
	D2D1_POINT_2F GetPosition();
	Card* GetHoldingCard();
};