#include "stdafx.h"
#include "SKeyEdit.h"
#include "AppFramework.h"
using namespace realology;
namespace SOUI
{
	SKeyEdit::SKeyEdit():bUseTab_(false)
	{

	}
	SKeyEdit::~SKeyEdit()
	{

	}
	void SKeyEdit::SetCallBack(std::function<void(std::string, std::string)> func,std::string title,std::string item)
	{
		pTask_ = func;
		sTitle_ = title;
		sItem_ = item;
	}

	void SKeyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		SetMsgHandled(FALSE);
		if (VK_RETURN == nChar || VK_ESCAPE == nChar)
		{
			SetAttribute(L"colorBkgnd", L"#00000000");
			SetAttribute(L"readOnly", L"1");
			bReadOnly_ = true;
			if (VK_RETURN == nChar)
			{
				if (pTask_) pTask_(sTitle_, sItem_);
			}
			else
			{
				SetWindowTextW(sLastText_);
				if (pESCTask_) pESCTask_();
			}
			if (!bUseTab_) App().ShowCustomKeyboard(false, nullptr);
			else           App().ShowWinKeyboard(false);
			KillFocus();
		}
	}

	void SKeyEdit::SetCallBackInfo(std::string title,std::string item)
	{
		sTitle_ = title;
		sItem_ = item;
	}

	void SKeyEdit::SetDBCTask(std::function<void(void)> func)
	{
		pLBDbClkTask_ = func;
	}

	void SKeyEdit::SetESCCallback(std::function<void(void)> func)
	{
		pESCTask_ = func;
	}

	void SKeyEdit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		__super::OnLButtonDown(nFlags, point);
		if (!bReadOnly_ && bUseTab_)
		{
			App().ShowWinKeyboard(true);
		}
	}


	void SKeyEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		SetAttribute(L"colorBkgnd", L"#F8F8F8");
		SetAttribute(L"readOnly", L"0");
		bReadOnly_ = false;
		sLastText_ = GetWindowTextW();
		if (!bUseTab_)
		{
			SetWindowTextW(L"");
			App().ShowCustomKeyboard(true,this);
		}
		else  App().ShowWinKeyboard(true);
		if (pLBDbClkTask_)
		{
			pLBDbClkTask_();
		}
	}


}