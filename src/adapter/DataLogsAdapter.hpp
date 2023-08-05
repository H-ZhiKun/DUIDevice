#pragma once
#include <helper/SAdapterBase.h>
using namespace SOUI;
class DataLogsAdapter :public SAdapterBase
{
	//用来保存所以行数据 
public:
	virtual int getCount()
	{
		return static_cast<int>(m_colTexts.size());
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
		pItem->FindChildByName(L"time")->SetWindowTextW(S_CA2W(m_colTexts[position].time.c_str()));
		pItem->FindChildByName(L"temp_avg")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].avg_temp));
		pItem->FindChildByName(L"press_avg")->SetWindowText(
			SStringT().Format(L"%.2f", m_colTexts[position].avg_pressure));

		pItem->FindChildByName(L"R600")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r600));
		pItem->FindChildByName(L"R300")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r300));
		pItem->FindChildByName(L"R200")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r200));
		pItem->FindChildByName(L"R100")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r100));
		pItem->FindChildByName(L"R60")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r60));
		pItem->FindChildByName(L"R30")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r30));
		pItem->FindChildByName(L"R20")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r20));
		pItem->FindChildByName(L"R10")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r10));
		pItem->FindChildByName(L"R6")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r6));
		pItem->FindChildByName(L"R3")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r3));
		pItem->FindChildByName(L"R0")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].r0));

		pItem->FindChildByName(L"den_max")->SetWindowText(
			SStringT().Format(L"%.0f", m_colTexts[position].max_density));
		pItem->FindChildByName(L"den_avg")->SetWindowText(
			SStringT().Format(L"%.0f", m_colTexts[position].avg_density));
		pItem->FindChildByName(L"den_min")->SetWindowText(
			SStringT().Format(L"%.0f", m_colTexts[position].min_density));

		pItem->FindChildByName(L"av")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].av));
		pItem->FindChildByName(L"pv")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].pv));
		pItem->FindChildByName(L"yp")->SetWindowText(
			SStringT().Format(L"%.1f", m_colTexts[position].yp));
		pItem->FindChildByName(L"nhb")->SetWindowText(
			SStringT().Format(L"%.4f", m_colTexts[position].nhb));
		pItem->FindChildByName(L"khb")->SetWindowText(
			SStringT().Format(L"%.4f", m_colTexts[position].khb));
		pItem->FindChildByName(L"npl")->SetWindowText(
			SStringT().Format(L"%.4f", m_colTexts[position].npl));
		pItem->FindChildByName(L"kpl")->SetWindowText(
			SStringT().Format(L"%.4f", m_colTexts[position].kpl));
		pItem->FindChildByName(L"ty")->SetWindowText(
			SStringT().Format(L"%.2f", m_colTexts[position].tau0));
	}
	void Add(RecordData& data)
	{
		m_colTexts.push_back(data);
	}
	void RemoveAll()
	{
		m_colTexts.clear();
	}
	std::vector<RecordData> m_colTexts;
};