#include "model.h"


/**
 * @brief ���������� ����� ��������� �������� ����������
 *
 * @param model - ���������, ���������� ���������� � ������
 *
 * @return void
 */
void InitModel(model_info_t* model) {
    model->data = NULL;
    model->numSymbols = 0;
    model->numLines = 0;
    model->maxLength = 0;
    model->shift = NULL;
}


/**
 * @brief ������ ������ �� �����
 *
 * @param model - ���������, ���������� ���������� � ������
 * @param maxInLine - ������������ ���-�� �������� � ������
 * @param err - ���������� ��� ������������� ������ ����
 *
 * @return void
 */
void ReadText(model_info_t* model, char* fileName, error_t* err) {
    FILE *fp = fopen(fileName, "rb");
    if (fp != NULL) {
        fseek(fp, 0L, SEEK_END);
        model->numSymbols = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        model->data = (char*)malloc(sizeof(char) * (model->numSymbols + 1));
        if (model->data != NULL) {
            fread(model->data, sizeof(char), model->numSymbols, fp);
            fclose(fp);
            model->data[model->numSymbols] = '\0';
            return;
        }
        else {
            *err = ERROR_MEMORY;
            return;
        }
    }
    else {
        *err = ERROR_FILE;
    }
}


/**
 * @brief ���������� ������� �������� �����
 *
 * @param model - ���������, ���������� ���������� � ������
 * @param maxInLine - ������������ ���-�� �������� � ������
 * @param err - ���������� ��� ������������� ������ ����
 *
 * @return void
 */
void FillShifts(model_info_t* model, unsigned int maxInLine, error_t* err) {
    unsigned int shiftSize = REALLOC_STEP;

    model->shift = (unsigned long*)malloc(sizeof(unsigned long) * REALLOC_STEP);
    if(model->shift != NULL) {
        model->shift[0] = 0;
        model->maxLength = 0;
        unsigned int strNum = 1;
        unsigned int countLength = 0;

        for(unsigned long i = 0; i <= model->numSymbols; i++) {
            countLength++;
            if ((i - model->shift[strNum - 1] >= maxInLine - 1) || model->data[i] == '\n' || model->data[i] == '\0') {
                if (countLength > model->maxLength){
                    model->maxLength = countLength;
                }
                if (strNum == shiftSize - 1) {
                    unsigned long* temp = (unsigned long*)realloc(model->shift, (REALLOC_STEP + shiftSize) * sizeof(unsigned long));
                    if(temp == NULL) {
                        *err = ERROR_MEMORY;
                        free(model->shift);
                        return;
                    }
                    else {
                        model->shift = temp;
                        shiftSize += REALLOC_STEP;
                    }
                }

                if (model->data[i] != '\n' && model->data[i] != '\0') {
                    while (model->data[i] != ' ' && i > model->shift[strNum - 1]) {
                        i--;
                    }
                }

                model->shift[strNum] = i + 1;
                countLength = 0;
                strNum++;
            }
        }
        model->numLines = strNum - 1;
    }
    else {
        *err = ERROR_MEMORY;
    }
}


/**
 * @brief ���������� ����� ������ ��� ��������� ��������� �����������
 *
 * @param model - ���������, ���������� ���������� � ������
 * @param maxInLine - ������������ ���-�� �������� � ������
 * @param err - ���������� ��� ������������� ������ ����
 *
 * @return void
 */
void UpdateModel(model_info_t* model, unsigned int maxInLine, error_t* err) {
    if (model->data != NULL) {
        if(model->shift != NULL){
            free(model->shift);
            model->shift = NULL;
        }
        FillShifts(model, maxInLine, err);
    }
}


/**
 * @brief ���������� ����� ��������� � �������� ����������� � ������ (����� �� ��������� - ��� �������)
 *
 * @param model - ���������, ���������� ���������� � ������
 * @param fileName - ������, ���������� ���������� ���� � �����
 * @param err - ���������� ��� ������������� ������ ����
 *
 * @return void
 */
void FillModel(model_info_t* model, char* fileName, error_t* err) {
    if (model->data != NULL) {
        free(model->data);
    }
    if (model->shift != NULL) {
        free(model->shift);
    }
    InitModel(model);

    //������ ����� � ��������� ���������� �����
    ReadText(model, fileName, err);
    if (*err != ERROR_NO) {
        return;
    }

    //����������� �������� ����� ��� ������ ��� �������
    FillShifts(model, UINT_MAX, err);
    if (*err != ERROR_NO) {
        free(model->data);
        return;
    }
}

