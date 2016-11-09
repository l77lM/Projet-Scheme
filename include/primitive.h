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



object prim_plus( object Liste );       /* Manip Nombre */
object prim_moins( object Liste);
object prim_mult( object Liste );
object prim_div( object Liste );

object prim_null(object A);             /* Predicats */
object prim_boolean(object A);
object prim_symbol(object A);
object prim_char(object A);
object prim_string(object A);
object prim_pair(object A);
object prim_integer(object A);
object prim_real(object A);
object prim_pinfty(object A);
object prim_minfty(object A);




#ifdef __cplusplus
}
#endif

#endif /* _PRIMITIVE_H_ */
