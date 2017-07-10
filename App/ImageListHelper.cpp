
#include "stdafx.h"
#include "ImageListHelper.h" 



// ImageLists.cpp: how to create imagelists from bitmaps and icons from resources or files to get
// get fit pictures into toolbars for your target group of users (age, abilities etc)
// Current version is for 32-bit images ( not 16/24-bit) with alpha channel. 

//  Image List Creation Flags  ILC_COLOR32     32-bit DIB section
//                          ILC_ORIGINALSIZE   (Vista ...)Imagelist should accept smaller than
//                                             set images and apply original size based on image added
//                       ILC_HIGHQUALITYSCALE  (Vista...)Imagelist should enable use of the high quality scaler

//  (ImageList_)LoadImage function flags how to load bitmaps or icons
//   UINT fuLoad one or more flags LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE
//                               LR_DEFAULTSIZE etc

//  Masks in ImageList_AddMasked and ImageList_Add functions
// 	COLORREF crMask  	The color used to generate a mask    ///////////////////////////
//  Each pixel of this color in the specified bitmap, cursor, or icon is changed to black,
//	and the corresponding bit in the mask is set to 1 
//	  crMask = CLR_NONE    - no mask is generated
//	  crMask = CLR_DEFAULT - the color of the pixel at the upper-left corner 
//	                         of the image is treated as the mask color



// image lists for 16x16, 24x24 and 32x32 of 32 - bit colors bmp and black mask
HIMAGELIST //hImgList_bmps_16, hImgList_bmps_24, hImgList_bmps_32, // by CreateImgList_Bmps_XX-old()
//hImgList_icons_32, hImgList_icons_48, // from 32x32 and 48x48 bmp in icons by CreateImgList_Icons_48_old()
hImgList_icons_64_file;// from 48x48 bmp in icons loaded frpm files by CreateImgList_Icons_64_Files()


///////////////////////////////////////////////////////////////////////////////////////////////////////
// 16x16 24x24 32x32 48x48 XXxYY bmp from resources
HIMAGELIST CreateImgList_Bmps(HINSTANCE hInstance, WORD IDB_BITMAP_START, WORD IDB_BITMAP_FIN,
	int cxDesired, int cyDesired, UINT fuLoad, COLORREF crMask)
{
	//INITCOMMONCONTROLSEX icex;
	HANDLE hImg;
	HIMAGELIST hImgList; // return value
	const int Img_X = cxDesired;
	const int Img_Y = cyDesired;
	const UINT flags = ILC_COLOR32;// | ILC_MASK;

	//icex.dwSize = sizeof(INITCOMMONCONTROLSEX);// Ensure that the common control DLL is loaded. 
	//icex.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&icex);

	hImgList = ImageList_Create(Img_X, Img_Y, flags, 0, 0);
	// hImgList = ImageList_LoadImageW(hInstance, MAKEINTRESOURCE(id), Img_X, 1, crMask, IMAGE_BITMAP, fuLoad);
	// if we have full set in one long bitmap and no need next cycle for(){}

	for (WORD id = IDB_BITMAP_START; id <= IDB_BITMAP_FIN; id++){

		hImg = LoadImageW(hInstance, MAKEINTRESOURCE(id), IMAGE_BITMAP, Img_X, Img_Y, fuLoad);
		ImageList_AddMasked(hImgList, (HBITMAP)hImg, crMask);
		DeleteObject(hImg);
	}
	return hImgList;
}

// ver with loaded commctl32.dll
// 16x16 24x24 32x32 48x48 bmp and 256x256 png from icon resources
HIMAGELIST CreateImgList_Icons(HINSTANCE hInstance, WORD IDI_ICON_START, WORD IDI_ICON_FIN,
	int cxDesired, int cyDesired, UINT fuLoad)
{

	HICON hIcon;
	HIMAGELIST hImgList;

	const int Img_X = cxDesired;
	const int Img_Y = cyDesired;
	const UINT flags = ILC_COLOR32; // for Vista and later 	ILC_ORIGINALSIZE and ILC_HIGHQUALITYSCALE



	hImgList = ImageList_Create(Img_X, Img_Y, flags, 0, 0);

	for (WORD id = IDI_ICON_START; id <= IDI_ICON_FIN; id++){
		hIcon = (HICON)LoadImageW(hInstance, MAKEINTRESOURCE(id), IMAGE_ICON, Img_X, Img_Y, fuLoad);
		ImageList_AddIcon(hImgList, hIcon);
		//DestroyIcon(hIcon);// take out of 
	}
	//DestroyIcon(hIcon);
	return hImgList;
}

// 16x16 24x24 32x32 48x48 bmp and 256x256 png from icon resources
HIMAGELIST CreateImgList_Icons_old(HINSTANCE hInstance, WORD IDI_ICON_START, WORD IDI_ICON_FIN,
	int cxDesired, int cyDesired, UINT fuLoad)
{
	//INITCOMMONCONTROLSEX icex;
	HIMAGELIST hImgList;
	HANDLE hImg;

	const int Img_X = cxDesired;
	const int Img_Y = cyDesired;
	const UINT flags = ILC_COLOR32; // for Vista and later 	ILC_ORIGINALSIZE and ILC_HIGHQUALITYSCALE
	// Ensure that the common control DLL is loaded. 
	//icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icex.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&icex);
	hImgList = ImageList_Create(Img_X, Img_Y, flags, 0, 0);

	for (WORD id = IDI_ICON_START; id <= IDI_ICON_FIN; id++){

		hImg = LoadImageW(hInstance, MAKEINTRESOURCE(id), IMAGE_ICON, Img_X, Img_Y, fuLoad);
		ImageList_AddIcon(hImgList, (HICON)hImg);
		//DeleteObject(hImg); // take out of 		
	}
	DeleteObject(hImg);
	return hImgList;
}

// 16x16 24x24 32x32 48x48 bmp and 256x256 png from icon files
void CreateImgList_Icons_64_Files() // we load images from files not from compiled resources
{
	//INITCOMMONCONTROLSEX icex;

	HANDLE hImg;

	const int Img_X = 64;
	const int Img_Y = 64;
	const UINT flags = ILC_COLOR32;// | ILC_MASK;
	//                 ILC_COLOR24 | ILC_MASK;  // black or gray
	//                 ILC_COLOR16 | ILC_MASK; // magenta RGB(255,0,255);
	// Ensure that the common control DLL is loaded. 
	//icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icex.dwICC = ICC_WIN95_CLASSES;
	//InitCommonControlsEx(&icex);

	hImgList_icons_64_file = ImageList_Create(Img_X, Img_Y, flags, 0, 0);

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Book_Green.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);
	DeleteObject(hImg);                                         // 0

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Book_JournalwPen.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);               // 1
	DeleteObject(hImg);

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Music_Note_Single.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);
	DeleteObject(hImg);                                         // 2

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\PaintPaletteFINAL.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);                // 3
	DeleteObject(hImg);

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\computer.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);
	DeleteObject(hImg);                                         //4

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Globe.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);                // 5
	DeleteObject(hImg);

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Hyperlink.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);
	DeleteObject(hImg);                                         // 6

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\cog.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);                // 7
	DeleteObject(hImg);

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Network_Fax.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);
	DeleteObject(hImg);                                         // 8

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\network_fax_default.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);                // 9
	DeleteObject(hImg);

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Network_Folder.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);
	DeleteObject(hImg);                                         // 10

	hImg = LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Network_Internet.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList_icons_64_file, (HICON)hImg);               // 11
	DeleteObject(hImg);
}

// 16x16 24x24 32x32 48x48 bmp and 256x256 png from icon files
HIMAGELIST CreateImgList_Icons_Files(HINSTANCE hInstance,
	//LPSTR IDI_ICON_START, LPSTR IDI_ICON_FIN,
	int cxDesired, int cyDesired, UINT fuLoad) // we load images from files not from compiled resources
{
	HICON hIcon; HIMAGELIST hImgList;

	const int Img_X = cxDesired;
	const int Img_Y = cyDesired;
	const UINT flags = ILC_COLOR32;// | ILC_MASK;
	//                 ILC_COLOR24 | ILC_MASK;  // black or gray
	//                 ILC_COLOR16 | ILC_MASK; // magenta RGB(255,0,255);

	hImgList = ImageList_Create(Img_X, Img_Y, flags, 0, 0);

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Book_Green.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);    // 0

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Book_JournalwPen.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);    // 1


	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Music_Note_Single.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);     // 2

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\PaintPaletteFINAL.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);     // 3


	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\computer.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);     //4

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Globe.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);      // 5


	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Hyperlink.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);     // 6

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\cog.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);      // 7	

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Network_Fax.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);      // 8

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\network_fax_default.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);     // 9


	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Network_Folder.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);    // 10

	hIcon = (HICON)LoadImageW(0, TEXT("..\\Images\\Toolbar\\Icons\\Network_Internet.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	ImageList_AddIcon(hImgList, hIcon);   // 11

	//DestroyIcon(hIcon);
	return hImgList;

}