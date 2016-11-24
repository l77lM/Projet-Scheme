#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "objectprim.h"
#include "eval.h"
#include "object.h"
#include "read.h"




object eval_arg(object A);

object save_primitive( char* nom, object (*ptrPrim)(object) );
void init_primitives(void);



object prim_plus( object Liste );       /* Manip Nombre */
object prim_moins( object Liste);
object prim_mult( object Liste );
object prim_div( object Liste );
object prim_quotient(object Liste);
object prim_remainder(object Liste);

object prim_egalite( object Liste );

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

object prim_egalite( object Liste );
object prim_sup( object Liste );
object prim_inf( object Liste );

object prim_conv_intchar(object A);
object prim_conv_charint(object Liste);
object prim_conv_strnum(object Liste);
object prim_conv_numstr(object Liste);
object prim_conv_symbstr(object Liste);
object prim_conv_strsymb(object Liste);

object prim_cons(object Liste);
object prim_car(object Liste);
object prim_cdr(object Liste);
object prim_setcar(object Liste);
object prim_setcdr(object Liste);
object prim_list(object Liste);

object prim_eq(object Liste);
object prim_abs(object A);
object prim_round (object A);





#ifdef __cplusplus
}
#endif

#endif /* _PRIMITIVE_H_ */
