#include "stdafx.h"
#include "SBlurWindow.h"
//#include <time.h>
//GDIP获取编码器ID
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

MDirect2D::MDirect2D()
{

	//初始化
	bool Success = false;

	// 创建D2D工厂
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		reinterpret_cast<void**>(&D2DFactory));

	//创建D3D设备
	if (SUCCEEDED(hr))
	{
		// D3D11 创建flag 
		// 一定要有D3D11_CREATE_DEVICE_BGRA_SUPPORT，否则创建D2D设备上下文会失败
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		// Debug状态 有D3D DebugLayer就可以取消注释
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		// 创建设备
		hr = D3D11CreateDevice(
			nullptr,					// 设为空选择默认设备
			D3D_DRIVER_TYPE_HARDWARE,	// 指定为硬件渲染
			nullptr,					// 强行指定WARP渲染 D3D_DRIVER_TYPE_WARP 没有软件接口
			creationFlags,				// 创建flag
			featureLevels,				// 欲使用的特性等级列表
			ARRAYSIZE(featureLevels),	// 特性等级列表长度
			D3D11_SDK_VERSION,			// SDK 版本
			&D3DDevice,				// 返回的D3D11设备指针
			&D3DfeatureLevels,			// 返回的特性等级
			&D3DDeviceContext);		// 返回的D3D11设备上下文指针}

		D3DDevice->QueryInterface(IID_IDXGIDevice1, reinterpret_cast<void**>(&DXGIDevice));

		// 创建D2D设备
		if (SUCCEEDED(hr)) {
			hr = D2DFactory->CreateDevice(DXGIDevice, &D2DDevice);
			// 创建D2D上下文
			if (SUCCEEDED(hr))
			{
				D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &D2DDeviceContext);
				// 创建WIC工厂
				if (SUCCEEDED(hr))
				{
					D2DDeviceContext->SetUnitMode(D2D1_UNIT_MODE::D2D1_UNIT_MODE_PIXELS);

					CoInitialize(NULL);
					hr = CoCreateInstance(
						CLSID_WICImagingFactory,//如果使用Win8+的话可以使用 CLSID_WICImagingFactory2
						nullptr,
						CLSCTX_INPROC_SERVER,
						IID_IWICImagingFactory,//win8+ IID_IWICImagingFactory
						reinterpret_cast<void**>(&WICFactory)
					);
					if (SUCCEEDED(hr))
						Success = true;
					else
						OutputDebugStringW(L"Direct2D.cpp: CoCreateInstance Error\n");
				}
				else
					OutputDebugStringW(L"Direct2D.cpp: D2DDevice->CreateDeviceContext Error\n");
			}
			else
				OutputDebugStringW(L"Direct2D.cpp: D2DFactory->CreateDevice Error\n");
		}
		else
			OutputDebugStringW(L"Direct2D.cpp: D3D11CreateDevice Error\n");
	}
	else
		OutputDebugStringW(L"Direct2D.cpp: D2D1CreateFactory Error\n");

	if (Success)
	{
		//创建特效
		D2DDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &D2DEffect);
		//取GDI互操作
		D2DDeviceContext->QueryInterface(IID_PPV_ARGS(&GdiInterop));
	}
}

MDirect2D::~MDirect2D()
{

}

Gdiplus::Bitmap* MDirect2D::GaussianBlur(Gdiplus::Bitmap* bitmap, int radius)
{
	MResources res = ID2DLoadImage(bitmap, radius);

	IStream* stream = SHCreateMemStream(res.data, res.Size);

	Gdiplus::Bitmap* outBitmap = new Gdiplus::Bitmap(stream, NULL);
	stream->Release();
	delete[] res.data;

	return outBitmap;
}

IBitmap* MDirect2D::IGaussianBlur(Gdiplus::Bitmap* bitmap, int radius)
{
	MResources res = ID2DLoadImage(bitmap, radius);
	IBitmap* outBitmap = SResLoadFromMemory::LoadImageW(res.data, res.Size);
	delete[] res.data;

	return outBitmap;
}

IBitmap* MDirect2D::IGaussianBlur(HBITMAP bitmap, int radius)
{
	IWICBitmap* wicbitmap = nullptr;
	WICFactory->CreateBitmapFromHBITMAP(bitmap, 0, WICBitmapUseAlpha, &wicbitmap);
	MResources outRes = ID2DGaussianBlur(wicbitmap, radius);
	wicbitmap->Release();

	IBitmap* outBitmap = SResLoadFromMemory::LoadImageW(outRes.data, outRes.Size);
	delete[] outRes.data;

	return outBitmap;
}


void MDirect2D::IGaussianBlur(HDC dc, int width, int height, int cx, int cy, int xSrc, int ySrc, HBITMAP bitmap, int radius)
{
	IWICBitmap* wicbitmap = nullptr;
	WICFactory->CreateBitmapFromHBITMAP(bitmap, 0, WICBitmapUseAlpha, &wicbitmap);
	MResources outRes = ID2DGaussianBlur(wicbitmap, radius, true, dc, width, height, cx, cy, xSrc, ySrc);
	wicbitmap->Release();
}

void MDirect2D::Release()
{
	//释放资源
	SafeRelease(D3DDevice);
	SafeRelease(D3DDeviceContext);
	SafeRelease(DXGIDevice);
	SafeRelease(WICFactory);
	SafeRelease(WICEncoders);
	SafeRelease(D2DDeviceContext);
	SafeRelease(D2DDevice);
	SafeRelease(D2DFactory);
	SafeRelease(D2DEffect);
	SafeRelease(GdiInterop);
	SafeRelease(D2DBitmap);

	delete this;
}

MResources MDirect2D::ID2DGaussianBlur(IWICBitmap*& bitmap, int radius, bool isDC, HDC pDC, int width, int height, int cx, int cy, int xSrc, int ySrc)
{
	//释放旧资源
	if (D2DBitmap)
	{
		D2DBitmap->Release();
		D2DBitmap = nullptr;
	}
	//创建D2D位图
	D2D1_PIXEL_FORMAT format;
	//像素格式
	format.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;//支持透明
	format.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	FLOAT dpiX, dpiY;
	D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 bitmapProp;
	bitmapProp.dpiX = dpiX;
	bitmapProp.dpiY = dpiY;
	bitmapProp.pixelFormat = format;
	//用法 作为渲染目标 和 GDI兼容是必要的
	bitmapProp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE;
	bitmapProp.colorContext = 0;

	HRESULT hr = D2DDeviceContext->CreateBitmapFromWicBitmap(bitmap, bitmapProp, &D2DBitmap);

	//创建渲染目标
	D2D1_SIZE_U imgSize;
	bitmap->GetSize(&imgSize.width, &imgSize.height);
	ID2D1Bitmap1* tmpBitmap_ = nullptr;
	hr = D2DDeviceContext->CreateBitmap(imgSize, 0, 0, bitmapProp, &tmpBitmap_);
	D2DDeviceContext->SetTarget(tmpBitmap_);

	ID2D1Image* tmpImage = nullptr;

	if (SUCCEEDED(hr))
	{
		if (!D2DBitmap) {
			return MResources();
			tmpBitmap_->Release();
		}
		//置入图像于属性设定
		D2DEffect->SetInput(0, D2DBitmap);
		D2DEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, (float)radius);
		//绘制效果
		D2DDeviceContext->BeginDraw();

		D2DDeviceContext->DrawBitmap(D2DBitmap);
		D2DDeviceContext->DrawImage(D2DEffect);

		if (isDC) {
			//如果是直接绘制到DC
			HDC D2DpDC;
			hr = GdiInterop->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &D2DpDC);

			if (SUCCEEDED(hr))
			{
				BitBlt(pDC, cx, cy, width, height, D2DpDC, xSrc, ySrc, SRCCOPY);
				GdiInterop->ReleaseDC(nullptr);
			}
		}

		tmpBitmap_->Release();

		hr = D2DDeviceContext->EndDraw();

		MResources retResources = MResources();

		if (!isDC) {
			//保存为字节集
			D2DEffect->GetOutput(&tmpImage);

			//创建编码WIC流和帧编码器
			IWICStream* stream = nullptr;
			IWICBitmapEncoder* encoder = nullptr;
			IWICBitmapFrameEncode* enFrame = nullptr;
			hr = WICFactory->CreateStream(&stream);
			hr = WICFactory->CreateEncoder(GUID_ContainerFormatJpeg, &GUID_VendorMicrosoft, &encoder);
			//初始化WIC流从字节流
			IStream* istream;
			CreateStreamOnHGlobal(NULL, true, &istream);
			hr = stream->InitializeFromIStream(istream);
			//创建新帧
			IPropertyBag2* propbag = nullptr;
			encoder->Initialize(stream, WICBitmapEncoderNoCache);
			hr = encoder->CreateNewFrame(&enFrame, &propbag);

			hr = enFrame->Initialize(0);
			//设定图片信息

			/*WICImageParameters imgParams;

			imgParams.DpiX = dpiX;
			imgParams.DpiY = dpiY;
			imgParams.PixelFormat = format;
			UINT height, width;
			bitmap->GetSize(&width, &height);
			imgParams.PixelHeight = height;
			imgParams.PixelWidth = width;
			imgParams.Top = 0;
			imgParams.Left = 0;*/
			//WICEncoders->WriteFrame(tmpImage, enFrame, &imgParams); //win8+ 使用WICImagingFactory2

			WICEncoders->CreateNewFrame(&enFrame, &propbag);
			enFrame->Commit();
			encoder->Commit();

			//读取全部字节
			LARGE_INTEGER dlibMove = { 0 };
			ULARGE_INTEGER size;
			stream->Seek(dlibMove, STREAM_SEEK_END, &size);
			stream->Seek(dlibMove, STREAM_SEEK_SET, 0);

			BYTE* buffer = new BYTE[(unsigned int)size.QuadPart];
			DWORD bytesRead = 0;
			istream->Read(buffer, (unsigned int)size.QuadPart, &bytesRead);

			retResources = { buffer, bytesRead };
			//释放资源
			istream->Release();
			stream->Release();
			encoder->Release();
			enFrame->Release();
			tmpImage->Release();
			propbag->Release();
		}
		return retResources;
	}

	return MResources();
}

MResources MDirect2D::ID2DLoadImage(Gdiplus::Bitmap* bitmap, int radius)
{
	IStream* pBuf = NULL;
	HGLOBAL hMemImage = GlobalAlloc(GMEM_MOVEABLE, 0);
	CreateStreamOnHGlobal(hMemImage, TRUE, &pBuf);
	CLSID PNGClsid;
	GetEncoderClsid(L"image/png", &PNGClsid);
	bitmap->Save(pBuf, &PNGClsid);
	BYTE* pImage = (BYTE*)GlobalLock(hMemImage);
	DWORD hSize = (DWORD)GlobalSize(hMemImage);

	IStream* stream = SHCreateMemStream(pImage, hSize);
	//创建解码器
	IWICBitmapDecoder* decoder;
	HRESULT hr = WICFactory->CreateDecoderFromStream(stream, &GUID_VendorMicrosoft, WICDecodeMetadataCacheOnDemand, &decoder);

	//截取一帧
	IWICBitmapFrameDecode* frame;
	decoder->GetFrame(0, &frame);

	IWICFormatConverter* converter;
	hr = WICFactory->CreateFormatConverter(&converter);

	IWICPalette* palette = nullptr;
	hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, palette, 0, WICBitmapPaletteTypeCustom);

	//创建自位图源
	IWICBitmap* wicbitmap = nullptr;
	hr = WICFactory->CreateBitmapFromSource(converter, WICBitmapCreateCacheOption::WICBitmapNoCache, &wicbitmap);

	MResources outResources = ID2DGaussianBlur(wicbitmap, radius);

	//释放资源
	pBuf->Release();
	decoder->Release();
	frame->Release();
	converter->Release();
	wicbitmap->Release();
	stream->Release();
	GlobalFree(hMemImage);

	return outResources;
}


ULONG_PTR m_gdiplusToken;
//初始化GDI
Gdiplus::GdiplusStartupInput StartupInput;
int ret = Gdiplus::GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
//初始化D2D
MDirect2D* MD2D = new MDirect2D();

namespace SOUI
{
	SBlurWindow::SBlurWindow(void) {
		d2dClass = (MDirect2D*)MD2D;
	}
	SBlurWindow::~SBlurWindow(void) {}

	void SBlurWindow::OnPaint(IRenderTarget* pRT)
	{
		CRect rect = GetClientRect();
		if (IsVisible() && pRT)
		{
			SPainter painter;
			BeforePaint(pRT, painter);

			HDC hSrcDC = pRT->GetDC();
			//为指定设备描述表创建兼容的内存设备描述表
			HDC hMemDC = CreateCompatibleDC(hSrcDC);
			//创建一个与指定设备描述表兼容的位图
			HBITMAP hBitmap = CreateCompatibleBitmap(hSrcDC, rect.Width(), rect.Height());
			//把新位图选到内存设备描述表中
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
			//把屏幕设备描述表拷贝到内存设备描述表中
			BitBlt(hMemDC, rect.left, rect.top, rect.Width(), rect.Height(), hSrcDC, rect.left, rect.top, SRCCOPY);
			//得到屏幕位图的句柄
			hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
			//图像处理

			//long clocks = clock();

			if (hBitmap)
				d2dClass->IGaussianBlur(hSrcDC, rect.Width(), rect.Height(), rect.left, rect.top, rect.left, rect.top, hBitmap, m_nblur);

			//AddDebugOutput(to_wstring(clock() - clocks).c_str());

			//释放相关资源
			DeleteDC(hMemDC);
			DeleteObject(hOldBitmap);
			DeleteObject(hBitmap);
			pRT->ReleaseDC(hSrcDC);

			AfterPaint(pRT, painter);
		}
	}
}