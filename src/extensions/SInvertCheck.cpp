#include "stdafx.h"
#include "SInvertCheck.h"

namespace SOUI
{
	SInvertCheck::SInvertCheck()
	{
		m_style.SetAttribute(L"align", L"center");
		m_bFocusable = TRUE;
	}

	UINT SInvertCheck::GetCurState()
	{
		return nInvertState;
	}

	void SInvertCheck::SetCurCheck(UINT check)
	{
		nInvertState = check;
	}

	void SInvertCheck::OnPaint(IRenderTarget* pRT)
	{
		if (m_pBgSkin)
		{
			CRect rc;
			GetClientRect(&rc);
			if (GetState() & WndState_Check)
			{
				m_pBgSkin->Draw(pRT, rc, nInvertState);
			}
		}
	}

}
