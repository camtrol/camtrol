SET(SOURCES ui_Screen1.c
    ui_Screen2.c
    ui_Screen3.c
    ui_Screen4.c
    ui_Screen5.c
    ui_Screen6.c
    ui_ScreenQR.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_logo_png.c
    ui_img_1296478115.c
    fonts/ui_font_SOYOMapleBold12.c
    fonts/ui_font_SOYOMapleBold14.c
    fonts/ui_font_SOYOMapleBold18.c)

add_library(ui ${SOURCES})
