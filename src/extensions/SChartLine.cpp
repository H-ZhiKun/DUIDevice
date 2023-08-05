#include "stdafx.h"
#include "SChartLine.h"


SChartLine::SChartLine()
{

}


SChartLine::~SChartLine()
{

}

void SChartLine::CreatedLine(SStringT Name, int line)
{
	recordData_[Name].Line = line;
	recordData_[Name].Color = mapColors[Name];
}

void SChartLine::AddData(SStringT Name, float xValue, float yValue)
{
	recordData_[Name].xValues.push_back(xValue);
	recordData_[Name].yValues.push_back(yValue);
}


void  SChartLine::Clear()
{
	for (auto& it : recordData_)
	{
		it.second.xValues.clear();
		it.second.yValues.clear();
		it.second.points.clear();
		it.second.nLineWidth = 2;
	}
}


void SChartLine::OnMouseMove(UINT nFlags, CPoint point)
{
	if (rectData.PtInRect(point))
	{
		textTips.clear();
		ptCursorPoint = point;
		auto iter = recordData_.find(L"Real");
		if (iter != recordData_.end())
		{
			size_t count = iter->second.points.size();
			for (int i = 0; i < count; i++)
			{
				if (iter->second.points[i].x + 3 >= ptCursorPoint.x
					&& iter->second.points[i].x - 3 <= ptCursorPoint.x)
				{

					SStringT yText = SStringT().Format(L"Shear Stress(Pa): %.1f", iter->second.yValues[i]);
					textTips.push_back(yText);
					SStringT xText = SStringT().Format(L"Shear Rate(s)	 : %.1f", iter->second.xValues[i]);
					textTips.push_back(xText);
					break;
				}
			}
		}
		InvalidateRect(rcClient);
	}
}

void SChartLine::OnMouseLeave()
{
	ptCursorPoint.x = ptCursorPoint.y = 0;
	textTips.clear();
	InvalidateRect(rcClient);
}



void SChartLine::OnPaint(IRenderTarget* pRT)
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

	SStringT strXTitle = xTitle_.GetText(TRUE);
	SStringT strYTitle = yTitle_.GetText(TRUE);

	Gdiplus::Font font(hdc, logfont);
	Gdiplus::SolidBrush brush(pRT->GetTextColor());
	Gdiplus::RectF rectText;
	gdi.MeasureString(strXTitle, -1, &font, Gdiplus::PointF(0, 0), &rectText);
	gdi.DrawString(strXTitle, -1, &font,
		Gdiplus::PointF(rectData.left + rectData.Width() / 2 - rectText.Width / 2,
			rcClient.bottom - rectText.Height - 15),
		&brush);
	gdi.MeasureString(strYTitle, -1, &font, Gdiplus::PointF(0, 0), &rectText);
	gdi.TranslateTransform((Gdiplus::REAL)rcClient.left + 15, rectData.top + rectData.Height() / 2 - rectText.Height / 2);
	gdi.RotateTransform(-90);
	gdi.DrawString(strYTitle, -1, &font,
		Gdiplus::PointF(-rectText.Width / 2, 0), &brush);

	//取消旋转
	gdi.ResetTransform();


	CAutoRefPtr<SOUI::IPen> Pen;

	pRT->FillSolidRoundRect(&rectData, CPoint(2, 2), nDataBgColor);
	pRT->CreatePen(PS_SOLID, nCoordColor, nCoordWidth, &Pen);
	pRT->SelectObject(Pen);
	pRT->DrawLines(&ptCoord[0], 2);
	pRT->DrawLines(&ptCoord[1], 2);


	if (recordData_.begin()->second.yValues.size() > 0)
	{
		Pen->Release();
		pRT->CreatePen(PS_SOLID, nLineColor, 1, &Pen);
		pRT->SelectObject(Pen);
		for (size_t i = 0; i <= nScaleNumber; i++)
		{
			{
				POINT Coord[2] = { 0 };
				Coord[0].x = rectData.left;
				Coord[0].y = (LONG)(rectData.bottom - fYRectScale * i);
				Coord[1].x = Coord[0].x + rectData.Width();
				Coord[1].y = Coord[0].y;

				pRT->DrawLines(Coord, 2);

				SIZE size;
				SStringT text = SStringT().Format(L"%.1f", yMin + fYValScale * i);
				pRT->MeasureText(text, text.GetLength(), &size);
				pRT->TextOut(Coord[0].x - size.cx - 15, Coord[0].y - size.cy / 2, text, -1);

			}
			{
				POINT Coord[2] = { 0 };
				Coord[0].x = (LONG)(rectData.left + fXRectScale * i);
				Coord[0].y = rectData.bottom;
				Coord[1].x = Coord[0].x;
				Coord[1].y = Coord[0].y - rectData.Height();

				pRT->DrawLines(Coord, 2);

				SIZE size;
				SStringT text = SStringT().Format(L"%.1f", xMin + fXValScale * i);
				pRT->MeasureText(text, text.GetLength(), &size);

				pRT->TextOut(Coord[0].x - size.cx / 2, Coord[0].y + 7, text, -1);

			}
		}

		//每条线的绘制
		auto pos = recordData_.begin();
		while (pos != recordData_.end())
		{
			if (pos->second.bShow)
			{
				Pen->Release();
				pRT->CreatePen(PS_SOLID, pos->second.Color, pos->second.nLineWidth, &Pen);
				pRT->SelectObject(Pen);
				size_t count = pos->second.points.size();
				if (pos->second.Line == 0)
				{
					pRT->DrawLines(&pos->second.points[0], count);
				}
				else
				{
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
				}
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
void SChartLine::GetXMaxMin(float& Max, float& Min)
{
	Max = Min = recordData_.begin()->second.xValues[0];
	for (auto& it : recordData_)
	{
		if (Min > it.second.xValues[0])
			Min = it.second.xValues[0];
		if (Max < it.second.xValues[it.second.xValues.size() - 1])
			Max = it.second.xValues[it.second.xValues.size() - 1];
	}
}

void SChartLine::GetYMaxMin(float& Max, float& Min)
{
	Max = Min = recordData_.begin()->second.yValues[0];
	for (auto& it : recordData_)
	{
		if (Min > it.second.yValues[0])
			Min = it.second.yValues[0];
		if (Max < it.second.yValues[it.second.yValues.size() - 1])
			Max = it.second.yValues[it.second.yValues.size() - 1];
	}
}


void SChartLine::SetLineWidth(SStringT Name, int Width)
{
	recordData_[Name].nLineWidth = Width;
}
void SChartLine::SetLineShow(SStringT name, bool bShow)
{
	recordData_[name].bShow = bShow;
	InvalidateRect(rcClient);
}
void SChartLine::UpdateData()
{
	if (recordData_.begin() == recordData_.end() || recordData_.begin()->second.yValues.size() == 0) return;
	rcClient = GetClientRect();

	GetXMaxMin(xMax, xMin);

	GetYMaxMin(yMax, yMin);

	fYValScale = (yMax - yMin) / (float)(nScaleNumber);
	fYRectScale = rectData.Height() / (float)(nScaleNumber);


	fXValScale = (xMax - xMin) / (float)(nScaleNumber);
	fXRectScale = rectData.Width() / (float)(nScaleNumber);

	for (auto& data : recordData_)
	{
		for (int i = 0; i < data.second.yValues.size(); i++)
		{
			POINT pt;
			float xVal = (data.second.xValues[i] - xMin) * (float)rectData.Width() / (xMax - xMin);
			pt.x = (LONG)(rectData.left + xVal);
			pt.y = (LONG)(rectData.bottom - ((data.second.yValues[i] - yMin) * (float)rectData.Height() / (yMax - yMin)));
			if (i > 0)
			{
				if (pt.x == data.second.points.back().x && pt.y == data.second.points.back().y)
				{
					continue;
				}
			}
			data.second.points.push_back(pt);
		}
	}
	InvalidateRect(rcClient);
}

