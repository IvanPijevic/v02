#include "nwpwin.h"
#include "res.h"

//edit, button, list_box for child windows

class edit : public vsite::nwp::window
{
public:
	std::string class_name() override { return "Edit"; }
};

class button : public vsite::nwp::window
{
public:
	std::string class_name() override { return "Button"; }
};

class list_box : public vsite::nwp::window
{
public:
	std::string class_name() override { return "Listbox"; }
};


class main_window : public vsite::nwp::window
{
protected:
	int on_create(CREATESTRUCT* pcs) override;
	void on_command(int id) override;
	void on_destroy() override;

private:
	edit m_edit;
	button m_add;
	button m_remove;
	list_box m_listBox;
};

int main_window::on_create(CREATESTRUCT* pcs)
{
	//child windows
	m_edit.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, IDC_EDIT, 150, 50, 100, 50);

	m_add.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Add", IDC_ADD, 150, 100, 100, 50);
	m_remove.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, "Remove", IDC_REMOVE, 150, 150, 100, 50);

	m_listBox.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, IDC_LB, 10, 50, 100, 100);

	EnableWindow(m_remove, false);
	
	return 0;
}

void main_window::on_command(int id){
	switch(id){
		case ID_FILE_EXIT:
			on_destroy();
			break;

		case ID_HELP_ABOUT:
			MessageBox(*this, "Add - add to list, Remove - remove from list", "Help", MB_OK | MB_ICONINFORMATION);
			break;

		case IDC_ADD:
			char msg[32];
			GetDlgItemText(*this, IDC_EDIT, msg, sizeof(msg));
			SendDlgItemMessage(*this, IDC_LB, LB_ADDSTRING, 0, (LPARAM)msg);
			EnableWindow(m_remove, true);

			break;

		case IDC_REMOVE:
			//delete target
			int selectedIndex = SendMessage(m_listBox, LB_GETCURSEL, 0, 0);

			if (selectedIndex != LB_ERR) 
			{
				SendMessage(m_listBox, LB_DELETESTRING, selectedIndex, 0);
			}

			//no target - Remove button inactive
			int n = SendMessage(m_listBox, LB_GETCOUNT, 0, 0);
			if (n != 0) 
			{
				EnableWindow(m_remove, true);
			}
			else
			{
				EnableWindow(m_remove, false);
			}
			break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	main_window w; 
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "win", (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_V2)));
	vsite::nwp::set_icons(instance, w, IDI_V2);
	vsite::nwp::application app;
	return app.run();
}
