#include "stdafx.h"
#include "SCheckBoxEx.h"

namespace SOUI
{
	SCheckBoxEx::SCheckBoxEx()
	{
		m_style.SetAttribute(L"align", L"center");
		m_bFocusable = TRUE;
	}

	void SCheckBoxEx::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if ((GetState()&WndState_PushDown) && GetWindowRect().PtInRect(point))
		{
			if (IsChecked())
				ModifyState(0, WndState_Check, TRUE);
			else
				ModifyState(WndState_Check, 0, TRUE);
		}

		SWindow::OnLButtonUp(nFlags, point);
	}

	void SCheckBoxEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == VK_SPACE)
		{
			if (IsChecked())
				ModifyState(0, WndState_Check, TRUE);
			else
				ModifyState(WndState_Check, 0, TRUE);

			FireCommand();
		}
	}

	HRESULT SCheckBoxEx::OnAttrCheck(const SStringW& strValue, BOOL bLoading)
	{
		SetCheck(STRINGASBOOL(strValue));
		return S_FALSE;
	}
}
