// ImageListHelper.h

// ImageLists from bmp and icons, resoursec and files (see ImageLists.cpp)
extern HIMAGELIST hImgList_current;
extern int curIconSizex, curIconSizexy;

extern HIMAGELIST hImgList_bmps_24, hImgList_bmps_32, hImgList_bmps_48; // from bmp res

extern HIMAGELIST hImgList_icons_24, hImgList_icons_32, hImgList_icons_48, // from icon res
hImgList_icons_64, hImgList_icons_72, hImgList_icons_128;

extern HIMAGELIST hImgList_icons_files_48, hImgList_icons_files_64;       //  from icon files

HIMAGELIST CreateImgList_Icons_Files(HINSTANCE hinst, int cxDesired, int cyDesired, UINT fuLoad);
HIMAGELIST CreateImgList_Icons(HINSTANCE hinst, WORD IDI_ICON_START, WORD IDI_ICON_FIN,
	int cxDesired, int cyDesired, UINT fuLoad);
HIMAGELIST CreateImgList_Bmps(HINSTANCE hinst, WORD IDB_BITMAP_, WORD IDB_BITMAP_FIN,
	int cxDesired, int cyDesired, UINT fuLoad, COLORREF crMask);

HIMAGELIST CreateImgList_Pngs(HINSTANCE hinst, WORD IDB_BITMAP_, WORD IDB_BITMAP_FIN,
	int cxDesired, int cyDesired, UINT fuLoad, COLORREF crMask);