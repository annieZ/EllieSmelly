/*
 * AWS IoT EduKit - Core2 for AWS IoT EduKit
 * EllieMeter v2.1.0
 * home.c
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
#include "home.h"


static const char* TAG = HOME_TAB_NAME;
static void start_smell_event_handler(lv_obj_t* slider, lv_event_t event);

void display_home_tab(lv_obj_t* tv){
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);   // Takes (blocks) the xGuiSemaphore mutex from being read/written by another task.
    
    lv_obj_t* home_tab = lv_tabview_add_tab(tv, HOME_TAB_NAME);   // Create a tab

    /* Create the title within the tab */
    static lv_style_t title_style;
    lv_style_init(&title_style);
    lv_style_set_text_font(&title_style, LV_STATE_DEFAULT, LV_THEME_DEFAULT_FONT_TITLE);
    lv_style_set_text_color(&title_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    
    lv_obj_t* tab_title_label = lv_label_create(home_tab, NULL);
    lv_obj_add_style(tab_title_label, LV_OBJ_PART_MAIN, &title_style);
    lv_label_set_static_text(tab_title_label, "Ellie Smelly");
    lv_label_set_align(tab_title_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(tab_title_label, home_tab, LV_ALIGN_IN_TOP_MID, 0, 50);

    lv_obj_t* body_label = lv_label_create(home_tab, NULL);
    lv_label_set_long_mode(body_label, LV_LABEL_LONG_BREAK);
    lv_label_set_static_text(body_label, "I can help identify a smell or you can help me build a collection of smells for other to use as  a reference. \n\n Tap to start smelling");
    lv_obj_set_width(body_label, 280);
    lv_obj_align(body_label, home_tab, LV_ALIGN_CENTER, 0 , 10);

     /* Create the sensor information label object */
    static lv_style_t btn_style;
    lv_style_init(&btn_style);
    lv_style_set_pad_top(&btn_style, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_bottom(&btn_style, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_left(&btn_style, LV_STATE_DEFAULT, 12);
    lv_style_set_pad_right(&btn_style, LV_STATE_DEFAULT, 12);

    lv_obj_t* startOver_btn = lv_btn_create(home_tab, NULL);
    lv_obj_set_size(startOver_btn, 100, 38);
    lv_obj_set_event_cb(startOver_btn, start_smell_event_handler);
    lv_obj_align(startOver_btn, home_tab, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -14);
    lv_btn_set_state(startOver_btn, LV_BTN_STATE_CHECKED_PRESSED);

    lv_obj_t* startSmelling_label = lv_label_create(startOver_btn, NULL);
    lv_label_set_static_text(startSmelling_label, "Start");
    

    xSemaphoreGive(xGuiSemaphore);
    
    ESP_LOGI(TAG, "\n\n Ellie smelly start page.\n\n");
}
static void start_smell_event_handler(lv_obj_t* obj, lv_event_t event){
          
    ESP_LOGI(TAG, "Home Screen");
}