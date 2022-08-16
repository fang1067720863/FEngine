#pragma once
#include"Node.h"
#include<vector>
class FGroup:public FNode
{
public:
	FGroup();
	FGroup(const std::string& name):FNode(name){}
	
	virtual ~FGroup();
	unsigned int GetNodeNum()const;
	virtual size_t AddChild(const Ptr<FNode>& pChild);
	virtual unsigned int DeleteChild(FNode* pChild);
	virtual bool DeleteChild(unsigned int i);
	FNode* GetChild(unsigned int i)const;
	FNode* GetChild(const std::string& name)const;
	virtual void DeleteAllChild();
	virtual void Draw()
	{
		for (auto& child : m_pChildren) child->Draw();
	}
	virtual void Update(float dt)
	{
		for (auto& child : m_pChildren) child->Update(dt);
	}
protected:
	virtual void UpdateWorldBound(double dAppTime);//更新边界体 
	virtual void UpdateNodeAll(double dAppTime);
	typedef std::vector<Ptr<FNode>>  NodeList; 
	NodeList m_pChildren;
};


