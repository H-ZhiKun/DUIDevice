#include "stdafx.h"
#include "SMutiImage.h"

namespace SOUI
{
	SMutiImage::SMutiImage()
	{
		m_style.SetAttribute(L"align", L"center");
		m_bFocusable = TRUE;
	}

	UINT SMutiImage::GetCurState()
	{
		return nIndexState;
	}

	void SMutiImage::SetCurCheck(UINT check)
	{
		if (check == nIndexState) return;
		nIndexState = check;
		Invalidate();
	}

	void SMutiImage::OnPaint(IRenderTarget* pRT)
	{
		SetMsgHandled(TRUE);
		if (m_pBgSkin)
		{
			CRect rc;
			GetClientRect(&rc);
			m_pBgSkin->Draw(pRT, rc, nIndexState);
		}
		__super::OnPaint(pRT);
	}

}
