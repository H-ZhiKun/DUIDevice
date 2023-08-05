#include "stdafx.h"
#include "SRotateWindow.h"
//#include <time.h>

namespace SOUI
{
	SRotateWindow::SRotateWindow(void) {
	}
	SRotateWindow::~SRotateWindow(void) {}

	void SRotateWindow::OnPaint(IRenderTarget* pRT)
	{
		CRect rect = GetClientRect();
		if (IsVisible() && pRT)
		{
			SPainter painter;
			BeforePaint(pRT, painter);
			IFont* iFont = (IFont *)pRT->GetCurrentObject(OT_FONT);
			const LOGFONT * logfont = iFont->LogFont();
			HDC hdc = pRT->GetDC();
			Gdiplus::Graphics gdi(hdc);

			//绘制文本
			CRect rcClient = GetClientRect();
			Gdiplus::Font font(hdc, logfont);
			Gdiplus::SolidBrush brush(pRT->GetTextColor());
			Gdiplus::Matrix matrix;
			Gdiplus::RectF rectF;
			gdi.MeasureString(m_strText.GetText(), -1, &font, Gdiplus::PointF(static_cast<float>(rcClient.left), static_cast<float>(rcClient.top)), &rectF);
	
			matrix.RotateAt(m_fRotate, Gdiplus::PointF(rcClient.left+ rectF.Width / 2, rcClient.top+rectF.Height / 2));
			gdi.SetTransform(&matrix);

			gdi.DrawString(m_strText.GetText(), -1, &font, Gdiplus::PointF(rcClient.left - rcClient.Height()/2 + rectF.Height/2, rcClient.top - rectF.Width / 2 + rcClient.Width()/2), &brush);
			//取消旋转
			gdi.ResetTransform();

			AfterPaint(pRT, painter);
		}
	}
}