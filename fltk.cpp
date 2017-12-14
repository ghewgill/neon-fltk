#include <iso646.h>
#include <string>

#include "neonext.h"

#ifdef _MSC_VER
#include <windows.h>
#pragma warning(disable: 4244)
#pragma warning(disable: 4996) // disable warnings about sprintf() in Fl headers
#endif

#include <FL/Fl.H>
#include <FL/Fl_Adjuster.H>
#include <FL/fl_ask.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Clock.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Copy_Surface.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Fill_Dial.H>
#include <FL/Fl_Fill_Slider.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Hor_Fill_Slider.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Line_Dial.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Nice_Slider.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Positioner.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Round_Clock.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Timer.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Wizard.H>

const Ne_MethodTable *Ne;

void generic_callback(Fl_Widget *w, void *t)
{
    Ne_ParameterList *params = Ne->parameterlist_alloc(1);
    Ne->cell_set_pointer(Ne->parameterlist_set_cell(params, 0), w);
    Ne->exec_callback(static_cast<Ne_Cell *>(t), params, NULL);
    Ne->parameterlist_free(params);
}

void modify_callback(int pos, int inserted, int deleted, int restyled, const char *deletedText, void *cbArg)
{
    Ne_ParameterList *params = Ne->parameterlist_alloc(5);
    Ne->cell_set_number_int(Ne->parameterlist_set_cell(params, 0), pos);
    Ne->cell_set_number_int(Ne->parameterlist_set_cell(params, 1), inserted);
    Ne->cell_set_number_int(Ne->parameterlist_set_cell(params, 2), deleted);
    Ne->cell_set_number_int(Ne->parameterlist_set_cell(params, 3), restyled);
    Ne->cell_set_string(Ne->parameterlist_set_cell(params, 4), deletedText != NULL ? deletedText : "");
    Ne->exec_callback(static_cast<Ne_Cell *>(cbArg), params, NULL);
    Ne->parameterlist_free(params);
}

extern "C" {

Ne_EXPORT int Ne_INIT(const Ne_MethodTable *methodtable)
{
    Ne = methodtable;
    return Ne_SUCCESS;
}

Ne_CONST_INT(Ne_ShortcutAlt, FL_ALT)
Ne_CONST_INT(Ne_ShortcutCommand, FL_COMMAND)
Ne_CONST_INT(Ne_ShortcutShift, FL_SHIFT)

Ne_CONST_INT(Ne_MenuDivider, FL_MENU_DIVIDER)
Ne_CONST_INT(Ne_MenuSubmenu, FL_SUBMENU)

Ne_CONST_INT(Ne_FontCourier, FL_COURIER)

Ne_CONST_INT(Ne_AlignLeft, FL_ALIGN_LEFT)

Ne_FUNC(Ne_alert)
{
    const std::string message = Ne_PARAM_STRING(0);

    fl_alert("%s", message.c_str());
    return Ne_SUCCESS;
}

Ne_FUNC(Ne_choice)
{
    const std::string prompt = Ne_PARAM_STRING(0);
    const std::string b0 = Ne_PARAM_STRING(1);
    const std::string b1 = Ne_PARAM_STRING(2);
    const std::string b2 = Ne_PARAM_STRING(3);

    int r = fl_choice(
        "%s",
        b0.empty() ? NULL : b0.c_str(),
        b1.empty() ? NULL : b1.c_str(),
        b2.empty() ? NULL : b2.c_str(),
        prompt.c_str()
    );
    Ne_RETURN_INT(r);
}

Ne_FUNC(Ne_fileChooser)
{
    const std::string message = Ne_PARAM_STRING(0);
    const std::string pattern = Ne_PARAM_STRING(1);
    const std::string defname = Ne_PARAM_STRING(2);

    Fl_Native_File_Chooser fnfc;
    fnfc.title(message.c_str());
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (fnfc.show() != 0) {
        Ne_RETURN_STRING("");
    }
    Ne_RETURN_STRING(fnfc.filename());
}

Ne_FUNC(Ne_getInput)
{
    const std::string prompt = Ne_PARAM_STRING(0);
    const std::string defstr = Ne_PARAM_STRING(1);

    const char *r = fl_input("%s", defstr.c_str(), prompt.c_str());
    if (r == NULL) {
        Ne_RETURN_STRING("");
    }
    Ne_RETURN_STRING(r);
}

Ne_FUNC(Ne_makeWindow)
{
    int w = Ne_PARAM_INT(0);
    int h = Ne_PARAM_INT(1);

    Ne_RETURN_POINTER(new Fl_Window(w, h));
}

Ne_FUNC(Ne_run)
{
    Fl::run();
    return Ne_SUCCESS;
}

Ne_FUNC(Ne_MenuBar_copy)
{
    Fl_Menu_Bar *m = Ne_PARAM_POINTER(Fl_Menu_Bar, 0);
    const Ne_Cell *items = Ne_IN_PARAM(1);

    Fl_Menu_Item *flitems = new Fl_Menu_Item[Ne->cell_get_array_size(items) + 1];
    memset(flitems, 0, sizeof(Fl_Menu_Item) * (Ne->cell_get_array_size(items) + 1));
    for (int i = 0; i < Ne->cell_get_array_size(items); i++) {
        const Ne_Cell *item = Ne->cell_get_array_cell(items, i);
        flitems[i].text = Ne->cell_get_string(Ne->cell_get_array_cell(item, 0));
        if (flitems[i].text[0] == 0) {
            flitems[i].text = NULL;
        }
        flitems[i].shortcut_ = Ne->cell_get_number_int(Ne->cell_get_array_cell(item, 1));
        flitems[i].callback_ = generic_callback;
        Ne_Cell *thunk = Ne->cell_alloc();
        Ne->cell_copy(thunk, Ne->cell_get_array_cell(item, 2));
        flitems[i].user_data_ = thunk;
        flitems[i].flags = Ne->cell_get_number_int(Ne->cell_get_array_cell(item, 3));
    }
    m->copy(flitems);
    delete[] flitems;
    return Ne_SUCCESS;
}

Ne_FUNC(Ne_TextBuffer_addModifyCallback)
{
    Fl_Text_Buffer *b = Ne_PARAM_POINTER(Fl_Text_Buffer, 0);
    const Ne_Cell *cb = Ne_IN_PARAM(1);

    // TODO: This allocates a new cell but it is never freed.
    // Might need to have an intermediate class in the middle
    // to hold the callback thunk.
    Ne_Cell *thunk = Ne->cell_alloc();
    Ne->cell_copy(thunk, cb);
    b->add_modify_callback(modify_callback, thunk);
    return Ne_SUCCESS;
}

Ne_FUNC(Ne_TextBuffer_searchForward)
{
    Fl_Text_Buffer *b = Ne_PARAM_POINTER(Fl_Text_Buffer, 0);
    int startPos = Ne_PARAM_INT(1);
    const std::string searchString = Ne_PARAM_STRING(2);

    int foundPos;
    if (not b->search_forward(startPos, searchString.c_str(), &foundPos)) {
        Ne_RETURN_BOOL(false);
    }
    Ne->cell_set_number_int(Ne_OUT_PARAM(0), foundPos);
    Ne_RETURN_BOOL(true);
}

#include "fltk.cpp.inc"

} // extern "C"
