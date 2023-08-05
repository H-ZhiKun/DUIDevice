#pragma once

namespace SOUI
{
	class SRotateWindow : public SWindow
	{
		SOUI_CLASS_NAME(SRotateWindow, L"rotateWnd")

	public:
		SRotateWindow(void);
		~SRotateWindow(void);

	protected:
		void OnPaint(IRenderTarget* pRT);

	private:
		float m_fRotate = 0;

		SOUI_ATTRS_BEGIN()
			ATTR_FLOAT(L"rotate", m_fRotate, TRUE)
		SOUI_ATTRS_END()

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
		SOUI_MSG_MAP_END()
	};
}