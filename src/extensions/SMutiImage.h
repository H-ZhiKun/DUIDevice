#pragma once

namespace SOUI
{

	class SOUI_EXP SMutiImage : public SWindow
	{
		SOUI_CLASS_NAME(SMutiImage, L"mutiImg")

	public:

		SMutiImage();
		UINT GetCurState();
		void SetCurCheck(UINT check);
		void OnPaint(IRenderTarget* pRT);
		SOUI_ATTRS_BEGIN()
			ATTR_INT(L"indexImg", nIndexState, FALSE)
			SOUI_ATTRS_END()
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			SOUI_MSG_MAP_END()
			UINT nIndexState = 0;
	};


}
