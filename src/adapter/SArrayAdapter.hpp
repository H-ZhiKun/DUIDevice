#pragma once
#include <helper/SAdapterBase.h>
#include <string>
using namespace SOUI;
class SArrayAdapter :public SAdapterBase 
{ 
	//用来保存所以行数据 
	SArray<std::wstring> m_colTexts;
	std::function<void(std::wstring)> m_callBackItemClick;
public: 
	void SetItemClickCallBack(std::function<void(std::wstring)> fun)
	{
		m_callBackItemClick = fun;
	}
	virtual int getCount()
	{ 
		return static_cast<int>(m_colTexts.GetCount());
	}
	virtual void getView(int position, SWindow* pItem, pugi::xml_node xmlTemplate)
	{ 
		if (pItem->GetChildrenCount() == 0) 
		{ 
			pItem->InitFromXml(xmlTemplate); 
			pItem->GetEventSet()->subscribeEvent(EventItemPanelClick::EventID, Subscriber(&SArrayAdapter::OnEventItemClick, this));
		}
		pItem->FindChildByName(L"text")->SetWindowText(m_colTexts[position].c_str()); 
	}
	void Add(LPCWSTR word) 
	{ 
		m_colTexts.Add(word); 
	}
	void RemoveAll() 
	{
		m_colTexts.RemoveAll();
	} 
	bool OnEventItemClick(EventArgs* e)
	{
		EventItemPanelClick* pEvt = sobj_cast<EventItemPanelClick>(e);
		if (NULL == pEvt) return true;

		SItemPanel* pItem = sobj_cast<SItemPanel>(pEvt->sender);
		if (NULL == pItem) return true;

		int nItem = static_cast<int>(pItem->GetItemIndex());
		if (m_callBackItemClick)
		{
			m_callBackItemClick(m_colTexts[nItem]);
		}
		return true;
	}
};