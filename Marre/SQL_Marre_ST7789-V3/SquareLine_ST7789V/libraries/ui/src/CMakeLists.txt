SET(SOURCES ui_Screen1.c
    ui_Screen2.c
    ui_Screen3.c
    ui_Screen4.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_logo_png.c
    ui_img_941217166.c
    ui_img_140512013.c
    ui_img_354864566.c
    ui_img_273796629.c
    ui_img_273797654.c
    ui_img_273803043.c
    ui_img_273795620.c
    fonts/ui_font_SOYOMapleBold12.c
    fonts/ui_font_SOYOMapleBold14.c
    fonts/ui_font_SOYOMapleBold18.c)

add_library(ui ${SOURCES})
