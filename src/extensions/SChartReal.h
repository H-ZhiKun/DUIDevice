#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>
#include "SystemDef.h"

namespace SOUI
{
	enum LINE_TYPE {
		TYPE_CURVE,
		TYPE_LINE,
		TYPE_POINT
	};
	
	
	class SChartReal : public SWindow
	{
		SOUI_CLASS_NAME(SChartReal, L"chartReal")
	public:


		struct LineTag
		{
			std::list<float> values;
			std::list<std::string> times;
			std::vector<POINT> points;
			COLORREF Color;
		};

		SChartReal();
		~SChartReal();
		void CreateLine(SStringT Name, COLORREF Color);
		void SetLineShow(SStringT title, bool bShow);
		//创建一条线
		void SetHistory(bool bHistory);
		bool isHistory();
		void AddData(const AdvanceData& data);
		void InitRect();
		void UpdateData();

		time_t Time2Stamp(std::string dateTime);
		std::string Stamp2Time(time_t t);
		void GetMaxMin();
	protected:
		void Clear();
		void OnPaint(IRenderTarget *pRT);
	protected:
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			SOUI_MSG_MAP_END()

			SOUI_ATTRS_BEGIN()
			ATTR_RECT(L"CoordMargin", rectCoordMargin, TRUE)
			ATTR_COLOR(L"LineColor", nLineColor, TRUE)
			ATTR_COLOR(L"CoordColor", nCoordColor, TRUE)
			ATTR_COLOR(L"DataBgColor", nDataBgColor, TRUE)
			SOUI_ATTRS_END()
	protected:

		CRect rectCoordMargin;
		CRect rectClient;
		CRect rectData;
		POINT ptCursorPoint;
		COLORREF nLineColor;
		COLORREF nCoordColor;
		COLORREF nDataBgColor;
		COLORREF nTipBgColor;
		int nCoordWidth = 1;
		int nLineWidth = 1;
		POINT ptCoord[4];
		std::vector<SStringT>  textTips; //提示框数据
		time_t xMaxVal_, xMinVal_;
		int nScaleNumber = 6;
		float fYRectScale = 0;
		float fXRectScale = 0;
		float fXValScale = 0;
		float yMaxVal_ = 0, yMinVal_ = 0;

		bool bHistory_ = false;

		std::map<SStringT, LineTag> recordData_;

		std::map<SStringT, COLORREF> vLineColor{ {L"checked_press", RGBA(255, 219, 77, 255)} ,
												{L"checked_vflow",RGBA(129, 192, 121, 255) },
												{L"checked_diffpress",RGBA(237, 109, 132, 255) },
												{L"checked_opening",RGBA(160, 125, 183, 255) },
												{L"checked_mlevel",RGBA(106, 106, 212, 255) } };

		std::list<SStringT> vLineShow{ L"checked_press" ,L"checked_vflow" ,L"checked_diffpress" ,L"checked_opening" ,L"checked_mlevel" };

	};

}