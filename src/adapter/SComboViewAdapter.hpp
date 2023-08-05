#pragma once
#include <helper/SAdapterBase.h>
#include <vector>

using namespace SOUI;
class SComboViewAdapter :public SAdapterBase
{ 
public: 
	SComboViewAdapter() {};
	SComboViewAdapter(SComboView* pView) 
	{
		pOwner_ = pView;
	};
	virtual int getCount()
	{ 
		return static_cast<int>(vItemData_.size());
	}
	virtual void getView(int position, SWindow* pItem, pugi::xml_node xmlTemplate)
	{ 
		if (pItem->GetChildrenCount() == 0) 
		{ 
			pItem->InitFromXml(xmlTemplate); 
		}
		pItem->FindChildByName(L"text")->SetWindowText(vItemData_[position]);
	}
	virtual SStringT getItemDesc(int position) 
	{
		return vItemData_[position];
	}
	void AddItem(SStringT text) 
	{ 
		for (const auto& it : vItemData_)
		{
			if (it == text)
			{
				return;
			}
		}
		vItemData_.push_back(std::move(text));
	}
	void RemoveItem(SStringT text)
	{
		vItemData_.erase(std::find(vItemData_.begin(), vItemData_.end(),text));
	}
	void RemoveHead()
	{
		vItemData_.erase(vItemData_.begin());
	}
	void SetOwner(SComboView* pView)
	{
		pOwner_ = pView;
	}
	void Clear()
	{
		vItemData_.clear();
	} 
	void SetSel(int sel = -1)
	{
		pOwner_->SetCurSel(sel);
	}
	void SetSel(SStringT selName)
	{
		int sel = -1;
		if (!selName.IsEmpty())
		{
			for (int i = 0; i < vItemData_.size(); i++)
			{
				if (selName == vItemData_[i])
				{
					sel = i;
					break;
					
				}
			}
		}
		pOwner_->SetCurSel(sel);
	}
	std::vector<SStringT> GetData()
	{
		return vItemData_;
	}
	void CopyData(SComboViewAdapter* adapter)
	{
		std::swap(vItemData_, adapter->GetData());
	}
private:
	std::vector<SStringT> vItemData_;
	SComboView* pOwner_ = nullptr;
};