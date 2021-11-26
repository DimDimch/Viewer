#pragma once

#include "components.h"


void InitModel(model_info_t* model);
void FillModel(model_info_t* model, char* fileName, error_t* err);

void UpdateModel(model_info_t* model, unsigned int maxInLine, error_t* err);

