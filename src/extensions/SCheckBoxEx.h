#pragma once

namespace SOUI
{

	class SOUI_EXP SCheckBoxEx : public SWindow
	{
		SOUI_CLASS_NAME(SCheckBoxEx, L"checkex")

	public:

		SCheckBoxEx();

	protected:

		void OnLButtonUp(UINT nFlags, CPoint point);

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);


		HRESULT OnAttrCheck(const SStringW& strValue, BOOL bLoading);

		SOUI_ATTRS_BEGIN()
			ATTR_CUSTOM(L"checked", OnAttrCheck)
		SOUI_ATTRS_END()

			SOUI_MSG_MAP_BEGIN()
			MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_KEYDOWN(OnKeyDown)
			SOUI_MSG_MAP_END()
	};


}
