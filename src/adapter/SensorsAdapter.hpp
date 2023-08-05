#pragma once
#include <helper/SAdapterBase.h>
#include "SystemDef.h"
#include <vector>

using namespace SOUI;
class SensorsAdapter :public SAdapterBase
{
public:

	virtual int getCount()
	{
		return (int)m_colTexts.size();
	}
	virtual void getView(int position, SWindow* pItem, pugi::xml_node xmlTemplate)
	{
		if (pItem->GetChildrenCount() == 0)
		{
			pItem->InitFromXml(xmlTemplate);
		}
		if (position % 2)
		{
			pItem->FindChildByName(L"wnd_bg")->SetAttribute(L"colorBkgnd", L"#D2D2D2FF");
		}
		else
		{
			pItem->FindChildByName(L"wnd_bg")->SetAttribute(L"colorBkgnd", L"#D2D2D200");
		}
		pItem->FindChildByName(L"time")->SetWindowText(S_CA2W(m_colTexts[position].time.c_str()));
		pItem->FindChildByName(L"val")->SetWindowText(SStringT().Format(L"%.2f", m_colTexts[position].value));
	}
	void Add(SensorsRecord& item)
	{
		m_colTexts.push_back(item);
	}
	void RemoveAll()
	{
		m_colTexts.clear();
	}

	std::vector<SensorsRecord> m_colTexts;
};