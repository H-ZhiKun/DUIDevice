#include "stdafx.h"
#include "SWinEdit.h"
#include "AppFramework.h"
namespace SOUI
{
	SWinEdit::SWinEdit()
	{

	}
	SWinEdit::~SWinEdit()
	{

	}


	void SWinEdit::OnLButtonUp(UINT nFlags, CPoint point)
	{
		__super::OnLButtonUp(nFlags, nFlags);
		realology::App().ShowWinKeyboard(true);
	}
}