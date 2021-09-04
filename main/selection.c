/*
 * AWS IoT EduKit - Core2 for AWS IoT EduKit
 * EllieMeter v2.1.0
 * selecton.c
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
#include "selection.h"

static void identify_event_handler(lv_obj_t* obj, lv_event_t event);
static void tell_me_event_handler(lv_obj_t* obj, lv_event_t event);
static void start_over_event_handler(lv_obj_t* slider, lv_event_t event);

static const char* TAG = SELECTION_TAB_NAME;

lv_obj_t* identified_tab;
lv_obj_t* tabview;

void display_user_selection_tab(lv_obj_t* tv, lv_obj_t* core2forAWS_screen_obj){
    xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);
    tabview = tv;
    identified_tab = lv_tabview_add_tab(tv, SELECTION_TAB_NAME);  // Create a tab

    /* Create the main body object and set background within the tab*/
    static lv_style_t bg_style;
    lv_obj_t* power_bg = lv_obj_create(identified_tab, NULL);
    lv_obj_align(power_bg, NULL, LV_ALIGN_IN_TOP_LEFT, 16, 36);
    lv_obj_set_size(power_bg, 290, 190);
    lv_obj_set_click(power_bg, false);
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(power_bg, LV_OBJ_PART_MAIN, &bg_style);

     /* Create the sensor information label object */
    lv_obj_t* body_label = lv_label_create(power_bg, NULL);
    lv_label_set_long_mode(body_label, LV_LABEL_LONG_BREAK);
    lv_label_set_static_text(body_label, "Great news! I have enough sample for me to try to identify or you to help me learn.\n\n Tap to select your preference:");
    lv_obj_set_width(body_label, 252);
    lv_obj_align(body_label, power_bg, LV_ALIGN_IN_TOP_MID, 0, 10);

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

    lv_obj_t* pwr_led_btn = lv_btn_create(power_bg, NULL);
    lv_obj_set_event_cb(pwr_led_btn, identify_event_handler);
    lv_obj_set_size(pwr_led_btn, 76, 38);
    lv_obj_align(pwr_led_btn, power_bg, LV_ALIGN_IN_BOTTOM_LEFT, 20, -14);
    lv_btn_set_checkable(pwr_led_btn, true);
    lv_btn_toggle(pwr_led_btn);
    // lv_btn_set_layout(pwr_led_btn, LV_LAYOUT_OFF);
    // lv_btn_set_fit(pwr_led_btn, LV_FIT_NONE);
    lv_btn_set_state(pwr_led_btn, LV_BTN_STATE_RELEASED);
    // lv_obj_add_style(pwr_led_btn, LV_OBJ_PART_MAIN, &btn_style);

    lv_obj_t* identify_label = lv_label_create(pwr_led_btn, NULL);
    lv_label_set_static_text(identify_label, "Identify");

    lv_obj_t* youTellMe_btn = lv_btn_create(power_bg, pwr_led_btn);
    lv_obj_set_event_cb(youTellMe_btn, tell_me_event_handler);
    lv_obj_align(youTellMe_btn, power_bg, LV_ALIGN_IN_BOTTOM_MID, 0, -14);

    lv_obj_t* youTellMe_label = lv_label_create(youTellMe_btn, NULL);
    lv_label_set_static_text(youTellMe_label, "Tell Me");

    lv_obj_t* startOver_btn = lv_btn_create(power_bg, pwr_led_btn);
    lv_obj_set_event_cb(startOver_btn, start_over_event_handler);
    lv_obj_align(startOver_btn, power_bg, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -14);
    lv_btn_set_state(startOver_btn, LV_BTN_STATE_CHECKED_PRESSED);

    lv_obj_t* startOver_label = lv_label_create(startOver_btn, NULL);
    lv_label_set_static_text(startOver_label, "StartOver");

    xSemaphoreGive(xGuiSemaphore);

  //  xTaskCreatePinnedToCore(selection_task, "selectionTask", configMINIMAL_STACK_SIZE * 2, (void*) core2forAWS_screen_obj, 0, &selection_handle, 1);
}

static void start_over_event_handler(lv_obj_t* obj, lv_event_t event){     
    ESP_LOGI(TAG, "Star over selected");
    // Home screen has index of 0, hardcoded :(  because it was first added in main
    lv_tabview_set_tab_act(tabview, 0, LV_ANIM_OFF); 
}

static void identify_event_handler(lv_obj_t* obj, lv_event_t event){
      ESP_LOGI(TAG, "Identify over selected");
    // send sample to cloud

    // wait for result
    // Call identify window screen has index of 3, hardcoded :(  because how it was added in main
    lv_tabview_set_tab_act(tabview, 3, LV_ANIM_OFF);
}

static void tell_me_event_handler(lv_obj_t* obj, lv_event_t event){
     ESP_LOGI(TAG, "Tell me selected");
    // display keyboard
    // Call tell me window screen has index of 2, hardcoded :(  because how it was added in main
    lv_tabview_set_tab_act(tabview, 2, LV_ANIM_OFF);
}
