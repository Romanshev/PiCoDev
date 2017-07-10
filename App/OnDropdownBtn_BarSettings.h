//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwMain; // hwSlider, hwEdit,

extern int idDropdownBtn_BarSettings;  // styles, icon sizes, show/hide btns etc...
//   DropDown button has popup menu on arrow and cmd on default button
LRESULT   OnDropdownBtn_BarSettings(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);