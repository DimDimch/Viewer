#pragma once

#include "components.h"


void PaintText(HDC hdc, model_info_t* model, view_info_t* view, scroll_info_t* scroll, PAINTSTRUCT* ps);

void PaintBackground(HDC hdc, view_info_t* view, PAINTSTRUCT* ps);
