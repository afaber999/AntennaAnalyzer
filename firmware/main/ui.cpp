#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ui.h"

static void draw_menu_button(uint16_t y,const char* label, int selected);


void draw_menu(void)
{
    gfx_lcd.init();          // Initialiseer het scherm
    gfx_lcd.setRotation(1);  // Draai het scherm (0-3)
    gfx_lcd.fillScreen(TFT_BLACK); // Maak het scherm zwart

    // Teken tekst
    gfx_lcd.setTextColor(TFT_LIGHTGREY);
    gfx_lcd.setTextSize(2);
    gfx_lcd.setCursor(10, 10);
    gfx_lcd.print("Starting!");

    // // Teken vormen
    gfx_lcd.drawRect(50, 50, 100, 60, TFT_YELLOW);
    gfx_lcd.fillCircle(160, 120, 30, TFT_BLUE);

    draw_menu_button(50, "OPEN", 0);
    draw_menu_button(90, "CLOSE", 1);
}

#define RGB565(r,g,b)  ( (((g)&0x1c)<<11) | (((b)&0xf8)<<5) | ((r)&0xf8) | (((g)&0xe0)>>5) )
#define RGBHEX(hex) ( (((hex)&0x001c00)<<3) | (((hex)&0x0000f8)<<5) | (((hex)&0xf80000)>>16) | (((hex)&0x00e000)>>13) )

#define FONT_GET_HEIGHT (16)
#define ILI9341_WIDTH (320)

#define MENU_BUTTON_WIDTH  60
#define MENU_BUTTON_HEIGHT 30
#define MENU_BUTTON_MAX 8
#define NUM_INPUT_HEIGHT   30

#define DEFAULT_FG_COLOR            RGB565(255,255,255)
#define DEFAULT_BG_COLOR            RGB565(  0,  0,  0)
#define DEFAULT_GRID_COLOR          RGB565(128,128,128)
#define DEFAULT_MENU_COLOR          RGB565(255,255,255)
#define DEFAULT_MENU_TEXT_COLOR     RGB565(  0,  0,  0)
#define DEFAULT_MENU_ACTIVE_COLOR   RGB565(180,255,180)
#define DEFAULT_TRACE_1_COLOR       RGB565(255,255,  0)
#define DEFAULT_TRACE_2_COLOR       RGB565(  0,255,255)
#define DEFAULT_TRACE_3_COLOR       RGB565(  0,255,  0)
#define DEFAULT_TRACE_4_COLOR       RGB565(255,  0,255)
#define DEFAULT_NORMAL_BAT_COLOR    RGB565( 31,227,  0)
#define DEFAULT_LOW_BAT_COLOR       RGB565(255,  0,  0)
#define DEFAULT_SPEC_INPUT_COLOR    RGB565(128,255,128);

// #define S_DELTA "\004"
// #define S_DEGREE "\037"
// #define S_SARROW "\030"
// #define S_INFINITY "\031"
// #define S_LARROW "\032"
// #define S_RARROW "\033"
// #define S_PI    "\034"
// #define S_MICRO "\035"
// #define S_OHM   "\036"


static uint8_t menu_current_level = 0;
static int8_t  selection = 0;

// static void draw_menu_button(uint16_t y,const char* label, int selected);


// type of menu item 
enum {
  MT_NONE,
  MT_BLANK,
  MT_SUBMENU,
  MT_CALLBACK,
  MT_CANCEL,
  MT_CLOSE
};


typedef void (*menuaction_cb_t)(int item, uint8_t data);

// Set structure align as WORD (save flash memory)
#pragma pack(push, 2)
typedef struct {
  uint8_t type;
  uint8_t data;
  char *label;
  const void *reference;
} menuitem_t;
#pragma pack(pop)


static void menu_caldone_cb(int item, uint8_t data)
{
  //extern const menuitem_t menu_save[];
  //extern const menuitem_t menu_cal[];
  (void)item;
  (void)data;
//   cal_done();
//   draw_cal_status();
//   menu_move_back();
//   menu_push_submenu(menu_save);
}


static void menu_calop_cb(int item, uint8_t data)
{
  //cal_collect(data);
  selection = item+1;
  //draw_cal_status();
  //draw_menu();
}


#define CAL_LOAD 0
#define CAL_OPEN 1
#define CAL_SHORT 2
#define CAL_THRU 3
#define CAL_ISOLN 4


// static const menuitem_t menu_calop[] = {
//   { MT_CALLBACK, CAL_OPEN,  "OPEN",  menu_calop_cb },
//   { MT_CALLBACK, CAL_SHORT, "SHORT", menu_calop_cb },
//   { MT_CALLBACK, CAL_LOAD,  "LOAD",  menu_calop_cb },
//   { MT_CALLBACK, CAL_ISOLN, "ISOLN", menu_calop_cb },
//   { MT_CALLBACK, CAL_THRU,  "THRU",  menu_calop_cb },
//   { MT_CALLBACK, 0,         "DONE",  menu_caldone_cb },
// //  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
//   { MT_NONE,     0, NULL, NULL } // sentinel
// };


// #define MENU_STACK_DEPTH_MAX 4
// const menuitem_t *menu_stack[MENU_STACK_DEPTH_MAX] = {
//   menu_calop, NULL, NULL, NULL
// };
// //   .menu_normal_color = DEFAULT_MENU_COLOR,
// //   .menu_active_color = DEFAULT_MENU_ACTIVE_COLOR,


static void draw_menu_button(uint16_t y,const char* label, int selected)
{
    uint16_t bg = (selected==0) ? DEFAULT_MENU_COLOR : DEFAULT_MENU_ACTIVE_COLOR;
    uint16_t fg = DEFAULT_MENU_TEXT_COLOR;

    // draw button background
    gfx_lcd.fillRect(ILI9341_WIDTH-MENU_BUTTON_WIDTH, y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT-2, bg);
    // ili9341_set_foreground(fg);
    // ili9341_set_background(bg);

    // TODO
    bg = DEFAULT_TRACE_3_COLOR;

    if (label[0] != '\2') {
        gfx_lcd.fillRect(320-MENU_BUTTON_WIDTH+3, y+8, MENU_BUTTON_WIDTH-6, 2+FONT_GET_HEIGHT+2, bg);
        gfx_lcd.setTextSize(2);        
        gfx_lcd.setTextColor(fg, bg);
        gfx_lcd.setCursor(320-MENU_BUTTON_WIDTH+5, y+10);
        gfx_lcd.print(label);
    //     ili9341_drawstring(label, 320-MENU_BUTTON_WIDTH+5, y+10);
    // } else {
        // const char* l1 = &label[1];
        // const char* l2 = l1 + strlen(l1) + 1;
        // gfx_lcd.fillRect(320-MENU_BUTTON_WIDTH+3, y+5, MENU_BUTTON_WIDTH-6, 2+FONT_GET_HEIGHT+1+FONT_GET_HEIGHT+2, bg);
        // ili9341_drawstring(l1, 320-MENU_BUTTON_WIDTH+5, y+7);
        // ili9341_drawstring(l2, 320-MENU_BUTTON_WIDTH+5, y+7+FONT_GET_HEIGHT+1);
    }
}

static void draw_menu_buttons(const menuitem_t *menu)
{
//   int i = 0;
//   for (i = 0; i < MENU_BUTTON_MAX; i++) {
//     const char *l1, *l2;
//     if (menu[i].type == MT_NONE)
//       break;
//     if (menu[i].type == MT_BLANK)
//       continue;
//     uint16_t y = MENU_BUTTON_HEIGHT*i;
 
//     draw_menu_button(y, menu[i].label, 0);
 
//     //uint16_t bg = config.menu_normal_color;
//     //uint16_t fg = DEFAULT_MENU_TEXT_COLOR;
//     // // focus only in MENU mode but not in KEYPAD mode
//     // if (ui_mode == UI_MENU && i == selection)
//     //   bg = config.menu_active_color;
//     // gfx_lcd.fillRect(320-MENU_BUTTON_WIDTH, y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT-2, bg);
    
    // menu_item_modify_attribute(menu, i, &fg, &bg);
//     // ili9341_set_foreground(fg);
//     // ili9341_set_background(bg);
//     // if (menu_is_multiline(menu[i].label, &l1, &l2)) {
//     //   gfx_lcd.fillRect(320-MENU_BUTTON_WIDTH+3, y+5, MENU_BUTTON_WIDTH-6, 2+FONT_GET_HEIGHT+1+FONT_GET_HEIGHT+2, bg);
    //   ili9341_drawstring(l1, 320-MENU_BUTTON_WIDTH+5, y+7);
    //   ili9341_drawstring(l2, 320-MENU_BUTTON_WIDTH+5, y+7+FONT_GET_HEIGHT+1);
//     // } else {
//     //   gfx_lcd.fillRect(320-MENU_BUTTON_WIDTH+3, y+8, MENU_BUTTON_WIDTH-6, 2+FONT_GET_HEIGHT+2, bg);
    //   ili9341_drawstring(menu[i].label, 320-MENU_BUTTON_WIDTH+5, y+10);
    // }
//     }
}

// void draw_menu(void)
// {
//   draw_menu_buttons(menu_stack[menu_current_level]);
// }

// static void
// erase_menu_buttons(void)
// {
//   gfx_lcd.fillRect(320-MENU_BUTTON_WIDTH, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT * MENU_BUTTON_MAX, DEFAULT_BG_COLOR);
//}

// // Key names
// #define KP_0          0
// #define KP_1          1
// #define KP_2          2
// #define KP_3          3
// #define KP_4          4
// #define KP_5          5
// #define KP_6          6
// #define KP_7          7
// #define KP_8          8
// #define KP_9          9
// #define KP_PERIOD    10
// #define KP_MINUS     11
// #define KP_X1        12
// #define KP_K         13
// #define KP_M         14
// #define KP_G         15
// #define KP_BS        16
// #define KP_INF       17
// #define KP_DB        18
// #define KP_PLUSMINUS 19
// #define KP_KEYPAD    20
// #define KP_N         21
// #define KP_P         22

// typedef struct {
//   uint8_t x:4;
//   uint8_t y:4;
//   int8_t  c;
// } keypads_t;

// static const keypads_t *keypads = NULL;

// #define KP_WIDTH     48
// #define KP_HEIGHT    48
// // Key x, y position (0 - 15) on screen
// #define KP_GET_X(posx) ((posx)*KP_WIDTH + (320-64-KP_WIDTH*4))
// #define KP_GET_Y(posy) ((posy)*KP_HEIGHT + 12 )



// static const keypads_t keypads_freq[] = {
//   { 1, 3, KP_PERIOD },
//   { 0, 3, KP_0 },
//   { 0, 2, KP_1 },
//   { 1, 2, KP_2 },
//   { 2, 2, KP_3 },
//   { 0, 1, KP_4 },
//   { 1, 1, KP_5 },
//   { 2, 1, KP_6 },
//   { 0, 0, KP_7 },
//   { 1, 0, KP_8 },
//   { 2, 0, KP_9 },
//   { 3, 0, KP_G },
//   { 3, 1, KP_M },
//   { 3, 2, KP_K },
//   { 3, 3, KP_X1 },
//   { 2, 3, KP_BS },
//   { 0, 0, -1 }
// };


// void draw_keypad(void)
// {
//     keypads = keypads_freq;
//     int i = 0;


//   while (keypads[i].c >= 0) {
//     uint16_t bg = (i == selection) ? DEFAULT_MENU_COLOR : DEFAULT_MENU_ACTIVE_COLOR;
//     uint16_t fg = DEFAULT_MENU_TEXT_COLOR;

//     ili9341_set_foreground(DEFAULT_MENU_TEXT_COLOR);
//     ili9341_set_background(bg);
//     int x = KP_GET_X(keypads[i].x);
//     int y = KP_GET_Y(keypads[i].y);
//     gfx_lcd.fillRect(x+2, y+2, KP_WIDTH-4, KP_HEIGHT-4, bg);
//     ili9341_drawfont(keypads[i].c,
//                      x + (KP_WIDTH - NUM_FONT_GET_WIDTH) / 2,
// //                      y + (KP_HEIGHT - NUM_FONT_GET_HEIGHT) / 2);
//     i++;
//   }
// }
