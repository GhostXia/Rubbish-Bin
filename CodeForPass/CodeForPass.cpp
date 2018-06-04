#include <stdio.h>
#include <iostream>
#include <string>
#include<functional>

template<typename T>
class ChainList
{
	T* Data = NULL;
	ChainList<T>* pPoint = NULL;
public:
	ChainList(T Element);
	ChainList() = default;
	T& operator[](int place);
	T& operator*();
	void push_front(T Element);
	void push_back(T Element);
	void push(T Element, int place);
	void remove_front();
	void remove_back();
	void remove(int place);
	void travel(std::function<void(T&)> travel_function);
};

int main()
{
	ChainList<std::string> pPoint;
	while (1)
	{
		system("cls");
		printf("������\n1.��Ԫ��\n2.ȡԪ��\n3.ɾ��Ԫ��\n4.�������");
		int Mode;
		scanf("%d", &Mode);
		switch (Mode)
		{
		case 1:
			int Mode2;
			std::cout << "1.��ͷ������\n2.��β������\n3.��ĳһλ�ò���";
			std::cin >> Mode2;
			switch (Mode2)
			{
			case 1:
			{
				std::cout << "����Ҫ���������:";
				std::string TmpData;
				std::cin >> TmpData;
				pPoint.push_front(TmpData);
			}
			break;
			case 2:
			{
				std::cout << "����Ҫ���������:";
				std::string TmpData;
				std::cin >> TmpData;
				pPoint.push_back(TmpData);
			}
			break;
			case 3:
			{
				std::cout << "����Ҫ���������:";
				std::string TmpData;
				int place;
				std::cin >> TmpData;
				std::cout << "��������λ��:";
				std::cin >> place;
				pPoint.push(TmpData, place);
			}
			break;
			}
			break;
		case 2:
		{
			int place;
			std::cout << "������鿴Ԫ�ص��±�:";
			std::cin >> place;
			std::cout << pPoint[place];
		}
		break;
		case 3:
		{
			int Mode2;
			std::cout << "1.��ͷ��ɾ��\n2.��β��ɾ��\n3.ɾ��ĳһλ�õ�Ԫ��";
			std::cin >> Mode2;
			switch (Mode2)
			{
			case 1:
				pPoint.remove_front();
				break;
			case 2:
				pPoint.remove_back();
				break;
			case 3:
			{
				int place;
				std::cout << "����Ҫɾ����Ԫ�ص��±�:";
				std::cin >> place;
				pPoint.remove(place);
			}
			break;
			}
		}
		break;
		case 4:
			pPoint.travel([](std::string& Data) {std::cout << Data << "->"; });
			break;
		}
		std::cout << std::endl;
		system("pause");
	}
}

template<typename T>
ChainList<T>::ChainList(T InitElement)
{
	Data = new T(InitElement);
}

template<typename T>
T& ChainList<T>::operator[](int place)
{
	auto Tmp = pPoint;
	for (int i = 0; i < place&&Tmp->pPoint; i++)
		Tmp = Tmp->pPoint;
	return **Tmp;
}

template<typename T>
T& ChainList<T>::operator*()
{
	return *Data;
}

template<typename T>
void ChainList<T>::push_front(T Element)
{
	auto TmpP = new ChainList<T>(Element);
	TmpP->pPoint = pPoint;
	pPoint = TmpP;
}

template<typename T>
void ChainList<T>::push_back(T Element)
{
	if (!pPoint)
	{
		push_front(Element);
		return;
	}
	auto Tmp = pPoint;
	while (Tmp->pPoint)
		Tmp = Tmp->pPoint;
	Tmp->pPoint = new ChainList<T>(Element);
}

template<typename T>
void ChainList<T>::push(T Element, int place)
{
	auto Tmp = pPoint;
	for (int i = 0; i < place&&Tmp->pPoint; i++)
		Tmp = Tmp->pPoint;
	auto TmpP = new ChainList<T>(Element);
	TmpP->pPoint = Tmp;
	Tmp->pPoint = TmpP;
}

template<typename T>
void ChainList<T>::remove_front()
{
	auto TmpP = pPoint->pPoint;
	delete pPoint;
	pPoint = TmpP;
}

template<typename T>
void ChainList<T>::remove_back()
{
	auto Tmp = pPoint;
	if (!Tmp->pPoint)
	{
		remove_front();
		return;
	}
	while (Tmp->pPoint->pPoint)
		Tmp = Tmp->pPoint;
	delete Tmp->pPoint;
	Tmp->pPoint = NULL;
}

template<typename T>
void ChainList<T>::remove(int place)
{
	auto Tmp = pPoint;
	if (!place)
	{
		if (!Tmp->pPoint)
			return;
		remove_front();
	}
	for (int i = 0; i < place - 1 && Tmp->pPoint; i++)
		Tmp = Tmp->pPoint;
	auto TmpP = Tmp->pPoint->pPoint;
	delete Tmp->pPoint;
	Tmp->pPoint = TmpP;
}

template<typename T>
void ChainList<T>::travel(std::function<void(T&)> travel_function)
{
	auto Tmp = pPoint;
	while (Tmp)
	{
		travel_function(**Tmp);
		Tmp = Tmp->pPoint;
	}
}