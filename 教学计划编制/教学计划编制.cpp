#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

struct ClassInfo
{
	string Name;
	int Score=0;
};

struct GraphElement
{
	ClassInfo Data;
	vector<string> To;
	vector<string> From;
};

class Graph
{
	vector<GraphElement> Data;
public:
	void push_back(GraphElement GE);
	void Mixup();
	vector<ClassInfo> FindElementWith0FromNode();
	GraphElement& FindElementByData(string _Data);
	void RemoveElementByName(string _Data,bool ClearRef=true);
	bool IsElementExistByName(string Name);
	int GetElementNumber();
};

void AddClass();
void DeleteClass();
void Process();

Graph ClassGraph;
int main()
{
	while (1)
	{
		int mode;
		system("cls");
		printf("1.��ӿγ�\n2.ɾ���γ�\n3.�޸Ŀγ���Ϣ\n4.����γ��Ų�");
		scanf("%d", &mode);
		switch (mode)
		{
		case 1:
			AddClass();
			break;
		case 2:
			DeleteClass();
			break;
		case 3:
			AddClass();
			break; 
		case  4:
			Process();
			break;
		}
	}
}

void AddClass()
{
	printf("����γ̵����ƣ�ѧ�֣����ȵ��γ�,�γ���������0ʱֹͣ\n");
	while (1)
	{
		char ClassName[10240] = { 0 };
		char TmpPreClass[256];
		scanf("%s", ClassName);
		if (!strcmp(ClassName, "0"))
			break;
		if (ClassGraph.IsElementExistByName(ClassName))
			ClassGraph.RemoveElementByName(ClassName,false);
		GraphElement TmpClass;
		TmpClass.Data.Name = ClassName;
		scanf("%d", &TmpClass.Data.Score);
		//scanf("%[^\n]", ClassName);
		while(getchar() != '\n')
		{
			scanf("%s", TmpPreClass);
			TmpClass.From.push_back(TmpPreClass);
		}
		ClassGraph.push_back(TmpClass);
	}
	ClassGraph.Mixup();
}

void DeleteClass()
{
	printf("����Ҫɾ���Ŀγ�,������صĿγ̶���Ͽ������ſε���ϵ:");
	char ClassName[256];
	scanf("%s", ClassName);
	ClassGraph.RemoveElementByName(ClassName);
}

void Process()
{
	vector<vector<ClassInfo>> TermClass;
	Graph TmpClassGraph(ClassGraph);
	for (vector<ClassInfo> ZeroNode = TmpClassGraph.FindElementWith0FromNode(); ZeroNode.size(); ZeroNode = TmpClassGraph.FindElementWith0FromNode())
	{
		TermClass.push_back(vector<ClassInfo>());
		for (auto it = ZeroNode.begin(); it < ZeroNode.end(); ++it)
			TermClass[TermClass.size() - 1].push_back(*it);
		for (auto it = ZeroNode.begin(); it < ZeroNode.end(); ++it)
			TmpClassGraph.RemoveElementByName(it->Name);
	}
	if (TmpClassGraph.GetElementNumber())
	{
		printf("�γ�ͼ���л�·���޷���ɿγ̷���\n");
		system("pause");
		return;
	}
	for (int i = 0; i < TermClass.size(); i++)
	{
		int TermScore=0;
		for (auto &Info : TermClass[i])
			TermScore += Info.Score;
		printf("��%dѧ�ڵĿγ�(��%dѧ��):\n", i+1,TermScore);
		for (int k = 0; k < TermClass[i].size(); k++)
			printf("%s\t\t%dѧ��\n", TermClass[i][k].Name.c_str(),TermClass[i][k].Score);
	}
	system("pause");
}

void Graph::push_back(GraphElement GE)
{
	Data.push_back(GE);
}

void Graph::Mixup()
{
	for (auto it = Data.begin(); it < Data.end(); ++it)
		it->To.clear();
	for (auto it = Data.begin(); it < Data.end(); ++it)
		for (auto it2 = Data.begin(); it2 < Data.end(); ++it2)
			if (find(it2->From.begin(), it2->From.end(), it->Data.Name) != it2->From.end())
				it->To.push_back(it2->Data.Name);
}

vector<ClassInfo> Graph::FindElementWith0FromNode()
{
	vector<ClassInfo> ZeroNode;
	for (auto it=Data.begin();it<Data.end();++it)
	{
		if (!it->From.size())
			ZeroNode.push_back(it->Data);
	}
	return ZeroNode;
}

GraphElement & Graph::FindElementByData(string _Data)
{
	for (auto it=Data.begin();it<Data.end();++it)
		if (it->Data.Name==_Data)
			return *it;
}

void Graph::RemoveElementByName(string _Data, bool ClearRef)
{
	for (auto it = Data.begin(); it < Data.end(); ++it)
		if (it->Data.Name == _Data)
		{
			if (ClearRef)
				for (auto & i : Data)
				{
					auto TmpIt = find(i.From.begin(), i.From.end(), _Data);
					if (TmpIt != i.From.end())
						i.From.erase(TmpIt);
				}
			Data.erase(it);
			break;
		}
	Mixup();
}

bool Graph::IsElementExistByName(string Name)
{
	for (auto it = Data.begin(); it < Data.end(); ++it)
		if (it->Data.Name == Name)
			return true;
	return false;
}

int Graph::GetElementNumber()
{
	return Data.size();
}
