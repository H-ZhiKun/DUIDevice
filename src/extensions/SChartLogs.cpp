#include "stdafx.h"
#include "SChartLogs.h"
#include <string>

#undef max
#undef min

SChartLogs::SChartLogs()
{

}


SChartLogs::~SChartLogs()
{

}
void SChartLogs::CreateLine(SStringT name)
{
	LineTag t;
	recordData_[name] = t;
}


void SChartLogs::AddData(SStringT name, float YValue, std::string& lvTime)
{
	recordData_[name].values.push_back(YValue);
	recordData_[name].times.push_back(lvTime);
}

void SChartLogs::AddComment(std::vector<CommentsData>& data)
{
	vCommentInfo.data = data;
}
void SChartLogs::InitRect()
{
	rcClient = GetClientRect();

	ptCoord[0].x = rcClient.left + rectCoordMargin.left;
	ptCoord[0].y = rcClient.top + rectCoordMargin.top;
	ptCoord[1].x = ptCoord[0].x;
	ptCoord[1].y = rcClient.bottom - rectCoordMargin.bottom;
	ptCoord[2].x = rcClient.right - rectCoordMargin.right;
	ptCoord[2].y = ptCoord[1].y;
	ptCoord[3].x = ptCoord[2].x;
	ptCoord[3].y = ptCoord[0].y;

	rectData.left = ptCoord[0].x + nCoordWidth;
	rectData.top = ptCoord[0].y;
	rectData.right = ptCoord[2].x - nCoordWidth;
	rectData.bottom = ptCoord[1].y;
}

void SChartLogs::UpdateData()
{
	if (rcClient.Width() == 0)
		InitRect();
	GetMaxMin();
	if (xMaxTime == xMinTime)
	{
		InvalidateRect(rcClient);
		return;
	}
	time_t xMax = Time2Stamp(xMaxTime);
	time_t xMin = Time2Stamp(xMinTime);
	float xScale = static_cast<float>(rectData.Width()) / (xMax - xMin);
	float yScaleLeft = 0.0f, yScaleRight = 0.0f;
	if (LMaxVal > LMinVal)
		yScaleLeft = static_cast<float>(rectData.Height()) / (LMaxVal - LMinVal);
	if (RMaxVal > RMinVal)
		yScaleRight = static_cast<float>(rectData.Height()) / (RMaxVal - RMinVal);
	for (auto& [Color, label] : mapLabelColor)
	{
		auto& pos = recordData_.find(label);
		if (pos == recordData_.end()) continue;
		std::vector<POINT> vPt;
		float valMin = 0.0f;
		float yScale = 0.0f;
		if (label.Find(L"Density") == -1)
		{
			yScale = yScaleLeft;
			valMin = LMinVal;
		}
		else
		{
			yScale = yScaleRight;
			valMin = RMinVal;
		}
		pos->second.points.clear();
		pos->second.ptIndex.clear();
		for (int i = 0; i < pos->second.values.size(); i++)
		{
			POINT pt;
			float xVal = (Time2Stamp(pos->second.times[i]) - xMin) * xScale;
			pt.x = (LONG)(rectData.left + xVal);
			pt.y = (LONG)(rectData.bottom - ((pos->second.values[i] - valMin) * yScale));
			if (i > 0)
			{
				POINT gap = pos->second.points.back();
				if (pt.x <= gap.x + 3 && pt.y <= gap.y + 3)
				{
					continue;
				}
			}
			pos->second.ptIndex.push_back(i);
			pos->second.points.push_back(pt);
		}
	}
	vCommentInfo.points.clear();
	for (int i = 0; i < vCommentInfo.data.size(); i++)
	{
		POINT pt;
		float xVal = (Time2Stamp(vCommentInfo.data[i].time) - xMin) * xScale;
		if (xVal < 0) continue;
		pt.x = (LONG)(rectData.left + xVal);
		pt.y = rectData.bottom;
		vCommentInfo.points.push_back(pt);
	}
	InvalidateRect(rcClient);
}


void SChartLogs::SChartLogs::Clear()
{
	for (auto& it : recordData_)
	{
		it.second.times.clear();
		it.second.values.clear();
		it.second.points.clear();
	}
}

void SChartLogs::CleanLabel()
{
	mapLabelColor.clear();
}

void SChartLogs::SetLabelAndColor(SStringT& label, COLORREF color)
{
	mapLabelColor.push_back({ color,label });
}

void SChartLogs::Clear(SStringT name)
{
	recordData_[name].times.clear();
	recordData_[name].values.clear();
	recordData_[name].points.clear();
}


void SChartLogs::OnMouseMove(UINT nFlags, CPoint point)
{
	SetMsgHandled(TRUE);
	if (rectData.PtInRect(point))
	{
		ptCursorPoint = point;
		textTips.clear();
		if (mapLabelColor.size() == 0) 
			return;
		SStringT strlabel = mapLabelColor.begin()->second;
		strlabel = strlabel.Mid(strlabel.Find(L"_") + 1);
		auto iter = recordData_.find(strlabel);
		for (int i = 0; i < iter->second.points.size(); i++)
		{
			if (iter->second.points[i].x + 3 >= ptCursorPoint.x
				&& iter->second.points[i].x - 3 <= ptCursorPoint.x)
			{
				int index = iter->second.ptIndex[i];
				if (textTips.size() == 0)
				{
					SStringT time = SStringT().Format(L"TIME: %s", S_CA2W(iter->second.times[index].c_str()));
					textTips.push_back(time);
				}
				for (auto& [color, label] : mapLabelColor)
				{
					label = label.Mid(label.Find(L"_") + 1);
					if(label == L"Cl-" || label == L"PH" || label == L"ES")
						continue;
					auto pos = recordData_.find(label);
					SStringT text = SStringT().Format(L"%s: %.2f", pos->first, pos->second.values[index]);
					textTips.push_back(text);
				}
				break;
			}
		}

		InvalidateRect(rcClient);
	}
}
void SChartLogs::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!bCommentsEdit)
	{
		int index = -1;
		for (int i = 0; i < vCommentInfo.points.size(); i++)
		{
			if (vCommentInfo.points[i].x + 4 >= point.x
				&& vCommentInfo.points[i].x - 4 <= point.x)
			{
				index = i;
				break;
			}
		}
		if (pCommentCallback)
		{
			if (index == -1)
			{
				pCommentCallback(bCommentsEdit, "", "");
			}
			else
			{
				pCommentCallback(bCommentsEdit, vCommentInfo.data[index].data, vCommentInfo.data[index].time);
			}
		}
	}
	else
	{
		if (pCommentCallback)
		{
			time_t xMax = Time2Stamp(xMaxTime);
			time_t xMin = Time2Stamp(xMinTime);
			time_t curTime = (point.x - rectData.left) * (xMax - xMin) / rectData.Width() + xMin;
			pCommentCallback(bCommentsEdit, "", Stamp2Time(curTime));
		}
	}

	SetMsgHandled(TRUE);
}

void SChartLogs::OnMouseLeave()
{
	textTips.clear();
	ptCursorPoint.x = ptCursorPoint.y = 0;
	InvalidateRect(rcClient);
	SetMsgHandled(TRUE);
}


time_t SChartLogs::Time2Stamp(std::string dateTime)
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


void SChartLogs::OnPaint(IRenderTarget* pRT)
{
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


	//绘制X轴上标线与文字
	SIZE xTextSize;
	SStringT xTextStart = S_CA2W(xMinTime.c_str());
	SStringT xTextEnd = S_CA2W(xMaxTime.c_str());
	pRT->MeasureText(xTextStart, xTextStart.GetLength(), &xTextSize);
	pRT->TextOut(rectData.left, rectData.bottom + 5, xTextStart, -1);
	pRT->MeasureText(xTextEnd, xTextEnd.GetLength(), &xTextSize);
	pRT->TextOut(rectData.right - xTextSize.cx, rectData.bottom + 5, xTextEnd, -1);

	//绘制Y轴上标线与文字
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

		if (i > 0)
			pRT->DrawLines(Coord, 2);

		if (fLeftValScale > 0)
		{
			SIZE size;
			SStringT text = SStringT().Format(L"%.1f", LMinVal + fLeftValScale * i);
			pRT->MeasureText(text, text.GetLength(), &size);
			pRT->TextOut(Coord[0].x - size.cx - 15, Coord[0].y - size.cy / 2, text, -1);
		}
		if (fRightValScale > 0)
		{
			SIZE size;
			SStringT text = SStringT().Format(L"%.1f", RMinVal + fRightValScale * i);
			pRT->MeasureText(text, text.GetLength(), &size);
			pRT->TextOut(rectData.right + 15, Coord[0].y - size.cy / 2, text, -1);
		}
	}


	//每条线的绘制
	for (auto& [Color, label] : mapLabelColor)
	{
		auto pos = recordData_.find(label);
		if (pos != recordData_.end())
		{
			Pen->Release();
			pRT->CreatePen(PS_SOLID, Color, 1, &Pen);
			pRT->SelectObject(Pen);
			size_t count = pos->second.points.size();
			pRT->DrawLines(&pos->second.points[0], count);

			CAutoRefPtr<SOUI::IBrush> pBrush;
			pRT->CreateSolidColorBrush(Color, &pBrush);
			pRT->SelectObject(pBrush);
			for (size_t i = 0; i < count; i++)
			{
				CRect Rect;
				Rect.left = pos->second.points[i].x - nDataPointSize / 2;
				Rect.top = pos->second.points[i].y - nDataPointSize / 2;
				Rect.bottom = Rect.top + nDataPointSize;
				Rect.right = Rect.left + nDataPointSize;
				pRT->FillSolidEllipse(Rect, Color);
			}
		}
	}
	{
		Pen->Release();
		pRT->CreatePen(PS_SOLID, nCommentColor, 1, &Pen);
		pRT->SelectObject(Pen);
		CAutoRefPtr<SOUI::IBrush> pBrush;
		pRT->CreateSolidColorBrush(nCommentColor, &pBrush);
		pRT->SelectObject(pBrush);
		for (size_t i = 0; i < vCommentInfo.points.size(); i++)
		{
			CRect Rect;
			Rect.left = vCommentInfo.points[i].x - 5;
			Rect.top = vCommentInfo.points[i].y - 5;
			Rect.bottom = Rect.top + 10;
			Rect.right = Rect.left + 10;
			pRT->FillSolidEllipse(Rect, nCommentColor);
		}
	}


	if (textTips.size())
	{
		CRect rectTip;
		SIZE szTime;
		pRT->MeasureText(textTips[0], textTips[0].GetLength(), &szTime);
		size_t tipCount = textTips.size();
		rectTip.top = ptCursorPoint.y + 10;
		rectTip.left = ptCursorPoint.x + 10;
		rectTip.bottom = (LONG)(rectTip.top + (szTime.cy + 5) * tipCount + 10);
		rectTip.right = rectTip.left + szTime.cx + 20;
		//调整提示框位置
		if (rectTip.bottom > rcClient.bottom) rectTip.OffsetRect(0, -(rectTip.bottom - rcClient.bottom));
		if (rectTip.right > rcClient.right) rectTip.OffsetRect(-(rectTip.right - rcClient.right), 0);
		//绘制提示框	  
		pRT->FillSolidRoundRect(rectTip, CPoint(2, 2), nTipBgColor);
		pRT->TextOut(rectTip.left + 10, rectTip.top + 10, textTips[0], -1);

		POINT pt;
		pt.x = rectTip.left + 10;
		pt.y = rectTip.top + 10;
		for (int tipIndex = 1; tipIndex < textTips.size(); tipIndex++)
		{
			pt.y = pt.y + szTime.cy + 5;
			pRT->TextOut(pt.x, pt.y, textTips[tipIndex], -1);
		}

	}

	//对于鼠标的响应 //线上的点需要对齐才能生效
	if (ptCursorPoint.x != 0)
	{
		CPoint CursorLinePos[2];
		//绘制一条跟随光标的线
		CursorLinePos[1].x = CursorLinePos[0].x = ptCursorPoint.x;
		CursorLinePos[0].y = rectData.top;
		CursorLinePos[1].y = rectData.bottom;

		CAutoRefPtr<SOUI::IPen> pPen;
		pRT->CreatePen(PS_SOLID, bCommentsEdit ? nCommentColor : nLineColor, nLineWidth, &pPen);
		pRT->SelectObject(pPen);
		pRT->DrawLines(CursorLinePos, 2);
	}
	AfterPaint(pRT, painter);
}


void SChartLogs::GetMaxMin()
{
	LMaxVal = LMinVal = 0;
	RMaxVal = RMinVal = 0;
	xMinTime = xMaxTime = "";
	for (auto& [Color, label] : mapLabelColor)
	{
		auto pos = recordData_.find(label);
		if (pos == recordData_.end())
			continue;
		if (pos->second.times.begin() == pos->second.times.end())
			continue;
		auto timeMax = *pos->second.times.rbegin();
		auto timeMin = *pos->second.times.begin();
		if (xMaxTime == xMinTime)
		{
			xMaxTime = timeMax;
			xMinTime = timeMin;
		}
		else
		{
			xMaxTime = std::max(xMaxTime, timeMax);
			xMinTime = std::min(xMinTime, timeMin);
		}

		if (label.Find(L"Density") == -1)
		{
			float tempMax = *std::max_element(pos->second.values.begin(), pos->second.values.end());
			float tempMin = *std::min_element(pos->second.values.begin(), pos->second.values.end());
			if (LMaxVal == LMinVal && LMaxVal == 0)
			{
				LMaxVal = tempMax;
				LMinVal = tempMin;
			}
			else
			{
				LMaxVal = std::max(tempMax, LMaxVal);
				LMinVal = std::min(tempMin, LMinVal);
			}
		}
		else
		{
			float tempMax = *std::max_element(pos->second.values.begin(), pos->second.values.end());
			float tempMin = *std::min_element(pos->second.values.begin(), pos->second.values.end());
			if (RMaxVal == RMinVal && RMinVal == 0)
			{
				RMaxVal = tempMax;
				RMinVal = tempMin;
			}
			else
			{
				RMaxVal = std::max(tempMax, RMaxVal);
				RMinVal = std::min(tempMin, RMinVal);
			}
		}
	}
	if (LMaxVal == LMinVal)
	{
		LMaxVal += 5;
		LMinVal -= 5;
	}
	else
	{
		LMaxVal += (LMaxVal - LMinVal) / 10;
		LMinVal -= (LMaxVal - LMinVal) / 10;
	}
	fLeftValScale = (LMaxVal - LMinVal) / (float)(nScaleNumber);
	fYRectScale = rectData.Height() / (float)(nScaleNumber);

	if (RMaxVal > 0)
	{
		RMaxVal += 20;
		RMinVal -= 20;
		fRightValScale = (RMaxVal - RMinVal) / (float)(nScaleNumber);
	}
	else
	{
		fRightValScale = 0;
	}

}


void SChartLogs::SetCommentsCallback(std::function<void(bool, std::string, std::string)> callback)
{
	pCommentCallback = callback;
}
void SChartLogs::SetCommentsEdit(bool bEdit)
{
	bCommentsEdit = bEdit;
	//InvalidateRect(rcClient);
}