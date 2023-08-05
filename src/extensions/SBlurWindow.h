#pragma once
#include <D3D11.h>
#include <d2d1_1.h>
#include <dxgi.h>
#include <wincodec.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "windowscodecs.lib" )

//使用Direct2D 1.1 最低需求win7

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
	高斯模糊
	bitmap - 输入图像
	radius - 模糊半径(模糊度)

	return 输出图像
	*/
	Gdiplus::Bitmap* GaussianBlur(Gdiplus::Bitmap* bitmap, int radius);
	IBitmap* IGaussianBlur(Gdiplus::Bitmap* bitmap, int radius);
	IBitmap* IGaussianBlur(HBITMAP bitmap, int radius);

	/*
	高斯模糊
	dc - 输出到指定设备场景
	width - 输出宽度
	height - 输出高度
	cx - 输出位置 X
	cy - 输出位置 Y
	bitmap - 输入位图
	radius - 模糊半径(模糊度)

	return 输出图像
	*/
	void IGaussianBlur(HDC dc, int width, int height, int cx, int cy, int xSrc, int ySrc, HBITMAP bitmap, int radius);

	void Release();

private:
	MResources ID2DGaussianBlur(IWICBitmap*& bitmap, int radius, bool isDC = false, HDC pDC = 0, int width = 0, int height = 0, int cx = 0, int cy = 0, int xSrc = 0, int ySrc = 0);
	MResources ID2DLoadImage(Gdiplus::Bitmap* bitmap, int radius);

	ID3D11Device* D3DDevice = nullptr;//D3D设备
	D3D_FEATURE_LEVEL D3DfeatureLevels;//D3D特性等级
	ID3D11DeviceContext* D3DDeviceContext = nullptr;//D3D设备上下文

	ID2D1Factory1* D2DFactory = nullptr;//D2D工厂
	ID2D1Device* D2DDevice = nullptr;//D2D设备
	IDXGIDevice* DXGIDevice = nullptr;//DXIG设备
	ID2D1DeviceContext* D2DDeviceContext = nullptr;//D2D设备上下文
	IWICImagingFactory* WICFactory = nullptr;//WIC工厂

	ID2D1Effect* D2DEffect = nullptr;//D2D特效
	ID2D1Bitmap1* D2DBitmap = nullptr;//D2D位图

	IWICBitmapEncoder* WICEncoders = nullptr;//WIC编码器

	ID2D1GdiInteropRenderTarget* GdiInterop = nullptr;//GDI互操作接口
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