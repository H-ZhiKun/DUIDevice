#include "stdafx.h"
#include "SChartTime.h"


SChartTime::SChartTime()
{

}


SChartTime::~SChartTime()
{

}

void SChartTime::CreatedLine(SStringT Name)
{
	recordData_[Name].Color = mapColors[Name];
}

void SChartTime::AddData(SStringT Name, float yValue)
{
	recordData_[Name].yValues.push_back(yValue);
}
void SChartTime::AddTime(SStringT time)
{
	xValues.push_back(time);
}
void  SChartTime::Clear()
{
	for (auto& it : recordData_)
	{
		xValues.clear();
		it.second.yValues.clear();
		it.second.points.clear();
	}
}


void SChartTime::OnMouseMove(UINT nFlags, CPoint point)
{
	if (rectData.PtInRect(point))
	{
		textTips.clear();
		ptCursorPoint = point;
		auto iter = recordData_.begin();
		if (iter != recordData_.end())
		{
			size_t count = iter->second.points.size();
			
			for (int i = 0; i < count; i++)
			{
				if (iter->second.points[i].x + 3 >= ptCursorPoint.x
					&& iter->second.points[i].x - 3 <= ptCursorPoint.x)
				{
					textTips.push_back(SStringT().Format(L"Time: %s", xValues[i]));
					textTips.push_back(SStringT().Format(L"%s: %.1f", iter->first, iter->second.yValues[i]));
					iter++;
					textTips.push_back(SStringT().Format(L"%s: %.1f", iter->first, iter->second.yValues[i]));
					break;
				}
			}
		}
		
		InvalidateRect(rcClient);
	}
}

void SChartTime::OnMouseLeave()
{
	ptCursorPoint.x = ptCursorPoint.y = 0;
	textTips.clear();
	InvalidateRect(rcClient);
}



void SChartTime::OnPaint(IRenderTarget* pRT)
{

	rcClient = GetClientRect();
	//绘制坐标轴
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

	SPainter painter;
	BeforePaint(pRT, painter);

	//绘制文本
	IFont* iFont = (IFont*)pRT->GetCurrentObject(OT_FONT);
	const LOGFONT* logfont = iFont->LogFont();
	HDC hdc = pRT->GetDC();
	Gdiplus::Graphics gdi(hdc);

	SStringT strleftTitle = leftTitle_.GetText(TRUE);
	SStringT strrightTitle = rightTitle_.GetText(TRUE);

	Gdiplus::Font font(hdc, logfont);
	Gdiplus::SolidBrush brush(pRT->GetTextColor());
	Gdiplus::RectF rectText;
	gdi.MeasureString(strleftTitle, -1, &font, Gdiplus::PointF(0, 0), &rectText);
	gdi.TranslateTransform((Gdiplus::REAL)rcClient.left + 15, rectData.top + rectData.Height() / 2 - rectText.Height / 2);
	gdi.RotateTransform(-90);
	gdi.DrawString(strleftTitle, -1, &font,
		Gdiplus::PointF(-rectText.Width / 2, 0), &brush);
	//取消旋转
	gdi.ResetTransform();
	gdi.MeasureString(strrightTitle, -1, &font, Gdiplus::PointF(0, 0), &rectText);
	gdi.TranslateTransform(rcClient.right - 15 - rectText.Height, rectData.top + rectData.Height() / 2 - rectText.Height / 2);
	gdi.RotateTransform(-90);
	gdi.DrawString(strrightTitle, -1, &font,
		Gdiplus::PointF(-rectText.Width / 2, 0), &brush);
	//取消旋转
	gdi.ResetTransform();


	CAutoRefPtr<SOUI::IPen> Pen;

	pRT->FillSolidRoundRect(&rectData, CPoint(2, 2), nDataBgColor);
	pRT->CreatePen(PS_SOLID, recordData_[L"PV"].Color, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[0], 2);
	pRT->CreatePen(PS_SOLID, nCoordColor, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[1], 2);
	pRT->CreatePen(PS_SOLID, recordData_[L"YP"].Color, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[2], 2);


	if (recordData_.begin()->second.yValues.size() > 0)
	{
		//绘制X轴上标线与文字
		SIZE xTextSize;
		pRT->MeasureText(xMinTime, xMinTime.GetLength(), &xTextSize);
		pRT->TextOut(rectData.left, rectData.bottom + 5, xMinTime, -1);
		pRT->MeasureText(xMaxTime, xMaxTime.GetLength(), &xTextSize);
		pRT->TextOut(rectData.right - xTextSize.cx, rectData.bottom + 5, xMaxTime, -1);


		Pen->Release();
		pRT->CreatePen(PS_SOLID, nLineColor, 1, &Pen);
		pRT->SelectObject(Pen);
		for (size_t i = 1; i < nScaleNumber; i++)
		{
			POINT Coord[2] = { 0 };
			Coord[0].x = rectData.left;
			Coord[0].y = (LONG)(rectData.bottom - fYRectScale * i);
			Coord[1].x = Coord[0].x + rectData.Width();
			Coord[1].y = Coord[0].y;

			pRT->DrawLines(Coord, 2);

			SIZE size;
			SStringT text = SStringT().Format(L"%.1f", lMin + leftValScale * i);
			pRT->MeasureText(text, text.GetLength(), &size);
			pRT->TextOut(Coord[0].x - size.cx - 15, Coord[0].y - size.cy / 2, text, -1);
			text = SStringT().Format(L"%.1f", rMin + rightValScale * i);
			pRT->MeasureText(text, text.GetLength(), &size);
			pRT->TextOut(Coord[1].x + 15, Coord[1].y - size.cy / 2, text, -1);
		}

		//每条线的绘制
		auto pos = recordData_.begin();
		while (pos != recordData_.end())
		{
			Pen->Release();
			pRT->CreatePen(PS_SOLID, pos->second.Color, nLineWidth, &Pen);
			pRT->SelectObject(Pen);
			size_t count = pos->second.points.size();

			pRT->DrawLines(&pos->second.points[0], count);

			for (int i = 0; i < count; i++)
			{
				CAutoRefPtr<SOUI::IBrush> pBrush;
				pRT->CreateSolidColorBrush(pos->second.Color, &pBrush);
				pRT->SelectObject(pBrush);
				CRect Rect;
				Rect.left = pos->second.points[i].x - nDataPointSize / 2;
				Rect.top = pos->second.points[i].y - nDataPointSize / 2;
				Rect.bottom = Rect.top + nDataPointSize;
				Rect.right = Rect.left + nDataPointSize;
				pRT->FillSolidEllipse(Rect, pos->second.Color);
			}
			pos++;
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
			pRT->CreatePen(PS_SOLID, nLineColor, 1, &pPen);
			pRT->SelectObject(pPen);
			pRT->DrawLines(CursorLinePos, 2);
		}

	}

	AfterPaint(pRT, painter);
}


void SChartTime::SetXMaxMin(SStringT& Max, SStringT& Min)
{
	xMaxTime = Max;
	xMinTime = Min;
}

void SChartTime::SetYMaxMin(float leftMax, float leftMin, float rightMax, float rightMin)
{
	lMax = leftMax;
	lMin = leftMin;
	rMax = rightMax;
	rMin = rightMin;
}


void SChartTime::GetYMaxMin(float& Max, float& Min)
{
	if (recordData_.begin()->second.yValues.size() == 0) return;
	Max = Min = recordData_.begin()->second.yValues[0];
	for (auto& it : recordData_)
	{
		for (int i = 0; i < it.second.yValues.size(); i++)
		{
			if (Max < it.second.yValues[i])
			{
				Max = it.second.yValues[i];
			}
			if (Min > it.second.yValues[i])
			{
				Min = it.second.yValues[i];
			}
		}
	}
}


void SChartTime::UpdateData()
{
	if (recordData_.size()==0 || recordData_.begin()->second.yValues.size() == 0) return;
	rcClient = GetClientRect();
	if (rcClient.Width() <= 0) return;

	xMax = Time2Stamp(S_CW2A(xMaxTime).GetBuffer(0));
	xMin = Time2Stamp(S_CW2A(xMinTime).GetBuffer(0));

	leftValScale = (lMax - lMin) / (float)(nScaleNumber);
	rightValScale = (rMax - rMin) / (float)(nScaleNumber);
	fYRectScale = rectData.Height() / (float)(nScaleNumber);


	for (auto& data : recordData_)
	{
		float yMax = 0, yMin = 0;
		if (data.first == L"PV")
		{
			yMax = lMax;
			yMin = lMin;
		}
		else
		{
			yMax = rMax;
			yMin = rMin;
		}
		auto iterX = xValues.begin();
		auto iterY = data.second.yValues.begin();
		while (iterX != xValues.end())
		{
			POINT pt;
			float xVal = (Time2Stamp(S_CW2A(*iterX).GetBuffer(0)) - xMin) * (float)rectData.Width() / (xMax - xMin);
			pt.x = (LONG)(rectData.left + xVal);
			pt.y = (LONG)(rectData.bottom - ((*iterY - yMin) * rectData.Height() / (yMax - yMin)));
			if (xVal < 0 || pt.y < rectData.top || pt.y > rectData.bottom)
			{
				iterX = xValues.erase(iterX);
				iterY = data.second.yValues.erase(iterY);
				continue;
			}
			data.second.points.push_back(pt);
			iterX++;
			iterY++;
		}
	}
	InvalidateRect(rcClient);
}

time_t SChartTime::Time2Stamp(std::string dateTime)
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