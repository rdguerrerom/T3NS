#pragma once

#include "instructions.h"

void QC_fetch_pUpdate(struct instructionset * const instructions, 
                         const int bond, const int is_left);

void QC_fetch_bUpdate(struct instructionset * const instructions, 
                         const int bond, const int is_left);

void QC_fetch_merge(struct instructionset * const instructions, 
                       const int bond);
