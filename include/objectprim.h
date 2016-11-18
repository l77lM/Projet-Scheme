#ifndef _OBJECTPRIM_H_
#define _OBJECTPRIM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "object.h"



object add_num(object A,object B);      /* Fonctions liées aux opérations de nombres  */
object oppose_num(object A);
object mult_num(object A,object B);
object inverse_num(object A);

object prim_predicat(object Liste,uint type);
object prim_predicat_number(object Liste,uint type);

int egalite_num(object A, object B);
int compare_num(object A, object B);

int compare_char(object A ,object B);





#ifdef __cplusplus
}
#endif

#endif /* _OBJECTPRIM_H_ */
