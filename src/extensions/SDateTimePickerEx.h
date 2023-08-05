#pragma once

#include <core/swnd.h>
#include <string>
namespace SOUI
{
    // 日期 选择控件
	class SOUI_EXP SDateTimePickerEx : public SWindow, public ISDropDownOwner
{
	SOUI_CLASS_NAME(SDateTimePickerEx, L"dateTimePickerEx")  

public:
	enum EnDateType
	{
		eDT_NULL = 0,
		eDT_Year ,
		eDT_Month ,
		eDT_Day ,
		eDT_Hour ,
		eDT_Minute ,
		eDT_Second ,
	};
	SDateTimePickerEx();
    ~SDateTimePickerEx();

public:
	void CloseUp();
	EnDateType HitTest(CPoint pt);
	void SetTime(const SYSTEMTIME& sysTime);
	void GetTime(SYSTEMTIME& sysTime);
	void SetTime(WORD wYear, WORD wMonth, WORD wDay, WORD wHour, WORD wMinute, WORD wSecond);
	SStringT GetWindowText(BOOL bRawText=FALSE);
	void SetWindowTextW(SStringT strTime);
	void SetUpdater(std::function<void(void)> func);
protected:		// 继承 
	virtual SWindow* GetDropDownOwner();
	virtual void OnCreateDropDown(SDropDownWnd* pDropDown);
	virtual void OnDestroyDropDown(SDropDownWnd* pDropDown);
protected:
	virtual BOOL CreateChildren(pugi::xml_node xmlNode);
	bool OnDateChanged(EventCalendarExChanged* pEvt);
	bool OnDateCmd(EventCmd* pEvt);
	void GetDropBtnRect(LPRECT pBtnRc, LPRECT pSkinRc = NULL);
	SStringT ToFormatText(EnDateType eType, WORD wNum);
	bool CalcPopupRect(int nHeight, CRect& rcPopup);
	void Draw(EnDateType eType, IRenderTarget* pRT, WORD wNum, CRect& rcText);
protected:
	void OnPaint(IRenderTarget* pRT);
	void OnLButtonDown(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint pt);
	void OnMouseLeave();
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags );
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnDestroy();
	void OnSetFocus(SWND wndOld);
	void OnKillFocus(SWND wndFocus);
	void DrawFocus(IRenderTarget *pRT);
	void TimeWheel(bool bUp);
	void CircluNum(bool bUp, WORD& wNum, WORD wMin, WORD wMax);
	
	SOUI_ATTRS_BEGIN()
		ATTR_SKIN(L"btnSkin", m_pSkinBtn, FALSE)
		ATTR_BOOL(L"timeEnable", m_bTimeEnable, FALSE)
		ATTR_INT(L"dropWidth", m_nDropWidth, FALSE)
	SOUI_ATTRS_END()

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)        
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_KEYDOWN(OnKeyDown) 
		MSG_WM_CHAR(OnChar)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SETFOCUS_EX(OnSetFocus)
		MSG_WM_KILLFOCUS_EX(OnKillFocus)
	
	SOUI_MSG_MAP_END()

protected:
	
	DWORD								m_dwBtnState;  /**< 按钮状态      */
	ISkinObj*							m_pSkinBtn;    /**< 按钮资源      */
	SDropDownWnd*						m_pDropDownWnd;  /**< DropDown指针 */
	EnDateType						m_eSelDateType;
	int										m_nNumWidth;
	int										m_nNumHeight;
	int										m_nCharWidth;
	COLORREF							m_crSelBg;
	COLORREF							m_crSelText;
	SYSTEMTIME						m_sysTime;
	SStringT								m_sKey;

	int										m_nDropWidth;
	bool									m_bTimeEnable;				// 是否 有 时 分 秒
	SCalendarEx*						m_pCalendar;
	WORD								m_wCharNum;
	std::function<void(void)> pUpdate = nullptr;
};
}