#include <stdio.h>
#include <string.h>

#include "primitive.h"
#include "object.h"

void init_primitives(void)
{
    save_primitive("+",prim_plus);
    save_primitive("-",prim_moins);
    save_primitive("*",prim_mult);
    save_primitive("/",prim_div);

    save_primitive("null?",prim_null);
    save_primitive("boolean?",prim_boolean);
    save_primitive("symbol?",prim_symbol);
    save_primitive("char?",prim_char);
    save_primitive("string?",prim_string);
    save_primitive("pair?",prim_pair);
    save_primitive("integer?",prim_integer);
    save_primitive("real?",prim_real);
    save_primitive("pinfty?",prim_pinfty);
    save_primitive("minfty?",prim_minfty);
}



object eval_arg(object A)
{
    /* Evalue la liste des arguments d'une primitive */
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




object prim_plus( object Liste )
{
    object res=make_integer(0);
    object test=Liste;
    while (test!=nil)
    {
        res=add_num(res,car(test));
        test=cdr(test);
    }
    return res;
}


object prim_moins( object Liste)
{
    object res=nil;
    object test=cdr(Liste);

    if ( test==nil )
    {
        res=oppose_num(car(Liste));
    }
    else
    {
        res=car(Liste);
        while (test!=nil)
        {
            res=add_num(res,oppose_num(car(test)));
            test=cdr(test);
        }
    }
    return res;
}


object prim_mult( object Liste )
{
    object res=nil;
    object test=cdr(Liste);

    if ( test==nil )
    {
        res=car(Liste);
    }
    else
    {
        res=car(Liste);
        while (test!=nil)
        {
            res=mult_num(res,car(test));
            test=cdr(test);
        }
    }
    return res;
}

object prim_div( object Liste )
{
    object res=nil;
    object test=cdr(Liste);

    object I=nil;

    if ( test==nil )
    {
        res=inverse_num(car(Liste));
    }
    else
    {
        res=car(Liste);
        while (test!=nil)
        {
            I=inverse_num(car(test));
            sfs_print_atom(res);
            sfs_print_atom(I);
            res=mult_num(res,I);
            test=cdr(test);
        }
    }
    return res;
}

object prim_null(object A)
{
    return prim_predicat(A,SFS_NIL);
}

object prim_boolean(object A)
{
    return prim_predicat(A,SFS_BOOLEAN);
}

object prim_symbol(object A)
{
    return prim_predicat(A,SFS_SYMBOL);
}

object prim_char(object A)
{
    return prim_predicat(A,SFS_CHARACTER);
}

object prim_string(object A)
{
    return prim_predicat(A,SFS_STRING);
}

object prim_pair(object A)
{
    return prim_predicat(A,SFS_PAIR);
}


object prim_integer(object A)
{
    return prim_predicat_number(A,NUM_INTEGER);
}

object prim_real(object A)
{
    return prim_predicat_number(A,NUM_REAL);
}

object prim_pinfty(object A)
{
    return prim_predicat_number(A,NUM_PINFTY);
}

object prim_minfty(object A)
{
    return prim_predicat_number(A,NUM_MINFTY);
}
