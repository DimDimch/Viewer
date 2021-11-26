#pragma once

#include "components.h"

#define MAX_SCROLL_RANGE 65534


void InitScroll(HWND hwnd, scroll_info_t* scroll);

void UpdateScrollShift(model_info_t* model, scroll_info_t* scroll, unsigned long symbolNum);

void FindScrollShift(model_info_t* model, scroll_info_t* scroll);
void FindScrollPos(model_info_t* model, scroll_info_t* scroll);
