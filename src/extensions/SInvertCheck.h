#pragma once

namespace SOUI
{

	class SOUI_EXP SInvertCheck : public SWindow
	{
		SOUI_CLASS_NAME(SInvertCheck, L"invertcheck")

	public:

		SInvertCheck();
		UINT GetCurState();
		void SetCurCheck(UINT check);
		void OnPaint(IRenderTarget* pRT);
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			SOUI_MSG_MAP_END()
			UINT nInvertState = 0;
	};


}
