#pragma once
#include <core/swnd.h>
#include <string>
namespace SOUI 
{
	class SOUI_EXP SKeyEdit : public SEdit
	{
		SOUI_CLASS_NAME(SKeyEdit, L"keyEdit")
	public:
		SKeyEdit();
		~SKeyEdit();
		void SetCallBack(std::function<void(std::string, std::string)> func,std::string title,std::string item);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void SetCallBackInfo(std::string title,std::string item);
		void SetDBCTask(std::function<void(void)> func);
		void SetESCCallback(std::function<void(void)> func);
	protected:
		SOUI_ATTRS_BEGIN()
			ATTR_BOOL(L"useTabTip", bUseTab_,FALSE)
		SOUI_ATTRS_END()

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		SOUI_MSG_MAP_END()
	protected:
		void OnLButtonDblClk(UINT nFlags, CPoint point);
		void OnLButtonDown(UINT nFlags, CPoint point);
	private:

		std::function<void(std::string, std::string)> pTask_ = nullptr;
		std::function<void(void)> pLBDbClkTask_ = nullptr;
		std::function<void(void)> pESCTask_ = nullptr;
		std::string sTitle_;
		std::string sItem_;
		SStringT sLastText_;
		bool bUseTab_;
		bool bReadOnly_ = true;
	};

}