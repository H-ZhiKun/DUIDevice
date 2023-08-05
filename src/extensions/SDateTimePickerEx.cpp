#include "stdafx.h"
#include "SDateTimePickerEx.h"
#include "souistd.h"
#include "control/SDateTimePicker.h"
#include "helper/STime.h"
#include "AppFramework.h"
using namespace realology;

namespace SOUI
{
	SDateTimePickerEx::SDateTimePickerEx()
		: m_pDropDownWnd(NULL)
		, m_pSkinBtn(GETBUILTINSKIN(SKIN_SYS_DROPBTN))
		, m_eSelDateType(eDT_NULL)
		, m_nNumWidth(0)
		, m_nCharWidth(0)
		, m_pCalendar(NULL)
		, m_nDropWidth(220)
		, m_bTimeEnable(true)
		, m_wCharNum(0)
	{
		m_evtSet.addEvent(EVENTID(EventDateTimeChanged));
		m_pNcSkin = GETBUILTINSKIN(SKIN_SYS_BORDER);
		m_style.SetAttribute(L"margin", L"1", TRUE);
		m_style.SetAttribute(L"align", L"center", TRUE);
		m_style.SetAttribute(L"valign", L"middle", TRUE);

		m_bFocusable = TRUE;

		GetLocalTime(&m_sysTime);

		m_crSelBg = RGBA(0, 120, 215, 255);
		m_crSelText = RGBA(255, 255, 255, 255);
	}

	SDateTimePickerEx::~SDateTimePickerEx()
	{
		if (NULL != m_pCalendar)
		{
			m_pCalendar->SSendMessage(WM_DESTROY);
			delete m_pCalendar;
		}
	}

	BOOL SDateTimePickerEx::CreateChildren(pugi::xml_node xmlNode)
	{
		m_pCalendar = (SCalendarEx*)SApplication::getSingleton().CreateWindowByName(SCalendarEx::GetClassName());
		if (NULL == m_pCalendar)
			return FALSE;
		m_pCalendar->SetContainer(GetContainer());

		pugi::xml_node xmlCal = xmlNode.child(L"calstyle");
		if (xmlCal)
			m_pCalendar->InitFromXml(xmlCal);

		m_pCalendar->SetAttribute(L"pos", L"0,0,-0,-0", TRUE);
		m_pCalendar->SetOwner(this);    //chain notify message to combobox
		//m_pCalendar->SetID(IDC_DROPDOWN_LIST);

		m_pCalendar->GetEventSet()->subscribeEvent(&SDateTimePickerEx::OnDateChanged, this);
		m_pCalendar->GetEventSet()->subscribeEvent(&SDateTimePickerEx::OnDateCmd, this);
		return TRUE;
	}

	bool SDateTimePickerEx::OnDateChanged(EventCalendarExChanged* pEvt)
	{
		if (NULL == pEvt) return true;

		m_sysTime.wDay = pEvt->iNewDay;
		m_sysTime.wMonth = pEvt->iNewMonth;
		m_sysTime.wYear = pEvt->iNewYear;

		Invalidate();

		EventDateTimeChanged e(this);
		e.newTime = m_sysTime;
		FireEvent(e);

		return true;
	}

	bool SDateTimePickerEx::OnDateCmd(EventCmd* pEvt)
	{
		CloseUp();
		return true;
	}

	SWindow* SDateTimePickerEx::GetDropDownOwner()
	{
		return this;
	}

	void SDateTimePickerEx::OnCreateDropDown(SDropDownWnd* pDropDown)
	{
		m_pCalendar->SetDate(m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
		//pDropDown->SetAttribute(L"sendWheel2Hover", L"1", TRUE);
		pDropDown->InsertChild(m_pCalendar);
		pDropDown->UpdateChildrenPosition();

		m_pCalendar->SetVisible(TRUE);
		m_pCalendar->SetFocus();

		m_dwBtnState = WndState_PushDown;
		CRect rcBtn;
		GetDropBtnRect(&rcBtn);
		InvalidateRect(rcBtn);
	}

	void SDateTimePickerEx::OnDestroyDropDown(SDropDownWnd* pDropDown)
	{
		pDropDown->RemoveChild(m_pCalendar);

		m_pCalendar->SetVisible(FALSE);
		m_pCalendar->SetContainer(GetContainer());

		m_dwBtnState = WndState_Normal;
		m_pDropDownWnd = NULL;
	}

	void SDateTimePickerEx::GetDropBtnRect(LPRECT pBtnRc, LPRECT pSkinRc)
	{
		SIZE szBtn = m_pSkinBtn->GetSkinSize();
		GetClientRect(pBtnRc);
		pBtnRc->left = pBtnRc->right - (pBtnRc->bottom - pBtnRc->top);			// 取 右边的正方形 为  按钮 区域

		if (NULL == pSkinRc)
			return;

		int n = (pBtnRc->bottom - pBtnRc->top - szBtn.cy) / 2;			// 按钮皮肤 则 要居中

		pSkinRc->right = pBtnRc->right - n;
		pSkinRc->left = pSkinRc->right - szBtn.cx;

		pSkinRc->top = pBtnRc->top + n;
		pSkinRc->bottom = pSkinRc->top + szBtn.cy;
	}

	SStringT SDateTimePickerEx::ToFormatText(EnDateType eType, WORD wNum)
	{
		SStringT szNum;
		if (eType == m_eSelDateType)						// 选中项
		{
			if (eDT_Year == eType)
			{
				if (m_wCharNum < 10)
					szNum.Format(_T("%d"), wNum);
				else if (m_wCharNum < 100)
					szNum.Format(_T("%02d"), wNum);
				else if (m_wCharNum < 1000)
					szNum.Format(_T("%03d"), wNum);
				else
					szNum.Format(_T("%04d"), wNum);
			}
			else
			{
				if (m_wCharNum > 0)
					szNum.Format(_T("%d"), wNum);
				else
					szNum.Format(_T("%02d"), wNum);
			}
		}
		else
		{
			if (eDT_Year == eType)
			{
				szNum.Format(_T("%04d"), wNum);
			}
			else
			{
				szNum.Format(_T("%02d"), wNum);
			}
		}

		return szNum;
	}

	void SDateTimePickerEx::Draw(EnDateType eType, IRenderTarget* pRT, WORD wNum, CRect& rcText)
	{
		SStringT szNum = ToFormatText(eType, wNum);

		if (eType != m_eSelDateType)
		{
			pRT->DrawText(szNum, szNum.GetLength(), rcText, GetTextAlign());
			return;
		}

		// 选中 的 填充 字体颜色 变 白
		pRT->FillSolidRect(rcText, m_crSelBg);

		COLORREF old = pRT->SetTextColor(m_crSelText);
		pRT->DrawText(szNum, szNum.GetLength(), rcText, GetTextAlign());
		pRT->SetTextColor(old);
	}

	void SDateTimePickerEx::OnPaint(IRenderTarget* pRT)
	{
		//__super::OnPaint(pRT);
		SPainter painter;

		BeforePaint(pRT, painter);

		CRect rcText;
		GetTextRect(rcText);
		if (0 == m_nNumWidth)
		{
			SIZE sz;
			pRT->MeasureText(_T("5"), 1, &sz);
			m_nNumWidth = sz.cx;
			m_nNumHeight = sz.cy;

			pRT->MeasureText(_T("-"), 1, &sz);
			m_nCharWidth = sz.cx;

		}
		rcText.OffsetRect(0, (rcText.Height() - m_nNumHeight) / 2 + 1);
		rcText.bottom = rcText.top + m_nNumHeight;


		//SStringT strText;

		// year
		rcText.right = rcText.left + m_nNumWidth * 4;
		//strText.Format(_T("%d"), m_sysTime.wYear);
		Draw(eDT_Year, pRT, m_sysTime.wYear, rcText);


		rcText.left = rcText.right;
		rcText.right = rcText.left + m_nCharWidth;
		pRT->DrawText(_T("-"), -1, rcText, GetTextAlign());

		//month
		rcText.left = rcText.right;
		rcText.right = rcText.left + m_nNumWidth * 2;
		//strText.Format(_T("%02d"), m_sysTime.wMonth);
		Draw(eDT_Month, pRT, m_sysTime.wMonth, rcText);

		rcText.left = rcText.right;
		rcText.right = rcText.left + m_nCharWidth;
		pRT->DrawText(_T("-"), -1, rcText, GetTextAlign());

		//day
		rcText.left = rcText.right;
		rcText.right = rcText.left + m_nNumWidth * 2;
		//strText.Format(_T("%02d"), m_sysTime.wDay);
		Draw(eDT_Day, pRT, m_sysTime.wDay, rcText);

		//rcText.left = rcText.right;
		//rcText.right = rcText.left + m_nCharWidth;
		//pRT->DrawText(L" ", -1, rcText, GetTextAlign());
		if (m_bTimeEnable)
		{
			// hour
			rcText.left = rcText.right + m_nCharWidth;
			rcText.right = rcText.left + m_nNumWidth * 2;
			//strText.Format(_T("%02d"), m_sysTime.wHour);
			Draw(eDT_Hour, pRT, m_sysTime.wHour, rcText);

			rcText.left = rcText.right;
			rcText.right = rcText.left + m_nCharWidth;
			pRT->DrawText(_T(":"), -1, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			// minute
			rcText.left = rcText.right;
			rcText.right = rcText.left + m_nNumWidth * 2;
			//strText.Format(_T("%02d"), m_sysTime.wMinute);
			Draw(eDT_Minute, pRT, m_sysTime.wMinute, rcText);

			rcText.left = rcText.right;
			rcText.right = rcText.left + m_nCharWidth;
			pRT->DrawText(_T(":"), -1, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			// second
			rcText.left = rcText.right;
			rcText.right = rcText.left + m_nNumWidth * 2;
			//strText.Format(_T("%02d"), m_sysTime.wSecond);
			Draw(eDT_Second, pRT, m_sysTime.wSecond, rcText);
		}



		AfterPaint(pRT, painter);
	}

	bool SDateTimePickerEx::CalcPopupRect(int nWidth, CRect& rcPopup)
	{
		CRect rcWnd = GetWindowRect();
		GetContainer()->FrameToHost(rcWnd);

		ClientToScreen(GetContainer()->GetHostHwnd(), (LPPOINT)&rcWnd);
		ClientToScreen(GetContainer()->GetHostHwnd(), ((LPPOINT)&rcWnd) + 1);

		HMONITOR hMonitor = ::MonitorFromWindow(GetContainer()->GetHostHwnd(), MONITOR_DEFAULTTONULL);
		CRect rcMonitor;
		if (hMonitor)
		{
			MONITORINFO mi = { sizeof(MONITORINFO) };
			::GetMonitorInfo(hMonitor, &mi);
			rcMonitor = mi.rcMonitor;
		}
		else
		{
			rcMonitor.right = GetSystemMetrics(SM_CXSCREEN);
			rcMonitor.bottom = GetSystemMetrics(SM_CYSCREEN);
		}

		int nHeight = nWidth * 4 / 5;

		if (rcWnd.bottom + nHeight <= rcMonitor.bottom)
		{
			rcPopup.SetRect(rcWnd.right - nWidth, rcWnd.bottom, rcWnd.right, rcWnd.bottom + nHeight);
			return true;
		}


		rcPopup.SetRect(rcWnd.right - nWidth, rcWnd.top - nHeight, rcWnd.right, rcWnd.top);
		return false;
	}

	SDateTimePickerEx::EnDateType SDateTimePickerEx::HitTest(CPoint pt)
	{
		CRect rcText;
		GetTextRect(rcText);
		int nWidth = pt.x - rcText.left;

		EnDateType eSelType = eDT_NULL;

		if (nWidth < (4 * m_nNumWidth + m_nCharWidth))
		{
			eSelType = eDT_Year;			// year
		}
		else if (nWidth < (6 * m_nNumWidth + 2 * m_nCharWidth))
		{
			eSelType = eDT_Month;			// month
		}
		else if (!m_bTimeEnable || nWidth < (8 * m_nNumWidth + 3 * m_nCharWidth))
		{
			eSelType = eDT_Day;			// day
		}
		else if (nWidth < (10 * m_nNumWidth + 4 * m_nCharWidth))
		{
			eSelType = eDT_Hour;			// hour
		}
		else if (nWidth < (12 * m_nNumWidth + 5 * m_nCharWidth))
		{
			eSelType = eDT_Minute;			// minute
		}
		else
		{
			eSelType = eDT_Second;			// second
		}

		return eSelType;
	}

	void SDateTimePickerEx::SetTime(const SYSTEMTIME& sysTime)
	{
		m_sysTime = sysTime;
		Invalidate();
	}

	void SDateTimePickerEx::SetTime(WORD wYear, WORD wMonth, WORD wDay, WORD wHour, WORD wMinute, WORD wSecond)
	{
		m_sysTime.wYear = wYear;
		m_sysTime.wMonth = wMonth;
		m_sysTime.wDay = wDay;
		m_sysTime.wHour = wHour;
		m_sysTime.wMinute = wMinute;
		m_sysTime.wSecond = wSecond;
		Invalidate();
	}

	void SDateTimePickerEx::GetTime(SYSTEMTIME& sysTime)
	{
		sysTime = m_sysTime;
	}

	SStringT SDateTimePickerEx::GetWindowText(BOOL bRawText)
	{
		SStringT szText;
		szText.Format(_T("%04d/%02d/%02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
		if (m_bTimeEnable)
			szText.AppendFormat(_T(" %02d:%02d:%02d"), m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);

		return szText;
	}
	void SDateTimePickerEx::SetWindowTextW(SStringT strTime)
	{
		if (strTime.IsEmpty())
		{
			GetLocalTime(&m_sysTime);
		}
		else
		{
			swscanf(strTime.GetBuffer(0), L"%hd/%hd/%hd %hd:%hd:%hd",
				&m_sysTime.wYear, &m_sysTime.wMonth, &m_sysTime.wDay,
				&m_sysTime.wHour, &m_sysTime.wMinute, &m_sysTime.wSecond);
		}
		Invalidate();
	}

	void SDateTimePickerEx::SetUpdater(std::function<void(void)> func)
	{
		pUpdate = func;
	}
	void SDateTimePickerEx::OnLButtonDown(UINT nFlags, CPoint pt)
	{
		__super::OnLButtonDown(nFlags, pt);
		if (WndState_PushDown == m_dwBtnState) return;
		App().ShowWinKeyboard(true);
		EnDateType  eSelType = HitTest(pt);
		if (m_eSelDateType != eSelType)
		{
			m_eSelDateType = eSelType;
			Invalidate();
		}

		return;
	}

	void SDateTimePickerEx::OnMouseMove(UINT nFlags, CPoint pt)
	{
		if (WndState_PushDown == m_dwBtnState)
			return;

		__super::OnMouseHover(nFlags, pt);

		CRect rcBtn;
		GetDropBtnRect(&rcBtn);

		if (rcBtn.PtInRect(pt))
		{
			m_dwBtnState = WndState_Hover;
			InvalidateRect(rcBtn);
		}
		else if (WndState_Hover == m_dwBtnState)
		{
			m_dwBtnState = WndState_Normal;
			InvalidateRect(rcBtn);
		}
	}

	void SDateTimePickerEx::OnMouseLeave()
	{
		if (WndState_PushDown == m_dwBtnState)
			return;

		if (GetState() & WndState_Hover)
			__super::OnMouseLeave();

		if (WndState_Hover == m_dwBtnState)
		{
			m_dwBtnState = WndState_Normal;

			CRect rcBtn;
			GetDropBtnRect(&rcBtn);
			InvalidateRect(rcBtn);
		}
	}

	BOOL SDateTimePickerEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		TimeWheel(zDelta > 0);
		return TRUE;
	}

	void SDateTimePickerEx::TimeWheel(bool bUp)
	{
		switch (m_eSelDateType)
		{
		case eDT_Year:
			bUp ? ++m_sysTime.wYear : --m_sysTime.wYear;
			break;
		case eDT_Month:
		{
			CircluNum(bUp, m_sysTime.wMonth, 1, 12);
			break;
		}
		case eDT_Day:
		{
			int nDays = SCalendarCore::GetDaysOfMonth(m_sysTime.wYear, m_sysTime.wMonth);
			CircluNum(bUp, m_sysTime.wDay, 1, nDays);
			break;
		}
		case eDT_Hour:
		{
			CircluNum(bUp, m_sysTime.wHour, 0, 23);
			break;
		}
		case eDT_Minute:
		{
			CircluNum(bUp, m_sysTime.wMinute, 0, 59);
			break;
		}
		case eDT_Second:
		{
			CircluNum(bUp, m_sysTime.wSecond, 0, 59);
			break;
		}
		default:
			break;
		}

		if (!SCalendarCore::DateCheck(m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay))
		{
			WORD nDays = SCalendarCore::GetDaysOfMonth(m_sysTime.wYear, m_sysTime.wMonth);
			m_sysTime.wDay = nDays;
		}
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		if (m_sysTime.wYear > curTime.wYear)
		{
			m_sysTime.wYear = curTime.wYear;
		}
		if (m_sysTime.wYear == curTime.wYear)
		{
			if (m_sysTime.wMonth > curTime.wMonth)
			{
				m_sysTime.wMonth = curTime.wMonth;
			}
		}
		if (m_sysTime.wYear == curTime.wYear && m_sysTime.wMonth == curTime.wMonth)
		{
			if (m_sysTime.wDay > curTime.wDay)
			{
				m_sysTime.wDay = curTime.wDay;
			}
		}
		m_wCharNum = 0;
		Invalidate();
	}

	void SDateTimePickerEx::CircluNum(bool bUp, WORD& wNum, WORD wMin/*=1*/, WORD wMax/*=0*/)
	{
		if (bUp)
		{
			if (wNum >= wMax)
				wNum = wMin;
			else
				++wNum;
		}
		else
		{
			if (wNum <= wMin)
				wNum = wMax;
			else
				--wNum;
		}
	}

	void SDateTimePickerEx::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
	{
		if (37 == nChar)				// <-
		{
			if (eDT_Year == m_eSelDateType)
				return;

			m_eSelDateType = static_cast<EnDateType>((int)m_eSelDateType - 1);
			m_wCharNum = 0;
			Invalidate();
		}
		else if (39 == nChar)
		{
			if (eDT_Second == m_eSelDateType)
				return;

			m_eSelDateType = static_cast<EnDateType>((int)m_eSelDateType + 1);
			m_wCharNum = 0;
			Invalidate();
		}
		else if (38 == nChar)
			TimeWheel(true);
		else if (40 == nChar)
			TimeWheel(false);
		else if (13 == nChar)
		{
			if(pUpdate != nullptr) pUpdate();
		}
	}

	void SDateTimePickerEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar > '9' || nChar < '0')
		{
			if (nChar != 8)
			{
				return;
			}
		}

		if (eDT_NULL == m_eSelDateType)
			return;
		// 这个 不写了   谁帮我写了吧
		//时间更新通知函数
		

		int nNum = nChar - 48;
		if (m_wCharNum > 0)
		{
			m_wCharNum *= 10;
			m_wCharNum += nNum;
		}
		else
		{
			m_wCharNum = nNum;
		}

		switch (m_eSelDateType)
		{
		case eDT_Year:
			SYSTEMTIME curTime;
			GetLocalTime(&curTime);
			if (nChar == 8)
			{
				m_sysTime.wYear = 0;
				//Invalidate();
				return;
			}
			if (m_wCharNum <= 0)
			{
				return;
			}
			else if (m_wCharNum > curTime.wYear)
				m_wCharNum = nNum;
			m_sysTime.wYear = m_wCharNum;
			break;
		case eDT_Month:
			if (nChar == 8)
			{
				m_sysTime.wMonth = 0;
				//Invalidate();
				return;
			}
			if (m_wCharNum <= 0)
			{
				return;
			}
			else if (m_wCharNum > 12)
				m_wCharNum = nNum;
			if (!SCalendarCore::DateCheck(m_sysTime.wYear, m_wCharNum, m_sysTime.wDay))
				m_sysTime.wDay = SCalendarCore::GetDaysOfMonth(m_sysTime.wYear, m_wCharNum);
			m_sysTime.wMonth = m_wCharNum;
			break;
		case eDT_Day:
			if (nChar == 8)
			{
				m_sysTime.wDay = 0;
				//Invalidate();
				return;
			}
			if (m_wCharNum <= 0)
			{
				return;
			}
			if (!SCalendarCore::DateCheck(m_sysTime.wYear, m_sysTime.wMonth, m_wCharNum))
				m_wCharNum = nNum;
			m_sysTime.wDay = m_wCharNum;
			break;
		case eDT_Hour:
			if (nChar == 8)
			{
				m_sysTime.wHour = 0;
				//Invalidate();
				return;
			}
			if (m_wCharNum > 23 || m_wCharNum < 0)
				m_wCharNum = nNum;
			m_sysTime.wHour = m_wCharNum;
			break;
		case eDT_Minute:
			if (nChar == 8)
			{
				m_sysTime.wMinute = 0;
				//Invalidate();
				return;
			}
			if (m_wCharNum > 59 || m_wCharNum < 0)
				m_wCharNum = nNum;
			m_sysTime.wMinute = m_wCharNum;
			break;
		case eDT_Second:
			if (nChar == 8)
			{
				m_sysTime.wSecond = 0;
				//Invalidate();
				return;
			}
			if (m_wCharNum > 59 || m_wCharNum < 0)
				m_wCharNum = nNum;
			m_sysTime.wSecond = m_wCharNum;
			break;
		default:
			break;
		}
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		if (m_sysTime.wYear > curTime.wYear)
		{
			m_sysTime.wYear = curTime.wYear;
		}
		if (m_sysTime.wYear == curTime.wYear)
		{
			if (m_sysTime.wMonth > curTime.wMonth)
			{
				m_sysTime.wMonth = curTime.wMonth;
			}
		}
		if (m_sysTime.wYear == curTime.wYear && m_sysTime.wMonth == curTime.wMonth)
		{
			if (m_sysTime.wDay > curTime.wDay)
			{
				m_sysTime.wDay = curTime.wDay;
			}
		}
		if (eDT_NULL != m_eSelDateType)
			Invalidate();

	}

	void SDateTimePickerEx::OnDestroy()
	{
		CloseUp();
		__super::OnDestroy();
	}

	void SDateTimePickerEx::OnSetFocus(SWND wndOld)
	{
		__super::OnSetFocus(wndOld);
	}

	void SDateTimePickerEx::OnKillFocus(SWND wndFocus)
	{
		__super::OnKillFocus(wndFocus);
		CloseUp();
		m_wCharNum = 0;
		if (eDT_NULL != m_eSelDateType)
		{
			m_eSelDateType = eDT_NULL;
			Invalidate();
		}
	}

	void SDateTimePickerEx::DrawFocus(IRenderTarget* pRT)
	{

	}



	void SDateTimePickerEx::CloseUp()
	{
		if (NULL != m_pDropDownWnd)
		{
			m_pDropDownWnd->EndDropDown(IDCANCEL);
		}
	}

} // SOUI
