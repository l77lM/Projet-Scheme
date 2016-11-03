#include <stdio.h>
#include <string.h>

#include "primitive.h"


object eval_arg(object A)
{
    /* Evalue la liste des arguments d'une primitive */
    printf("eval_arg");
    /*object ptr=A;
    object val=nil;
    while (ptr != nil)
    {
        val=sfs_eval(car(ptr));
        ptr->this.pair.car=val;
    }*/
    return sfs_eval(A);
}

object save_primitive( char* nom, object (*ptrPrim)(object) )
{
    /* Ajoute la primitive et le symbole associé dans l'environnement */
    object prim=make_primitive(ptrPrim);
    object symb=make_symbol(nom);
    object A=add_symb(meta_env,symb,prim);

    sfs_print_env(meta_env);    /*DEbug */
    return A;
}

void init_primitives(void)
{
    save_primitive("+",prim_plus);
}


object prim_plus( object Liste )
{
    printf("Plus ");  /* Debugage */
    object res=make_integer(0);
    object test=Liste;
    while (test!=nil)
    {
        res=add_num(res,car(test));
        test=cdr(test);
    }
    return res;
}


object add_num(object A,object B)
{
    printf("add_num "); /* Debugage */
    object res=nil;
    uint ta=A->this.number.numtype;
    uint tb=B->this.number.numtype;

    if (   ( ( ta == NUM_INTEGER) || (ta==NUM_UINTEGER)   )  &&   ((tb==NUM_INTEGER) || (tb==NUM_UINTEGER) ) )
    {
        /* Si A et B entiers */
        res=make_integer( (A->this.number.this.integer) + (B->this.number.this.integer) );
    }

    else if    ( (ta==NUM_REAL )  &&   ((tb==NUM_INTEGER) || (tb==NUM_UINTEGER) ) )
    {
        /* Si l'un des deux est entier et l'autre reel */
        res=make_real( (A->this.number.this.real) + (B->this.number.this.integer) );
    }

    else if    ( (tb==NUM_REAL )  &&   ((ta==NUM_INTEGER) || (ta==NUM_UINTEGER) ) )
    {
        /* Si l'un des deux est entier et l'autre reel */
        res=make_real( (B->this.number.this.real) + (A->this.number.this.integer) );
    }

    else if   ( ( ta==NUM_REAL )  &&   (tb==NUM_REAL) )
    {
        /* Si les deux sont reels */
        res=make_real( (A->this.number.this.real) + (B->this.number.this.real) );
    }

    else if (  (ta==NUM_UNDEF )  ||  (tb==NUM_UNDEF )   ||    ((ta==NUM_PINFTY) && (tb==NUM_MINFTY))  ||  ((tb==NUM_PINFTY) && (ta==NUM_MINFTY))  )
    {
        /* Si l'un des deux est une forme indeterminée ou que l'un est +inf et l'autre -inf   */
        res=make_undef();
    }

    else if (    (ta==NUM_PINFTY )   ||   (tb==NUM_PINFTY )  )
    {
        /* Si l'un des deux est +inf (l'autre est fini sinon on est sorti avant) */
        res=make_infty(0);
    }

    else if (    (ta==NUM_PINFTY )   ||   (tb==NUM_PINFTY )  )
    {
        /* Si l'un des deux est -inf (l'autre est fini sinon on est sorti avant) */
        res=make_infty(-1);
    }


    else
    {
        WARNING_MSG("Addition impossible");
        res=nil;
    }

    return res;

}





object sous_num(object A,object B)
{
    object res=nil;
    if (   ( (A->this.number.numtype==NUM_INTEGER) || (A->this.number.numtype==NUM_UINTEGER)   )  &&   ((B->this.number.numtype==NUM_INTEGER) || (B->this.number.numtype==NUM_UINTEGER) ) )
    {
        res=make_integer( (A->this.number.this.integer) - (B->this.number.this.integer) );
        return res;
    }

    else
    {
        WARNING_MSG("Soustraction impossible");
        return nil;
    }

}

object mult_num(object A,object B)
{
    object res=nil;
    if (   ( (A->this.number.numtype==NUM_INTEGER) || (A->this.number.numtype==NUM_UINTEGER)   )  &&   ((B->this.number.numtype==NUM_INTEGER) || (B->this.number.numtype==NUM_UINTEGER) ) )
    {
        res=make_integer( (A->this.number.this.integer) * (B->this.number.this.integer) );
        return res;
    }

    else
    {
        WARNING_MSG("Multiplication impossible");
        return nil;
    }

}

object div_num(object A,object B)
{
    object res=nil;
    if (   ( (A->this.number.numtype==NUM_INTEGER) || (A->this.number.numtype==NUM_UINTEGER)   )  &&   ((B->this.number.numtype==NUM_INTEGER) || (B->this.number.numtype==NUM_UINTEGER) ) )
    {
        res=make_integer( (A->this.number.this.integer) / (B->this.number.this.integer) );
        return res;
    }

    else
    {
        WARNING_MSG("Division impossible");
        return nil;
    }

}
