//
// Created by Luca Giovannesi.
//

#ifndef RIOT_GREENHOUSE_LOGIC_CONDITION_H
#define RIOT_GREENHOUSE_LOGIC_CONDITION_H

#include "stdlib.h"

typedef enum {
    LESS,
    GREATER,
    EQUAL,
    NOT_EQUAL
} operator_e;


typedef struct logic_condition_s {
    int enabled;
    const int *operand_a;
    const int *operand_b;
    operator_e operator;
    int last_evaluation;

    const int *parameter;

    void (*action)(int *);

    const void *to_check;
} logic_condition_t;

const logic_condition_t *
logic_condition_add(const int *operand_a, operator_e operator, const int *operand_b, void(*action)(int *),
                    const int *parameter,
                    const logic_condition_t *to_check);

int _logic_condition_evaluate_operand(const int *operand_a,const int *operand_b, operator_e operator);

int _logic_condition_evaluate_condition(const logic_condition_t *condition);

void logic_condition_update(void);

#endif //RIOT_GREENHOUSE_LOGIC_CONDITION_H
