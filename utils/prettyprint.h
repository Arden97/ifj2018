///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of printing token function	                         //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef IFJ18_PP_H
#define IFJ18_PP_H

#include "../scanner/scanner.h"
#include <stdio.h>

void token_prettyprint(ifj18_token_t *token);

#endif
