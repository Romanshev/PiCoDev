//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwMain;

// simple Tb button (or Cmd buttons) idCmdBtn_Img_Label_[Use]_Style
extern int idCmdBtn_Open;	// demo of standart button, the templated colors and actions like Move, Hide, Show, Delete (menu Std)
LRESULT	  OnCmdBtn_OpenFile(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern int idCmdBtn_Save, idCmdBtn_SaveAs;	// demo of transforms of standart buttons into checkgroup couple (Menu Checkgroup)
LRESULT	  OnCmdBtn_Save  (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT	  OnCmdBtn_SaveAs(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);