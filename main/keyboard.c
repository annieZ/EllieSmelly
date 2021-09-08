/*
 * AWS IoT EduKit - Core2 for AWS IoT EduKit
 * EllieMeter v2.1.0
 * keyoard.c
 * 
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

#include "core2forAWS.h"
#include "global.h"
#include "keyboard.h"
#include "received.h"
#include "core_http_config.h"

lv_obj_t* tabview;
lv_obj_t* keyboard_tab;
static lv_obj_t * kb;
static lv_obj_t * ta;
static const char* TAG = KEYBOARD_TAB_NAME;

void display_keyboard_tab(lv_obj_t* tv, lv_obj_t* core2forAWS_screen_obj){
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
    tabview = tv;
    keyboard_tab = lv_tabview_add_tab(tv, KEYBOARD_TAB_NAME);  // Create a tab

    /* Create the main body object and set background within the tab*/
    static lv_style_t bg_style;
    lv_obj_t* keyb_bg = lv_obj_create(keyboard_tab, NULL);
    lv_obj_align(keyb_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 16, 36);
    lv_obj_set_size(keyb_bg, 290, 190);
    lv_obj_set_click(keyb_bg, false);
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(keyb_bg, LV_OBJ_PART_MAIN, &bg_style);

    /* Create the title within the main body object */
    static lv_style_t title_style;
    lv_style_init(&title_style);
    lv_style_set_text_font(&title_style, LV_STATE_DEFAULT, LV_THEME_DEFAULT_FONT_TITLE);
    lv_style_set_text_color(&title_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_t* tab_title_label = lv_label_create(keyb_bg, NULL);
    lv_obj_add_style(tab_title_label, LV_OBJ_PART_MAIN, &title_style);
    lv_label_set_static_text(tab_title_label, "");
    lv_obj_align(tab_title_label, keyb_bg, LV_ALIGN_IN_TOP_MID, 0, 10);

    /* Add Keyoard */

 /*Create a keyboard to use it with an of the text areas*/
       /*Create a text area. The keyboard will write here*/
    ta  = lv_textarea_create(keyboard_tab, NULL);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, 20);
    lv_obj_align(ta, tab_title_label, LV_ALIGN_CENTER, 0 , 10);
    lv_obj_set_event_cb(ta, ta_event_cb);
    lv_textarea_set_text(ta, "");
   
    kb_create();

  
    xSemaphoreGive(xGuiSemaphore);

 //   xTaskCreatePinnedToCore(keyboard_task, "keyboardTask", configMINIMAL_STACK_SIZE * 2, (void*) core2forAWS_screen_obj, 0, &keyboard_handle, 1);
}



static void kb_event_cb(lv_obj_t * keyboard, lv_event_t e)
{
    lv_keyboard_def_event_cb(kb, e);
    if(e == LV_EVENT_CANCEL) {
        lv_textarea_set_text(ta, "");
        // start over
        lv_tabview_set_tab_act(tabview, 0, LV_ANIM_OFF);
    }
    if(e == LV_EVENT_APPLY) {
        // Call back end server and then display received  
        userInputStr = lv_textarea_get_text(ta);        
        lv_textarea_set_text(ta, "");
        if(userInputStr != NULL){           
            ESP_LOGI(TAG, "\n\n Read %s: ", userInputStr); 
            lv_tabview_set_tab_act(tabview, 4, LV_ANIM_OFF);  
         }
    }
}

static void kb_create(void)
{
    userInputStr=NULL;
    kb = lv_keyboard_create(keyboard_tab, NULL);
    lv_keyboard_set_cursor_manage(kb, true);
    lv_obj_set_event_cb(kb, kb_event_cb);
    lv_keyboard_set_textarea(kb, ta);

}

static void ta_event_cb(lv_obj_t * ta_local, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED && kb == NULL) {
        kb_create();
    }
}