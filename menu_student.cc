#include "menu_student.h"
#include "Dialog.h"

void menu_student()
{
		Dialog dialog;
		dialog.hc = dialog.vc = true;
		dialog.addlinebody("Menu student");
		dialog.start();
}

