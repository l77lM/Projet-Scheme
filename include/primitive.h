#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "object.h"

object eval_arg(object A);

object save_primitive( char* nom, object (*ptrPrim)(object) );
void init_primitives(void);



object prim_plus( object Liste );

object add_num(object A,object B);      /* Fonctions liées aux opérations de nombres  */
object sous_num(object A,object B);
object mult_num(object A,object B);
object div_num(object A,object B);



#ifdef __cplusplus
}
#endif

#endif /* _PRIMITIVE_H_ */
