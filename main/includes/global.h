#pragma once
#
#define HOME_TAB_NAME "HOME"
#define IDENTIFIED_TAB_NAME "SAMPLE_IDENITIFIED"
#define KEYBOARD_TAB_NAME "USER_KEYBOARD"
#define SAMPLE_RECEIVED_TAB_NAME "SAMPLE_RECEIVED"
#define SELECTION_TAB_NAME "SELECTION_TAB"

//  User input 
const char * userInputStr;

// Sensor input
const  char * tvoc;
const  char * eCO2;

// Received label
lv_obj_t* received_label;
lv_obj_t* received_bg;