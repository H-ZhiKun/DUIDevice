#pragma once
#include "SystemDef.h"
#include <map>
#include <vector>
#include <string>
#include <list>
namespace SOUI
{
	class SChartLogs : public SWindow
	{
		SOUI_CLASS_NAME(SChartLogs, L"chartLogs")
	public:

		struct LineTag
		{
			std::vector<float> values;
			std::vector<std::string> times;
			std::vector<POINT> points;
			std::vector<int> ptIndex;
		};
		struct CommentsPt
		{
			std::vector<CommentsData> data;
			std::vector<POINT> points;
		};

		SChartLogs();
		~SChartLogs();
		void CreateLine(SStringT name);
		void AddData(SStringT name, float YValue, std::string& lvTime);
		void AddComment(std::vector<CommentsData>& data);
		void InitRect();
		void UpdateData();
		//清空所有数据
		void Clear(SStringT name);
		void Clear();
		void CleanLabel();
		void SetLabelAndColor(SStringT& label, COLORREF color);
		time_t Time2Stamp(std::string dateTime);
		std::string Stamp2Time(time_t t)
		{
			char charTime[50] = {};
			struct tm* stm;
			t += 28800;
			stm = gmtime(&t);
			strftime(charTime, 50, "%Y/%m/%d %H:%M:%S", stm);
			return charTime;
		}
		void GetMaxMin();
		

		void SetCommentsCallback(std::function<void(bool,std::string, std::string)> callback);
		void SetCommentsEdit(bool bEdit);
	protected:

		void OnPaint(IRenderTarget* pRT);
		void OnMouseMove(UINT nFlags, CPoint point);

		void OnMouseLeave();
		void OnLButtonDown(UINT nFlags, CPoint point);
	protected:
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_MOUSEMOVE(OnMouseMove) 
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			SOUI_MSG_MAP_END()

			SOUI_ATTRS_BEGIN()
			ATTR_RECT(L"CoordMargin", rectCoordMargin, TRUE)
			ATTR_COLOR(L"LineColor", nLineColor, TRUE)
			ATTR_INT(L"DataPointSize", nDataPointSize, TRUE)
			ATTR_COLOR(L"CoordColor", nCoordColor, TRUE)
			ATTR_COLOR(L"DataBgColor", nDataBgColor, TRUE)
			ATTR_COLOR(L"TipBgColor", nTipBgColor, TRUE)
			ATTR_COLOR(L"CommentColor", nCommentColor, TRUE)
			SOUI_ATTRS_END()
	protected:
		COLORREF nLineColor = 0;
		COLORREF nCoordColor = 0;
		COLORREF nDataBgColor = 0;
		COLORREF nTipBgColor = 0;
		COLORREF nCommentColor = 0;
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
		float fLeftValScale = 0;
		float fRightValScale = 0;
		float fYRectScale = 0;
		float RMaxVal = 0, RMinVal = 0, LMaxVal = 0, LMinVal = 0;
		std::string xMaxTime, xMinTime;

		CommentsPt vCommentInfo;
		bool bCommentsEdit = false;
		std::function<void(bool,std::string, std::string)> pCommentCallback = nullptr;
		std::vector<SStringT>  textTips; //提示框数据
		std::list<std::pair<COLORREF, SStringT>> mapLabelColor;
	};

}