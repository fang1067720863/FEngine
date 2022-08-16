#include "Group.h"

FGroup::FGroup()
{
}

FGroup::~FGroup()
{
}

unsigned int FGroup::GetNodeNum() const
{
	return 0;
}

size_t FGroup::AddChild(const Ptr<FNode>& pChild)
{
	m_pChildren.push_back(pChild);
	return m_pChildren.size()-1;
}

unsigned int FGroup::DeleteChild(FNode* pChild)
{
	return 0;
}

bool FGroup::DeleteChild(unsigned int i)
{
	return false;
}

FNode* FGroup::GetChild(unsigned int i) const
{
	return nullptr;
}
FNode* FGroup::GetChild(const std::string& name)const
{
	for (auto& child : m_pChildren)
	{
		if (child->GetName() == name)
		{
			return child.get();
		}
	}
	return nullptr;
}

void FGroup::DeleteAllChild()
{
}

void FGroup::UpdateWorldBound(double dAppTime)
{
}

void FGroup::UpdateNodeAll(double dAppTime)
{
}
