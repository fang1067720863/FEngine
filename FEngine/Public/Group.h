#pragma once
#include"Node.h"
#include<vector>
class FGroup:FNode
{
public:
	FGroup();
	virtual ~FGroup();
	unsigned int GetNodeNum()const;
	virtual unsigned int AddChild(FNode* pChild);
	virtual unsigned int DeleteChild(FNode* pChild);
	virtual bool DeleteChild(unsigned int i);
	FNode* GetChild(unsigned int i)const;
	virtual void DeleteAllChild();

protected:
	virtual void UpdateWorldBound(double dAppTime);//更新边界体 
	virtual void UpdateNodeAll(double dAppTime);
	typedef std::vector<Ptr<FNode>>  NodeList; 
	NodeList m_pChild;
};


