#pragma once

namespace SOUI 
{


	class SOUI_EXP SWinEdit : public SEdit
	{
		SOUI_CLASS_NAME(SWinEdit, L"winEdit")
	public:
		SWinEdit();
		~SWinEdit();
	protected:
		void OnLButtonUp(UINT nFlags, CPoint point);
	protected:
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_LBUTTONUP(OnLButtonUp)
		SOUI_MSG_MAP_END()
	};

}