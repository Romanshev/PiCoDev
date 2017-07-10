//OnSliderBtn_MoveIcons.h
//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwSlider, hwEdit, hwMain;

extern int idSliderBtn_Zoom;   // embed trackbar 
LRESULT	  OnSliderBtn_MoveIcons(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//  Slider demo vars
extern int padScale_Slider; // Increase and decrease Centered rect by pad scaling
extern int imgSpaceSlider;	// Increase distance between Images of buttons on bottom part of app window
extern RECT rcImgSliding;