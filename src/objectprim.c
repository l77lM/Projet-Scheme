#include <stdio.h>
#include <string.h>

#include "objectprim.h"
#include "object.h"

object add_num(object A,object B)
{
    DEBUG_MSG("Add_num");
    if (A==Error || B==Error)
    {
        return Error;
    }

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

    else if (    (ta==NUM_MINFTY )   ||   (tb==NUM_MINFTY )   )
    {
        /* Si l'un des deux est -inf (l'autre est fini sinon on est sorti avant) */
        res=make_infty(-1);
    }


    else
    {
        WARNING_MSG("Addition impossible");
        res=Error;
    }

    return res;

}


object oppose_num(object A)
{
    DEBUG_MSG("Oppose_num");
    if (A==Error)
    {
        return Error;
    }
    object res=nil;
    uint ta=A->this.number.numtype;

    if (ta==NUM_INTEGER || ta==NUM_UINTEGER)
    {
        res=make_integer(-(A->this.number.this.integer));
    }

    else if (ta==NUM_REAL)
    {
        res=make_real(-(A->this.number.this.real));
    }

    else if (ta==NUM_MINFTY)
    {
        res=make_infty(0);
    }

    else if (ta==NUM_PINFTY)
    {
        res=make_infty(-1);
    }

    else if (ta==NUM_UNDEF)
    {
        res=A;
    }

    else
    {
        WARNING_MSG("Soustraction impossible");
        res=Error;
    }
    return res;
}


object mult_num(object A,object B)
{
    DEBUG_MSG("Mult_num");
    if (A==Error || B==Error)
    {
        return Error;
    }

    object res=nil;
    uint ta=A->this.number.numtype;
    uint tb=B->this.number.numtype;

    if (   ( ( ta == NUM_INTEGER) || (ta==NUM_UINTEGER)   )  &&   ((tb==NUM_INTEGER) || (tb==NUM_UINTEGER) ) )
    {
        /* Si A et B entiers */
        res=make_integer( (A->this.number.this.integer) * (B->this.number.this.integer) );
    }

    else if    ( (ta==NUM_REAL )  &&   ((tb==NUM_INTEGER) || (tb==NUM_UINTEGER) ) )
    {
        /* Si l'un des deux est entier et l'autre reel */
        res=make_real( (A->this.number.this.real) * (B->this.number.this.integer) );
    }

    else if    ( (tb==NUM_REAL )  &&   ((ta==NUM_INTEGER) || (ta==NUM_UINTEGER) ) )
    {
        /* Si l'un des deux est entier et l'autre reel */
        res=make_real( (B->this.number.this.real) * (A->this.number.this.integer) );
    }

    else if   ( ( ta==NUM_REAL )  &&   (tb==NUM_REAL) )
    {
        /* Si les deux sont reels */
        res=make_real( (A->this.number.this.real) * (B->this.number.this.real) );
    }

    else if (  (ta==NUM_UNDEF )  ||  (tb==NUM_UNDEF )  )
    {
        /* Si l'un des deux est une forme indeterminée   */
        res=make_undef();
    }

    else if (    (ta==NUM_PINFTY && tb==NUM_MINFTY )   ||   (ta==NUM_MINFTY && tb==NUM_PINFTY )  )
    {
        /* Si on a (-inf)*(+inf) */
        res=make_infty(-1);
    }

    else if (  (ta==NUM_MINFTY && tb==NUM_MINFTY )  )
    {
        /* Si on a (-inf)*(-inf) */
        res=make_infty(0);
    }


    else if (    (ta==NUM_MINFTY )   ||   (tb==NUM_MINFTY )   )
    {
        /* Si l'un des deux est -inf (l'autre est fini sinon on est sorti avant) */
        res=make_infty(-1);
    }

    else if (    (ta==NUM_PINFTY )   ||   (tb==NUM_PINFTY )   )
    {
        /* Si l'un des deux est +inf (l'autre est fini ou (+inf) sinon on est sorti avant) */
        res=make_infty(0);
    }


    else
    {
        WARNING_MSG("Multiplication impossible");
        res=Error;
    }

    return res;


}


object inverse_num(object A)
{
    DEBUG_MSG("Inverse_num");
    if (A==Error)
    {
        return Error;
    }
    object res=nil;

    uint ta=A->this.number.numtype;

    if (ta==NUM_INTEGER || ta==NUM_UINTEGER)
    {
        float x=(A->this.number.this.integer);
        if (x==0)
        {
            return make_infty(0);
        }
        res=make_real(1/x);
    }

    else if (ta==NUM_REAL)
    {
        float x=(A->this.number.this.real);
        res=make_real(1/x);
    }

    else if (ta==NUM_MINFTY   ||  ta==NUM_PINFTY  || ta==NUM_UNDEF )
    {
        res=make_undef();
    }

    else
    {
        WARNING_MSG("Divide impossible");
        return Error;
    }
    return res;
}


object prim_predicat(object Liste,uint type)
{
    DEBUG_MSG("Predicat");


    if ( Liste==nil )
    {
        if ( type==SFS_NIL )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    object test=Liste;

    while (test!=nil)
    {
        if (car(test)==Error)
        {
            return Error;
        }

        if (car(test)->type != type)
        {
            return false;
        }
        test=cdr(test);
    }
    return true;

}

object prim_predicat_number(object Liste,uint type)
{
    DEBUG_MSG("Predicat Number");
    if ( Liste==nil )
    {
        return false;
    }

    object test=Liste;

    while (test!=nil)
    {
        if (car(test)==Error)
        {
            return Error;
        }

        if ( car(test)->this.number.numtype != type)
        {
            return false;
        }
        test=cdr(test);
    }
    return true;

}


int egalite_num(object A, object B)
{
    /* Renvoie 0 si egaux, -1 si different et 1 si erreur */
    DEBUG_MSG("Egalite num");

    if (A==Error || B==Error)
    {
        return 1;
    }


    uint ta=A->this.number.numtype;
    uint tb=B->this.number.numtype;

    if ((ta==NUM_UNDEF )  ||  (tb==NUM_UNDEF ))
    {
        return -1;
    }

    else if ( ((ta==NUM_PINFTY) && (tb==NUM_MINFTY))  ||  ((tb==NUM_PINFTY) && (ta==NUM_MINFTY)) )
    {
        return -1;
    }

    else if ( ((ta==NUM_PINFTY) && (tb==NUM_PINFTY))  ||  ((tb==NUM_MINFTY) && (ta==NUM_MINFTY)) )
    {
        return 0;
    }

    else if (  (ta==NUM_PINFTY) || (tb==NUM_MINFTY)  ||  (tb==NUM_PINFTY) || (ta==NUM_MINFTY) )
    {
        return -1;
    }

    float a;
    float b;

    if (  (ta == NUM_INTEGER) || (ta==NUM_UINTEGER)  )
    {
        a=A->this.number.this.integer;
    }

    else if    (ta==NUM_REAL )
    {
        a=A->this.number.this.real;
    }

    if ( (tb == NUM_INTEGER) || (tb==NUM_UINTEGER) )
    {
        b=B->this.number.this.integer;
    }

    else if    (tb==NUM_REAL )
    {
        b=B->this.number.this.real;
    }

    if (a==b)
    {
        return 0;
    }

    else
    {
        return -1;
    }
}

int compare_num(object A, object B)
{
    /* Renvoie 1 si A > B  -1 sinon  et 0 si erreur */
    DEBUG_MSG("Compare num");
    if (A==Error || B==Error)
    {
        return 0;
    }
    uint ta=A->this.number.numtype;
    uint tb=B->this.number.numtype;

    if ((ta==NUM_UNDEF )  ||  (tb==NUM_UNDEF ))
    {
        return 0;
    }

    else if ( ((ta==NUM_PINFTY) && (tb==NUM_PINFTY))  ||  ((tb==NUM_MINFTY) && (ta==NUM_MINFTY)) )
    {
        return 0;
    }

    else if  ((ta==NUM_PINFTY) && (tb==NUM_MINFTY))
    {
        return 1;
    }

    else if  ((ta==NUM_MINFTY) && (tb==NUM_PINFTY))
    {
        return -1;
    }

    else if (  (ta==NUM_PINFTY) || (tb==NUM_MINFTY)  )
    {
        return 1;
    }

    else if (  (tb==NUM_PINFTY) || (ta==NUM_MINFTY) )
    {
        return -1;
    }

    float a;
    float b;

    if (  (ta == NUM_INTEGER) || (ta==NUM_UINTEGER)  )
    {
        a=A->this.number.this.integer;
    }

    else if    (ta==NUM_REAL )
    {
        a=A->this.number.this.real;
    }

    if ( (tb == NUM_INTEGER) || (tb==NUM_UINTEGER) )
    {
        b=B->this.number.this.integer;
    }

    else if    (tb==NUM_REAL )
    {
        b=B->this.number.this.real;
    }

    if (a>b)
    {
        return 1;
    }

    else
    {
        return -1;
    }
}


int compare_char(object A ,object B)
{
    /* Compare les objets de types SYMBOL , STRING et CHARACTERE */
    DEBUG_MSG("Compare char");
    if (A==Error || B==Error)
    {
        return 1;
    }

    int ta=A->type;
    int tb=B->type;
    int res=-1;

    if ( (ta==SFS_SYMBOL) || (tb==SFS_SYMBOL) )
    {
        DEBUG_MSG("Compare symbol");
        res=strcmp(A->this.symbol,B->this.symbol);
    }

    else if ( (ta==SFS_STRING) || (tb==SFS_STRING) )
    {
        DEBUG_MSG("Compare string");
        res=strcmp(A->this.symbol,B->this.symbol);
    }

    else if ( (ta==SFS_CHARACTER) || (tb==SFS_CHARACTER) )
    {
        DEBUG_MSG("Compare character");
        if ( A->this.character == B->this.character )
        {
            res=0;
        }
    }
    return res;
}





