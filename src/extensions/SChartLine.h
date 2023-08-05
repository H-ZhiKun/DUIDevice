#pragma once
#include <vector>
#include <map>
namespace SOUI
{
	class SChartLine :public SWindow
	{
		SOUI_CLASS_NAME(SChartLine, L"chartLine")
	public:
		struct LineTag
		{
			bool bShow = true;
			std::vector<float> xValues;
			std::vector<float> yValues;
			std::vector<POINT> points;
			int nLineWidth = 2;
			COLORREF Color = 0;
			int Line = 0;
		};

		SChartLine();
		~SChartLine();

		//创建一条线
		void CreatedLine(SStringT Name, int LineType = 0);
		//向线添加数据
		void AddData(SStringT Name, float xValue, float yValue);

		//清空所有数据
		void Clear();

		//设置线宽
		void SetLineWidth(SStringT Name, int Width);
		void SetLineShow(SStringT name, bool bShow);

		void UpdateData();
	protected:
		void GetXMaxMin(float& Max, float& Min);
		void GetYMaxMin(float& Max, float& Min);

		void OnPaint(IRenderTarget* pRT);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseLeave();
	protected:
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			SOUI_MSG_MAP_END()

			SOUI_ATTRS_BEGIN()
			ATTR_RECT(L"CoordMargin", rectCoordMargin, TRUE)
			ATTR_COLOR(L"LineColor", nLineColor, TRUE)
			ATTR_INT(L"DataPointSize", nDataPointSize, TRUE)
			ATTR_COLOR(L"CoordColor", nCoordColor, TRUE)
			ATTR_COLOR(L"DataBgColor", nDataBgColor, TRUE)
			ATTR_COLOR(L"TipBgColor", nTipBgColor, TRUE)
			ATTR_I18NSTRT(L"xTitle", xTitle_, TRUE)
			ATTR_I18NSTRT(L"yTitle", yTitle_, TRUE)
			SOUI_ATTRS_END()
	protected:
		COLORREF nLineColor = 0;
		COLORREF nCoordColor = 0;
		COLORREF nDataBgColor = 0;
		COLORREF nTipBgColor = 0;
		int nCoordWidth = 1;

		int nDataPointSize = 6;//数据点大小

		using CoordLineData = std::map<SStringT, LineTag>;
		CoordLineData recordData_;
		CPoint ptCursorPoint;
		CRect rcClient;
		CRect rectCoordMargin;
		CRect rectData;
		POINT ptCoord[4];
		int nScaleNumber = 6;
		float fYValScale = 0;
		float fXValScale = 0;
		float fYRectScale = 0;
		float fXRectScale = 0;
	
		float xMax = 0, xMin = 0, yMax = 0, yMin = 0;
		STrText xTitle_;
		STrText yTitle_;

		std::vector<SStringT>  textTips; //提示框数据
		std::map<SStringT, COLORREF> mapColors{ {L"Real", RGBA(140, 58, 75, 255) },
												{L"NT",RGBA(255, 219, 77, 255)},
												{L"BH", RGBA(237, 109, 132, 255)},
												{L"PL", RGBA(129, 192, 121, 255)},
												{L"HB", RGBA(106, 106, 212, 255)}};

	};

}