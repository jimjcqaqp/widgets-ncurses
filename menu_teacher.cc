#include "menu_teacher.h"
#include "Dialog.h"

void menu_teacher()
{
	Dialog dialog;
	dialog.hc = dialog.vc = true;
	dialog.addlinebody("Menu teacher");
	dialog.start();
}

