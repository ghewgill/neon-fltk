import os
import shutil
import stat
import sys
import tarfile

env = Environment()
if sys.platform == "win32":
    env.Append(CXXFLAGS=["/EHsc", "/MDd", "/W4", "/WX"])
else:
    env.Append(CXXFLAGS=["-std=c++0x"])

libfltk = None

fltkenv = Environment()
if sys.platform == "win32":
    env.Append(CXXFLAGS=[
        "/wd4458", # shadow warning triggered in fltk headers
    ])

if GetOption("clean"):
    shutil.rmtree("fltk-1.3.4-1")
elif not os.path.exists("fltk-1.3.4-1/configure"):
    tarfile.open("fltk-1.3.4-1-source.tar.gz").extractall(".")
    if sys.platform == "win32":
        open("fltk-1.3.4-1/Fl/abi-version.h", "w").write("#define FL_ABI_VERSION 10304\n")

env.Command(["fltk.neon.inc", "fltk.cpp.inc"], ["mkapi.py", "fltk.json"], sys.executable + " $SOURCES " + Dir(".").abspath)

env.Command("fltk.neon", ["fltk.neon.in", "fltk.neon.inc"], lambda source, target, env: open(target[0].abspath, "w").write(open(source[0].abspath).read() + open(source[1].abspath).read()))

if sys.platform == "win32":
    fltkenv.Append(CPPPATH=["fltk-1.3.4-1"])
    fltkenv.Append(CPPPATH=["fltk-1.3.4-1/ide/VisualC2010"])
    fltkenv.Append(CPPDEFINES=["FL_LIBRARY", "WIN32"])
    fltkenv.Append(CFLAGS=["/MDd"])
    fltkenv.Append(CXXFLAGS=["/MDd"])
    libfltk = fltkenv.Library("fltk-1.3.4-1/lib/fltk.lib", [
        "fltk-1.3.4-1/src/filename_absolute.cxx",
        "fltk-1.3.4-1/src/filename_expand.cxx",
        "fltk-1.3.4-1/src/filename_isdir.cxx",
        "fltk-1.3.4-1/src/Fl.cxx",
        "fltk-1.3.4-1/src/Fl_abort.cxx",
        "fltk-1.3.4-1/src/Fl_Adjuster.cxx",
        "fltk-1.3.4-1/src/fl_arc.cxx",
        "fltk-1.3.4-1/src/fl_arci.cxx",
        "fltk-1.3.4-1/src/fl_arg.cxx",
        "fltk-1.3.4-1/src/fl_ask.cxx",
        "fltk-1.3.4-1/src/Fl_Bitmap.cxx",
        "fltk-1.3.4-1/src/Fl_Box.cxx",
        "fltk-1.3.4-1/src/fl_boxtype.cxx",
        "fltk-1.3.4-1/src/Fl_Browser.cxx",
        "fltk-1.3.4-1/src/Fl_Browser_.cxx",
        "fltk-1.3.4-1/src/Fl_Browser_load.cxx",
        "fltk-1.3.4-1/src/Fl_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Chart.cxx",
        "fltk-1.3.4-1/src/Fl_Check_Browser.cxx",
        "fltk-1.3.4-1/src/Fl_Check_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Choice.cxx",
        "fltk-1.3.4-1/src/Fl_Clock.cxx",
        "fltk-1.3.4-1/src/Fl_color.cxx",
        "fltk-1.3.4-1/src/Fl_Color_Chooser.cxx",
        "fltk-1.3.4-1/src/Fl_compose.cxx",
        "fltk-1.3.4-1/src/Fl_Copy_Surface.cxx",
        "fltk-1.3.4-1/src/Fl_Counter.cxx",
        "fltk-1.3.4-1/src/fl_cursor.cxx",
        "fltk-1.3.4-1/src/fl_curve.cxx",
        "fltk-1.3.4-1/src/Fl_Device.cxx",
        "fltk-1.3.4-1/src/Fl_Dial.cxx",
        "fltk-1.3.4-1/src/Fl_dnd.cxx",
        "fltk-1.3.4-1/src/Fl_Double_Window.cxx",
        "fltk-1.3.4-1/src/fl_draw.cxx",
        "fltk-1.3.4-1/src/fl_draw_image.cxx",
        "fltk-1.3.4-1/src/fl_draw_pixmap.cxx",
        "fltk-1.3.4-1/src/Fl_File_Browser.cxx",
        "fltk-1.3.4-1/src/Fl_File_Chooser.cxx",
        "fltk-1.3.4-1/src/Fl_File_Chooser2.cxx",
        "fltk-1.3.4-1/src/fl_file_dir.cxx",
        "fltk-1.3.4-1/src/Fl_File_Icon.cxx",
        "fltk-1.3.4-1/src/Fl_File_Input.cxx",
        "fltk-1.3.4-1/src/fl_font.cxx",
        "fltk-1.3.4-1/src/Fl_get_system_colors.cxx",
        "fltk-1.3.4-1/src/fl_gleam.cxx",
        "fltk-1.3.4-1/src/Fl_grab.cxx",
        "fltk-1.3.4-1/src/Fl_Group.cxx",
        "fltk-1.3.4-1/src/fl_gtk.cxx",
        "fltk-1.3.4-1/src/Fl_Image.cxx",
        "fltk-1.3.4-1/src/Fl_Image_Surface.cxx",
        "fltk-1.3.4-1/src/Fl_Input.cxx",
        "fltk-1.3.4-1/src/Fl_Input_.cxx",
        "fltk-1.3.4-1/src/Fl_labeltype.cxx",
        "fltk-1.3.4-1/src/fl_line_style.cxx",
        "fltk-1.3.4-1/src/Fl_lock.cxx",
        "fltk-1.3.4-1/src/Fl_Light_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Menu.cxx",
        "fltk-1.3.4-1/src/Fl_Menu_.cxx",
        "fltk-1.3.4-1/src/Fl_Menu_add.cxx",
        "fltk-1.3.4-1/src/Fl_Menu_Bar.cxx",
        "fltk-1.3.4-1/src/Fl_Menu_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Menu_Window.cxx",
        "fltk-1.3.4-1/src/Fl_Native_File_Chooser.cxx",
        "fltk-1.3.4-1/src/fl_oval_box.cxx",
        "fltk-1.3.4-1/src/Fl_Overlay_Window.cxx",
        "fltk-1.3.4-1/src/Fl_Pack.cxx",
        "fltk-1.3.4-1/src/Fl_Paged_Device.cxx",
        "fltk-1.3.4-1/src/Fl_Pixmap.cxx",
        "fltk-1.3.4-1/src/fl_plastic.cxx",
        "fltk-1.3.4-1/src/Fl_Positioner.cxx",
        "fltk-1.3.4-1/src/Fl_Preferences.cxx",
        "fltk-1.3.4-1/src/Fl_Progress.cxx",
        "fltk-1.3.4-1/src/fl_read_image.cxx",
        "fltk-1.3.4-1/src/fl_rect.cxx",
        "fltk-1.3.4-1/src/Fl_Repeat_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Return_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Roller.cxx",
        "fltk-1.3.4-1/src/fl_round_box.cxx",
        "fltk-1.3.4-1/src/Fl_Round_Button.cxx",
        "fltk-1.3.4-1/src/Fl_Scroll.cxx",
        "fltk-1.3.4-1/src/fl_scroll_area.cxx",
        "fltk-1.3.4-1/src/Fl_Scrollbar.cxx",
        "fltk-1.3.4-1/src/Fl_Shared_Image.cxx",
        "fltk-1.3.4-1/src/fl_shortcut.cxx",
        "fltk-1.3.4-1/src/Fl_Single_Window.cxx",
        "fltk-1.3.4-1/src/Fl_Slider.cxx",
        "fltk-1.3.4-1/src/fl_symbols.cxx",
        "fltk-1.3.4-1/src/Fl_Tabs.cxx",
        "fltk-1.3.4-1/src/Fl_Text_Buffer.cxx",
        "fltk-1.3.4-1/src/Fl_Text_Display.cxx",
        "fltk-1.3.4-1/src/Fl_Text_Editor.cxx",
        "fltk-1.3.4-1/src/Fl_Tile.cxx",
        "fltk-1.3.4-1/src/Fl_Tiled_Image.cxx",
        "fltk-1.3.4-1/src/Fl_Tooltip.cxx",
        "fltk-1.3.4-1/src/Fl_utf.c",
        "fltk-1.3.4-1/src/Fl_utf8.cxx",
        "fltk-1.3.4-1/src/Fl_Valuator.cxx",
        "fltk-1.3.4-1/src/Fl_Value_Input.cxx",
        "fltk-1.3.4-1/src/Fl_Value_Slider.cxx",
        "fltk-1.3.4-1/src/fl_vertex.cxx",
        "fltk-1.3.4-1/src/Fl_Widget.cxx",
        "fltk-1.3.4-1/src/Fl_Window.cxx",
        "fltk-1.3.4-1/src/Fl_Window_fullscreen.cxx",
        "fltk-1.3.4-1/src/Fl_Window_hotspot.cxx",
        "fltk-1.3.4-1/src/Fl_Window_shape.cxx",
        "fltk-1.3.4-1/src/Fl_Wizard.cxx",
        "fltk-1.3.4-1/src/Fl_XBM_Image.cxx",
        "fltk-1.3.4-1/src/Fl_XPM_Image.cxx",
        "fltk-1.3.4-1/src/filename_list.cxx",
        "fltk-1.3.4-1/src/filename_match.cxx",
        "fltk-1.3.4-1/src/flstring.c",
        "fltk-1.3.4-1/src/numericsort.c",
        "fltk-1.3.4-1/src/scandir.c",
        "fltk-1.3.4-1/src/screen_xywh.cxx",
        "fltk-1.3.4-1/src/vsnprintf.c",
        "fltk-1.3.4-1/src/xutf8/case.c",
        "fltk-1.3.4-1/src/xutf8/is_spacing.c",
    ])
    env.Append(CPPPATH=["fltk-1.3.4-1"])
    env.Append(CPPDEFINES=["WIN32"])
else:
    libfltk = fltkenv.Command("fltk-1.3.4-1/lib/libfltk.a", "fltk-1.3.4-1/configure", "cd lib/fltk/fltk-1.3.4-1 && env CFLAGS=-fPIC CXXFLAGS=-fPIC ./configure && make")
    env.Append(CPPPATH=["fltk-1.3.4-1"])

env.Append(CPPPATH=["../../src"])
env.Append(LIBS=[libfltk])
if sys.platform == "win32":
    env.Append(LIBS=["advapi32", "comdlg32", "gdi32", "ole32", "shell32", "user32"])
elif sys.platform.startswith("linux"):
    env.Append(LIBS=["X11"])
elif sys.platform == "darwin":
    env.Append(LINKFLAGS=[
        "-framework", "AppKit",
        "-framework", "CoreFoundation",
        "-framework", "CoreGraphics",
        "-framework", "CoreText",
    ])
env.SharedLibrary("neon_fltk", "fltk.cpp")
