#include "stdafx.h"
#include "SChartReal.h"
#include "Utils.h"
#include <algorithm>

#undef max
#undef min

SChartReal::SChartReal()
{

}


SChartReal::~SChartReal()
{

}
void SChartReal::SetLineShow(SStringT title, bool bShow)
{
	if (bShow)
	{
		auto finder = std::find(vLineShow.begin(), vLineShow.end(), title);
		if (finder == vLineShow.end())
		{
			vLineShow.push_back(title);
		}
	}
	else
	{
		auto finder = std::find(vLineShow.begin(), vLineShow.end(), title);
		if (finder != vLineShow.end())
		{
			vLineShow.erase(finder);
		}
	}
	UpdateData();
}

void SChartReal::SetHistory(bool bHistory)
{
	bHistory_ = bHistory;
}
bool SChartReal::isHistory()
{
	return bHistory_;
}

void SChartReal::CreateLine(SStringT Name, COLORREF Color)
{
	LineTag t;
	t.Color = Color;
	recordData_[Name] = t;
}

void SChartReal::AddData(const AdvanceData& data)
{
	Clear();
	for (auto& it : data.mapValue)
	{
		auto value = it.second.begin();
		auto time = data.vTime.begin();
		auto& values = recordData_[it.first].values;
		auto& times = recordData_[it.first].times;
		while (time != data.vTime.end())
		{
			values.push_back(*value);
			times.push_back(*time);
			time++;
			value++;
		}
	}
	UpdateData();
}



time_t SChartReal::Time2Stamp(std::string dateTime)
{
	tm temptm;
	memset(&temptm, 0, sizeof(temptm));
	sscanf(dateTime.c_str(), "%d/%d/%d %d:%d:%d",
		&temptm.tm_year, &temptm.tm_mon, &temptm.tm_mday,
		&temptm.tm_hour, &temptm.tm_min, &temptm.tm_sec);
	temptm.tm_year -= 1900;
	temptm.tm_mon--;
	time_t curTick = mktime(&temptm);
	return curTick;
}
std::string SChartReal::Stamp2Time(time_t t)
{
	char charTime[50] = {};
	struct tm* stm;
	t += 28800;
	stm = gmtime(&t);
	strftime(charTime, 50, "%Y/%m/%d %H:%M:%S", stm);
	return charTime;
}

void SChartReal::Clear()
{
	for (auto& it : recordData_)
	{
		it.second.points.clear();
		it.second.values.clear();
		it.second.times.clear();
	}
}



void SChartReal::OnPaint(IRenderTarget* pRT)
{
	if (rectData.Width() == 0)
		return;
	SPainter painter;
	BeforePaint(pRT, painter);

	CAutoRefPtr<SOUI::IPen> Pen;

	pRT->FillSolidRoundRect(&rectData, CPoint(2, 2), nDataBgColor);
	pRT->CreatePen(PS_SOLID, nCoordColor, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[0], 2);
	Pen->Release();
	pRT->CreatePen(PS_SOLID, nCoordColor, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[1], 2);
	Pen->Release();
	pRT->CreatePen(PS_SOLID, nCoordColor, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[2], 2);


	Pen->Release();
	pRT->CreatePen(PS_SOLID, nLineColor, nLineWidth, &Pen);
	pRT->SelectObject(Pen);
	for (size_t i = 0; i <= nScaleNumber; i++)
	{
		POINT Coord[2] = { 0 };
		Coord[0].x = rectData.left;
		Coord[0].y = (LONG)(rectData.bottom - fYRectScale * i);
		Coord[1].x = Coord[0].x + rectData.Width();
		Coord[1].y = Coord[0].y;

		pRT->DrawLines(Coord, 2);

		SIZE size;
		SStringT text = SStringT().Format(L"%.1f", yMinVal_ + (yMaxVal_ - yMinVal_) / 6 * i);
		pRT->MeasureText(text, text.GetLength(), &size);

		pRT->TextOut(Coord[0].x - size.cx - 15, Coord[0].y - size.cy / 2, text, -1);

	}
	{
		time_t endVal = xMaxVal_ - xMinVal_;

		for (time_t i = 0; i <= endVal; i += 300)
		{
			POINT Coord[2] = { 0 };
			Coord[0].x = (LONG)(rectData.left + i * fXRectScale);
			Coord[0].y = rectData.bottom;
			Coord[1].x = Coord[0].x;
			Coord[1].y = rectData.top;

			pRT->DrawLines(Coord, 2);

			SIZE size;
			SStringT timeText = S_CA2W(Stamp2Time(xMinVal_ + i).c_str());
			pRT->MeasureText(timeText, timeText.GetLength(), &size);
			pRT->TextOut(Coord[0].x - size.cx / 2, Coord[0].y + 15, timeText, -1);
		}
		POINT Coord[2] = { 0 };
		Coord[0].x = (LONG)(rectData.left + endVal * fXRectScale);
		Coord[0].y = rectData.bottom;
		Coord[1].x = Coord[0].x;
		Coord[1].y = rectData.top;

		pRT->DrawLines(Coord, 2);

		SIZE size;
		SStringT timeText = S_CA2W(Stamp2Time(xMinVal_ + endVal).c_str());
		pRT->MeasureText(timeText, timeText.GetLength(), &size);
		pRT->TextOut(Coord[0].x - size.cx / 2, Coord[1].y - 15 - size.cy, timeText, -1);
	}

	//每条线的绘制
	for (auto& title : vLineShow)
	{
		auto target = recordData_.find(title);
		if (target == recordData_.end())
			continue;
		Pen->Release();
		pRT->CreatePen(PS_SOLID, target->second.Color, 3, &Pen);
		pRT->SelectObject(Pen);
		size_t count = target->second.points.size();
		pRT->DrawLines(&target->second.points[0], count);

	}

	AfterPaint(pRT, painter);
}

void SChartReal::GetMaxMin()
{
	yMaxVal_ = yMinVal_ = 0.0f;
	xMaxVal_ = xMinVal_ = 0;
	for (const auto& target : vLineShow)
	{
		auto valBegin = recordData_[target].values.begin();
		auto valEnd = recordData_[target].values.end();;
		if (recordData_[target].times.size() == 0)
			continue;
		if (xMaxVal_ == xMinVal_ && xMaxVal_ == 0)
		{
			xMaxVal_ = Time2Stamp(*std::max_element(recordData_[target].times.begin(), recordData_[target].times.end()));
			xMinVal_ = Time2Stamp(*std::min_element(recordData_[target].times.begin(), recordData_[target].times.end()));
		}

		float tempMax = *std::max_element(valBegin, valEnd);
		float tempMin = *std::min_element(valBegin, valEnd);
		if (yMaxVal_ == yMinVal_ && yMaxVal_ == 0.0f)
		{
			yMaxVal_ = tempMax;
			yMinVal_ = tempMin;
		}
		else
		{
			yMaxVal_ = std::max(yMaxVal_, tempMax);
			yMinVal_ = std::min(yMinVal_, tempMin);
		}
	}
	float gap = yMaxVal_ - yMinVal_;
	if (gap == 0)
	{
		yMaxVal_ += 5.0;
		yMinVal_ -= 5.0;
	}
	else
	{
		yMaxVal_ += gap / 10;
		yMinVal_ -= gap / 10;
	}
}
void SChartReal::InitRect()
{
	rectClient = GetClientRect();
	ptCoord[0].x = rectClient.left + rectCoordMargin.left;
	ptCoord[0].y = rectClient.top + rectCoordMargin.top;
	ptCoord[1].x = ptCoord[0].x;
	ptCoord[1].y = rectClient.bottom - rectCoordMargin.bottom;
	ptCoord[2].x = rectClient.right - rectCoordMargin.right;
	ptCoord[2].y = ptCoord[1].y;
	ptCoord[3].x = ptCoord[2].x;
	ptCoord[3].y = ptCoord[0].y;

	rectData.left = ptCoord[0].x + nCoordWidth;
	rectData.top = ptCoord[0].y;
	rectData.right = ptCoord[2].x - nCoordWidth;
	rectData.bottom = ptCoord[1].y;

	fYRectScale = rectData.Height() / (float)(nScaleNumber);
	fXRectScale = rectData.Width() / (float)1801;
}
void SChartReal::UpdateData()
{
	if (rectClient.Width() == 0)
	{
		InitRect();
		if (fYRectScale == 0)
		{
			return;
		}
	}
	GetMaxMin();
	if (xMaxVal_ == 0)
		return;


	for (auto& title : vLineShow)
	{
		auto dataBegin = recordData_.find(title);
		if(dataBegin == recordData_.end())
			continue;

		dataBegin->second.points.clear();
		auto iterX = dataBegin->second.times.begin();
		auto enderX = dataBegin->second.times.end();
		auto iterY = dataBegin->second.values.begin();
		while (iterX != enderX)
		{
			POINT pt;
			float xVal = (Time2Stamp(*iterX) - xMinVal_) * fXRectScale;
			pt.x = (LONG)(rectData.left + xVal);
			pt.y = (LONG)(rectData.bottom - ((*iterY - yMinVal_) * rectData.Height() / (yMaxVal_ - yMinVal_)));
			dataBegin->second.points.push_back(pt);
			++iterX;
			++iterY;
		}
	}

	InvalidateRect(rectClient);
}
