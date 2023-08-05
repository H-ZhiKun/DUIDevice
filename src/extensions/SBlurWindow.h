#pragma once
#include <D3D11.h>
#include <d2d1_1.h>
#include <dxgi.h>
#include <wincodec.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "windowscodecs.lib" )

//ʹ��Direct2D 1.1 �������win7

struct MResources
{
	BYTE* data;
	DWORD Size;
};

template<class Interface>
inline void SafeRelease(Interface*& pInterfaceToRelease) {
	if (pInterfaceToRelease != nullptr) {
		pInterfaceToRelease->Release();
		pInterfaceToRelease = nullptr;
	}
}

class MDirect2D
{
public:
	MDirect2D();
	~MDirect2D();

	/*
	��˹ģ��
	bitmap - ����ͼ��
	radius - ģ���뾶(ģ����)

	return ���ͼ��
	*/
	Gdiplus::Bitmap* GaussianBlur(Gdiplus::Bitmap* bitmap, int radius);
	IBitmap* IGaussianBlur(Gdiplus::Bitmap* bitmap, int radius);
	IBitmap* IGaussianBlur(HBITMAP bitmap, int radius);

	/*
	��˹ģ��
	dc - �����ָ���豸����
	width - ������
	height - ����߶�
	cx - ���λ�� X
	cy - ���λ�� Y
	bitmap - ����λͼ
	radius - ģ���뾶(ģ����)

	return ���ͼ��
	*/
	void IGaussianBlur(HDC dc, int width, int height, int cx, int cy, int xSrc, int ySrc, HBITMAP bitmap, int radius);

	void Release();

private:
	MResources ID2DGaussianBlur(IWICBitmap*& bitmap, int radius, bool isDC = false, HDC pDC = 0, int width = 0, int height = 0, int cx = 0, int cy = 0, int xSrc = 0, int ySrc = 0);
	MResources ID2DLoadImage(Gdiplus::Bitmap* bitmap, int radius);

	ID3D11Device* D3DDevice = nullptr;//D3D�豸
	D3D_FEATURE_LEVEL D3DfeatureLevels;//D3D���Եȼ�
	ID3D11DeviceContext* D3DDeviceContext = nullptr;//D3D�豸������

	ID2D1Factory1* D2DFactory = nullptr;//D2D����
	ID2D1Device* D2DDevice = nullptr;//D2D�豸
	IDXGIDevice* DXGIDevice = nullptr;//DXIG�豸
	ID2D1DeviceContext* D2DDeviceContext = nullptr;//D2D�豸������
	IWICImagingFactory* WICFactory = nullptr;//WIC����

	ID2D1Effect* D2DEffect = nullptr;//D2D��Ч
	ID2D1Bitmap1* D2DBitmap = nullptr;//D2Dλͼ

	IWICBitmapEncoder* WICEncoders = nullptr;//WIC������

	ID2D1GdiInteropRenderTarget* GdiInterop = nullptr;//GDI�������ӿ�
};
namespace SOUI
{
	class SBlurWindow : public SWindow
	{
		SOUI_CLASS_NAME(SBlurWindow, L"blurWnd")

	public:
		SBlurWindow(void);
		~SBlurWindow(void);

	protected:
		void OnPaint(IRenderTarget* pRT);

	private:
		MDirect2D* d2dClass;
		int m_nblur = 1;

		SOUI_ATTRS_BEGIN()
			ATTR_INT(L"blur", m_nblur, TRUE)
		SOUI_ATTRS_END()

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
		SOUI_MSG_MAP_END()
	};
}