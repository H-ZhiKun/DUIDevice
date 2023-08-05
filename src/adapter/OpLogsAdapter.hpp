#pragma once
#include <helper/SAdapterBase.h>
#include <vector>
using namespace SOUI;
class OpLogsAdapter :public SAdapterBase 
{ 
	//用来保存所以行数据 
public: 
	struct OpLogsDataItem
	{
		SStringT time;
		SStringT user;
		SStringT event;
	};
	virtual int getCount()
	{ 
		return static_cast<int>(colTexts_.size());
	}
	virtual void getView(int position, SWindow* pItem, pugi::xml_node xmlTemplate)
	{ 
		if (pItem->GetChildrenCount() == 0) 
		{ 
			pItem->InitFromXml(xmlTemplate); 
		}

		pItem->FindChildByName(L"time")->SetWindowText(colTexts_[position].time);
		pItem->FindChildByName(L"user")->SetWindowText(colTexts_[position].user);
		pItem->FindChildByName(L"event")->SetWindowText(colTexts_[position].event);
	}
	void Add(SStringT time, SStringT user, SStringT event)
	{
		OpLogsDataItem item;
		item.time = time;
		item.user = user;
		item.event = event;
		colTexts_.push_back(std::move(item));
	}
	void RemoveAll() 
	{
		colTexts_.clear();
	}
	std::vector<OpLogsDataItem> colTexts_;
};