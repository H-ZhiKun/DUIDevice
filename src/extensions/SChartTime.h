#pragma once
#include <vector>
#include <map>
#include <string>
namespace SOUI
{
	class SChartTime :public SWindow
	{
		SOUI_CLASS_NAME(SChartTime, L"chartTime")
	public:
		struct LineTag
		{
			std::vector<float> yValues;
			std::vector<POINT> points;
			COLORREF Color;
		};

		SChartTime();
		~SChartTime();

		//创建一条线
		void CreatedLine(SStringT Name);
		//向线添加数据
		void AddData(SStringT Name, float yValue);
		void AddTime(SStringT time);

		//清空所有数据
		void Clear();

		void UpdateData();
		void SetXMaxMin(SStringT& Max, SStringT& Min);
		void SetYMaxMin(float leftMax, float leftMin, float rightMax, float rightMin);
	protected:
		void GetYMaxMin(float& Max, float& Min);
		time_t Time2Stamp(std::string dateTime);
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
			ATTR_I18NSTRT(L"leftTitle", leftTitle_, TRUE)
			ATTR_I18NSTRT(L"rightTitle", rightTitle_, TRUE)
			SOUI_ATTRS_END()
	protected:
		COLORREF nLineColor;
		COLORREF nCoordColor;
		COLORREF nDataBgColor;
		COLORREF nTipBgColor;
		int nCoordWidth = 1;
		int nLineWidth = 1;

		int nDataPointSize = 6;//数据点大小

		using CoordLineData = std::map<SStringT, LineTag>;
		CoordLineData recordData_;
		CPoint ptCursorPoint;
		CRect rcClient;
		CRect rectCoordMargin;
		CRect rectData;
		POINT ptCoord[4];
		int nScaleNumber = 6;
		float leftValScale = 0;
		float rightValScale = 0;
		float fYRectScale = 0;



		float lMax = 0, lMin = 0, rMax = 0, rMin = 0;
		time_t xMax, xMin;
		SStringT xMaxTime, xMinTime;
		STrText leftTitle_;
		STrText rightTitle_;

		std::vector<SStringT> xValues;

		std::vector<SStringT>  textTips; //提示框数据
		std::map<SStringT, COLORREF> mapColors{ {L"PV", RGBA(106, 106, 212, 255)},
												{L"YP",RGBA(237, 109, 132, 255)} };

	};

}