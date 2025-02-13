#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <atlimage.h>
using namespace std;
inline void showmenu()
{
	puts("从以下列表中选择：");
	puts("输入1以显示图片；");
	puts("输入2以转换图片；");
	puts("输入3以将图片转为灰度图像；");
	puts("输入4以将图片转为黑白图像；");
	puts("输入5以将图片降质；");
	puts("输入6以将图片反色；");
	puts("输入7以将图片左右反转；");
	puts("输入8以将图片上下反转；");
    puts("输入9以将宽度相等的图片上下拼接为一张；");
    puts("输入a以将高度相等的图片左右拼接为一张。");
}
inline void showimg(const CImage& img)
{
	HDC hdc = GetDC(GetConsoleWindow());
	while (true)
	{
		img.Draw(hdc, 0, 0);
		Sleep(100);
	}
}
inline void imgturngray(CImage& img)
{
	COLORREF pix;
	int X = img.GetWidth(), Y = img.GetHeight();
	double avg;
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			pix = img.GetPixel(i, j);
			avg = (GetRValue(pix) * 0.299 + GetGValue(pix) * 0.587 + GetBValue(pix) * 0.114);
			img.SetPixelRGB(i, j, avg, avg, avg);
		}
	}
}
inline void imgturnblackwhite(CImage& img)
{
	COLORREF pix;
	int X = img.GetWidth(), Y = img.GetHeight();
	double avg;
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			pix = img.GetPixel(i, j);
			avg = (GetRValue(pix) * 0.299 + GetGValue(pix) * 0.587 + GetBValue(pix) * 0.114);
			if (avg <= 127.5) img.SetPixelRGB(i, j, 0, 0, 0);
			else img.SetPixelRGB(i, j, 255, 255, 255);
		}
	}
}
inline void imgdegradation(const CImage& oriimg, CImage& newimg, int val)
{
	COLORREF pix;
	int X = oriimg.GetWidth();
	int Y = oriimg.GetHeight();
	newimg.Create(X / val, Y / val, oriimg.GetBPP());
	for (int i = 0; i < X; i += val)
	{
		for (int j = 0; j < Y; j += val)
		{
			pix = oriimg.GetPixel(i, j);
			newimg.SetPixel(i / 2, j / 2, pix);
		}
	}
}
inline void imgdecolor(CImage& img)
{
	COLORREF pix;
	int X = img.GetWidth(), Y = img.GetHeight();
	byte r, g, b;
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			pix = img.GetPixel(i, j);
			r = 255 - GetRValue(pix);
			g = 255 - GetGValue(pix);
			b = 255 - GetBValue(pix);
			img.SetPixelRGB(i, j, r, g, b);
		}
	}
}
inline void imgreverse_upsidedown(CImage& img)
{
	COLORREF pix1,pix2;
	int X = img.GetWidth(), Y = img.GetHeight();
	for (int i = 0; i < X / 2; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			pix1 = img.GetPixel(i, j);
			pix2 = img.GetPixel(X - i, j);
			img.SetPixel(i, j, pix2);
			img.SetPixel(X - i, j, pix1);
		}
	}
}
inline void imgreverse_leftsideright(CImage& img)
{
	COLORREF pix1, pix2;
	int X = img.GetWidth(), Y = img.GetHeight();
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y / 2; j++)
		{
			pix1 = img.GetPixel(i, j);
			pix2 = img.GetPixel(i, Y - j);
			img.SetPixel(i, j, pix2);
			img.SetPixel(i, Y - j, pix1);
		}
	}
}
inline bool connect_updown(const CImage& img1, const CImage& img2, CImage& img)
{
    int width1 = img1.GetWidth();
    int height1 = img1.GetHeight();
    int width2 = img2.GetWidth();
    int height2 = img2.GetHeight();
    if (width1 != width2) return false;
    img.Create(width1, height1 + height2, 32);
    for (int i = 0; i < width1; i++)
    {
        for (int j = 0; j < height1; j++)
        {
            img.SetPixel(i, j, img1.GetPixel(i, j));
        }
    }
    for (int i = 0; i < width1; i++)
    {
        for (int j = 0; j < height2; j++)
        {
            img.SetPixel(i, j + height1, img2.GetPixel(i, j));
        }
    }
    return true;
}
inline bool connect_leftright(const CImage& img1, const CImage& img2, CImage& img)
{
    int width1 = img1.GetWidth();
    int height1 = img1.GetHeight();
    int width2 = img2.GetWidth();
    int height2 = img2.GetHeight();
    if (height1 != height2) return false;
    img.Create(width1 + width2, height1, 32);
    for (int i = 0; i < width1; i++)
    {
        for (int j = 0; j < height1; j++)
        {
            img.SetPixel(i, j, img1.GetPixel(i, j));
        }
    }
    for (int i = 0; i < width2; i++)
    {
        for (int j = 0; j < height1; j++)
        {
            img.SetPixel(i + width1, j, img2.GetPixel(i, j));
        }
    }
    return true;
}
void mainfunc();
inline void exeagain()
{
	puts("是否再进行一次处理？是请输入y，否请输入n");
	char c = _getch();
    switch (c)
    {
    case 'y':
        system("cls");
        mainfunc();
        break;
    case 'n':
        exit(0);
    default:
        MessageBox(GetDesktopWindow(), _T("无法执行此操作！"), _T("内部处理"), MB_OK);
        exit(0);
    };
}
void mainfunc()
{
    register string s; register CImage image, image2, image3; register CString path1, path2, path3; register int n;
    showmenu();
    char c = _getch();
    system("cls");
    switch (c)
    {
    case '1':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        image.Draw(GetDC(GetConsoleWindow()), 0, 0);
        MessageBox(GetDesktopWindow(), _T("图像显示成功"), _T("内部处理"), MB_OK);
        showimg(image);
        break;
    case '2':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入转换后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        image.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像转换成功"), _T("内部处理"), MB_OK);
        break;
    case '3':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入转换后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        imgturngray(image);
        image.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        break;
    case '4':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入转换后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        imgturnblackwhite(image);
        image.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        break;
    case '5':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入降质后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        system("cls");
        puts("输入降质系数："); cin >> n;
        system("cls");
        imgdegradation(image, image2, n);
        image2.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        break;
    case '6':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入反色后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        imgdecolor(image);
        image.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        break;
    case '7':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入反转后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        imgreverse_upsidedown(image);
        image.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        break;
    case '8':
        puts("输入源文件路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入反转后文件路径："); cin >> s;
        system("cls");
        path2 = s.data();
        imgreverse_leftsideright(image);
        image.Save((LPCTSTR)path2);
        system("cls");
        MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        break;
    case '9':
        puts("输入源文件1（在上）路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入源文件2（在下）路径："); cin >> s;
        system("cls");
        path2 = s.data();
        image2.Load((LPCTSTR)path2);
        if (image2.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入拼接后文件路径："); cin >> s;
        system("cls");
        path3 = s.data();
        if (connect_updown(image, image2, image3))
        {
            image3.Save((LPCTSTR)path3);
            system("cls");
            MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        }
        else
        {
            system("cls");
            MessageBox(GetDesktopWindow(), _T("图像宽度不一致！"), _T("内部处理"), MB_OK);
        }
        break;
    case 'a':
        puts("输入源文件1（在左）路径："); cin >> s;
        system("cls");
        path1 = s.data();
        image.Load((LPCTSTR)path1);
        if (image.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入源文件2（在右）路径："); cin >> s;
        system("cls");
        path2 = s.data();
        image2.Load((LPCTSTR)path2);
        if (image2.IsNull())
        {
            MessageBox(GetDesktopWindow(), _T("文件路径不存在！"), _T("内部处理"), MB_OK);
            break;
        }
        puts("输入拼接后文件路径："); cin >> s;
        system("cls");
        path3 = s.data();
        if (connect_leftright(image, image2, image3))
        {
            image3.Save((LPCTSTR)path3);
            system("cls");
            MessageBox(GetDesktopWindow(), _T("图像处理成功"), _T("内部处理"), MB_OK);
        }
        else
        {
            system("cls");
            MessageBox(GetDesktopWindow(), _T("图像高度不一致！"), _T("内部处理"), MB_OK);
        }
        break;
    default:
        MessageBox(GetDesktopWindow(), _T("无法执行此操作！"), _T("内部处理"), MB_OK);
        break;
    }
    exeagain();
}
