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

unsigned int FGroup::AddChild(FNode* pChild)
{
	return 0;
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

void FGroup::DeleteAllChild()
{
}

void FGroup::UpdateWorldBound(double dAppTime)
{
}

void FGroup::UpdateNodeAll(double dAppTime)
{
}
