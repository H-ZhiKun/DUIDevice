#pragma once
#include <helper/SAdapterBase.h>
#include "MysqlClient.h"
#include "Utils.h"
#include <vector>

using namespace SOUI;
class DiagAdapter :public SAdapterBase
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
		pItem->FindChildByName(L"text_time")->SetWindowText(m_colTexts[position].timeStart_);
		pItem->FindChildByName(L"text_time_end")->SetWindowText(m_colTexts[position].timeEnd_);
		pItem->FindChildByName(L"text_code")->SetWindowText(m_colTexts[position].code_);
		pItem->FindChildByName(L"text_des")->SetWindowText(m_colTexts[position].des_);
	}
	void Add(DiagDataItem&& item)
	{
		m_colTexts.push_back(item);
	}
	void RemoveAll()
	{
		m_colTexts.clear();
	}

	std::vector<DiagDataItem> m_colTexts;
};