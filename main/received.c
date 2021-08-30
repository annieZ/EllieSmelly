/*
 * AWS IoT EduKit - Core2 for AWS IoT EduKit
 * EllieMeter v2.1.0
 * received.c
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
#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

#include "core2forAWS.h"
#include "global.h"
#include "received.h"

static void start_over_event_handler(lv_obj_t* slider, lv_event_t event);

static const char* TAG = SAMPLE_RECEIVED_TAB_NAME;

lv_obj_t* received_tab;
lv_obj_t* tabview;

void display_received_tab(lv_obj_t* tv, lv_obj_t* core2forAWS_screen_obj){
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
    tabview = tv;
    received_tab = lv_tabview_add_tab(tv, SAMPLE_RECEIVED_TAB_NAME);  // Create a tab

    /* Create the main body object and set background within the tab*/
    static lv_style_t bg_style;
    lv_obj_t* received_bg = lv_obj_create(received_tab, NULL);
    lv_obj_align(received_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 16, 36);
    lv_obj_set_size(received_bg, 290, 190);
    lv_obj_set_click(received_bg, false);
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(received_bg, LV_OBJ_PART_MAIN, &bg_style);
 
 /* Create the sensor information label object */
    lv_obj_t* body_label = lv_label_create(received_bg, NULL);
    lv_label_set_long_mode(body_label, LV_LABEL_LONG_BREAK);
    lv_label_set_static_text(body_label, "Sample received!\n\n I have now # of samples of the same kind. \nThe more I have the better matches I can make");
    lv_obj_set_width(body_label, 252);
    lv_obj_align(body_label, received_bg, LV_ALIGN_IN_TOP_LEFT, 0, 10);

    static lv_style_t body_style;
    lv_style_init(&body_style);
    lv_style_set_text_color(&body_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_add_style(body_label, LV_OBJ_PART_MAIN, &body_style);

    /* Create the sensor information label object */
    static lv_style_t btn_style;
    lv_style_init(&btn_style);
    lv_style_set_pad_top(&btn_style, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_bottom(&btn_style, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_left(&btn_style, LV_STATE_DEFAULT, 12);
    lv_style_set_pad_right(&btn_style, LV_STATE_DEFAULT, 12);

    lv_obj_t* startOver_btn = lv_btn_create(received_bg, NULL);
    lv_obj_set_size(startOver_btn, 100, 38);
    lv_obj_set_event_cb(startOver_btn, start_over_event_handler);
    lv_obj_align(startOver_btn, received_bg, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -14);
    lv_btn_set_state(startOver_btn, LV_BTN_STATE_CHECKED_PRESSED);

    lv_obj_t* startOver_label = lv_label_create(startOver_btn, NULL);
    lv_label_set_static_text(startOver_label, "Start Over");

    xSemaphoreGive(xGuiSemaphore);

   // xTaskCreatePinnedToCore(received_task, "receivedTask", configMINIMAL_STACK_SIZE * 2, (void*) core2forAWS_screen_obj, 0, &received_handle, 1);
}

static void start_over_event_handler(lv_obj_t* obj, lv_event_t event){
    
    ESP_LOGI(TAG, "Star over selected");
    // Home screen has index of 0, hardcoded :(  because it was first added in main
    lv_tabview_set_tab_act(tabview, 0, LV_ANIM_OFF); 
}




void received_task(void* pvParameters){
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
    lv_obj_t* battery_label = lv_label_create((lv_obj_t*)pvParameters, NULL);
    lv_label_set_text(battery_label, LV_SYMBOL_BATTERY_FULL);
    lv_label_set_recolor(battery_label, true);
    lv_label_set_align(battery_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(battery_label, (lv_obj_t*)pvParameters, LV_ALIGN_IN_TOP_RIGHT, -20, 10);
    lv_obj_t* charge_label = lv_label_create(battery_label, NULL);
    lv_label_set_recolor(charge_label, true);
    lv_label_set_text(charge_label, "");
    lv_obj_align(charge_label, battery_label, LV_ALIGN_CENTER, -4, 0);
    xSemaphoreGive(xGuiSemaphore);



    vTaskDelete(NULL); // Should never get to here...
}