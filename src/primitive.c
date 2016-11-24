#include <stdio.h>
#include <string.h>

#include "primitive.h"


void init_primitives(void)
{
    save_primitive("+",prim_plus);
    save_primitive("-",prim_moins);
    save_primitive("*",prim_mult);
    save_primitive("/",prim_div);
    save_primitive("quotient",prim_quotient);
    save_primitive("remainder",prim_remainder);
    save_primitive("=",prim_egalite);
    save_primitive(">",prim_sup);
    save_primitive("<",prim_inf);

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

    save_primitive("integer->char",prim_conv_intchar);
    save_primitive("char->integer",prim_conv_charint);
    save_primitive("string->number",prim_conv_strnum);
    save_primitive("number->string",prim_conv_numstr);
    save_primitive("symbol->string",prim_conv_symbstr);
    save_primitive("string->symbol",prim_conv_strsymb);


    save_primitive("cons",prim_cons);
    save_primitive("car",prim_car);
    save_primitive("cdr",prim_cdr);
    save_primitive("set-car!",prim_setcar);
    save_primitive("set-cdr!",prim_setcdr);
    save_primitive("list",prim_list);

    save_primitive("eq?",prim_eq);
    save_primitive("abs",prim_abs);
    save_primitive("round",prim_round);
}



object eval_arg(object A)
{
    /* Evalue la liste des arguments d'une primitive */

    if ( A->type==SFS_NIL )
        return nil;

    else
    {
        object output=make_pair();
        output->this.pair.car=sfs_eval(car(A));
        output->this.pair.cdr=eval_arg(cdr(A));
        return output;
    }

    /*
        if ( A->type==SFS_PAIR && ( (car(A))->type != SFS_SYMBOL) )
        {
            output=make_pair();
            output->this.pair.car=eval_arg(car(A));
            output->this.pair.cdr=eval_arg(cdr(A));
        }
        else
        {
            output=sfs_eval(A);
        }

        return output;
    */
}


object save_primitive( char* nom, object (*ptrPrim)(object) )
{
    /* Ajoute la primitive et le symbole associé dans l'environnement */
    object prim=make_primitive(ptrPrim);
    object symb=make_symbol(nom);
    object A=add_symb(meta_env,symb,prim);

    /*sfs_print_env(meta_env);*/    /*DEbug */
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


object prim_egalite( object Liste )
{
    if (cdr(Liste)==nil)
    {
        WARNING_MSG("Pas assez d'elements");
        return Error;
    }
    object test=cdr(Liste);
    int egal=0;

    object A=nil;
    object B=car(Liste);

    while (test!=nil)
    {
        A=B;
        B=car(test);

        if (A==Error || B==Error)
        {
            return Error;
        }

        egal=egalite_num(A,B);

        if (egal==-1)
        {
            return false;
        }

        test=cdr(test);
    }
    return true;
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

object prim_pair(object  A)
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

object prim_quotient(object Liste)
{
    if (prim_integer(Liste)==false)
    {
        WARNING_MSG("Entrez un entier plutot c'est mieux");
        return Error;
    }
    object A=car(Liste);

    /*if(A->this.number.numtype!=NUM_INTEGER)                             ---Methode 2 ---
        {
            WARNING_MSG("Entrez un entier plutot c'est mieux");
            return nil;

        }*/

    object test=cdr(Liste);
    if (test==nil)
    {
        WARNING_MSG("Pas assez d'élément");
        return Error;

    }

    object B=car(test);

    int res=(A->this.number.this.integer)/(B->this.number.this.integer);

    return( make_integer(res) );
}

object prim_remainder(object Liste)
{
    if (prim_integer(Liste)==false)
    {
        WARNING_MSG("Entrez un entier plutot c'est mieux");
        return Error;
    }
    object A=car(Liste);

    object test=cdr(Liste);
    if (test==nil)
    {
        WARNING_MSG("Pas assez d'élément");
        return Error;
    }
    object B=car(test);

    if (A==Error || B==Error)
    {
        return Error;
    }

    int res=(A->this.number.this.integer)%(B->this.number.this.integer);

    return( make_integer(res) );
}

object prim_inf( object Liste )
{
    if (cdr(Liste)==nil)
    {
        WARNING_MSG("Pas assez d'elements");
        return Error;
    }
    object test=cdr(Liste);
    int egal=0;

    object A=nil;
    object B=car(Liste);

    while (test!=nil)
    {
        A=B;
        B=car(test);
        egal=compare_num(A,B);

        if (egal==1)
        {
            return false;
        }

        if (egal==0)
        {
            WARNING_MSG("Comparaison impossible");
            return Error;
        }

        test=cdr(test);
    }
    return true;
}

object prim_sup( object Liste )
{
    if (cdr(Liste)==nil)
    {
        WARNING_MSG("Pas assez d'elements");
        return Error;
    }
    object test=cdr(Liste);
    int egal=0;

    object A=nil;
    object B=car(Liste);

    while (test!=nil)
    {
        A=B;
        B=car(test);
        egal=compare_num(A,B);

        if (egal==-1)
        {
            return false;
        }

        if (egal==0)
        {
            WARNING_MSG("Comparaison impossible");
            return Error;
        }

        test=cdr(test);
    }
    return true;
}


object prim_conv_intchar(object Liste)
{
    DEBUG_MSG("int->char");
    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for integer->char");
        return Error;
    }

    object A=car(Liste);
    char a;

    if  (A->type == SFS_NUMBER)
    {
        if ( (A->this.number.numtype == NUM_INTEGER)  || (A->this.number.numtype == NUM_UINTEGER))
        {
            a=(A->this.number.this.integer);
            if ( (a>-1) && (a<256) )
            {
                return (make_character(a));
            }
            else
            {
                WARNING_MSG("Character unknown");
                return Error;
            }
        }
    }
    WARNING_MSG("Argument of integer->char is not a number");
    return Error;
}


object prim_conv_charint(object Liste)
{
    DEBUG_MSG("char->integer");
    if (cdr(Liste)!=nil)
    {
        DEBUG_MSG("Trop d'arguments");
        WARNING_MSG("Too many arguments for charactere->integer");
        return Error;
    }

    object A=car(Liste);
    char a;

    if  (A->type == SFS_CHARACTER)
    {
        DEBUG_MSG("type=character");
        a=A->this.character;
        return (make_integer(a));
    }

    else
    {
        WARNING_MSG("Charactere->integer impossible");
        return Error;
    }


}

object prim_conv_symbstr(object Liste)
{
    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for symbol->string");
        return Error;
    }
    object A=car(Liste);

    if  (A->type == SFS_SYMBOL)
    {
        return make_string(A->this.symbol);
    }
    WARNING_MSG("symbol->string impossible");
    return Error;
}

object prim_conv_strsymb(object Liste)
{
    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for string->symbol");
        return Error;
    }
    object A=car(Liste);
    if  (A->type == SFS_STRING)
    {
        return make_symbol(A->this.string);
    }
    WARNING_MSG("string->symbol impossible");
    return Error;
}

object prim_conv_strnum(object Liste)
{
    DEBUG_MSG("string->number");
    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for string->number");
        return Error;
    }
    object A=car(Liste);
    object res=nil;

    if  (A->type == SFS_STRING)
    {
        DEBUG_MSG("string->number argument is a string");

        uint ind=0;

        res=sfs_read_atom(A->this.string,&ind);

        if (res->type == SFS_NUMBER)
        {
            return res;
        }
        else
        {
            WARNING_MSG("Argument of string->number is not a number");
            return Error;
        }
    }
    else
    {
        WARNING_MSG("string->number impossible");
        return Error;
    }
}

object prim_conv_numstr(object Liste)
{
    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for number->string");
        return Error;
    }
    object A=car(Liste);
    if  (A->type == SFS_NUMBER)
    {
        int ta=A->this.number.numtype;
        float num;

        if ( ta == NUM_INTEGER || ta == NUM_UINTEGER )
        {
            num=A->this.number.this.integer ;
        }

        else if ( ta == NUM_REAL )
        {
            num=A->this.number.this.real ;
        }

        else if ( ta == NUM_PINFTY )
        {
            return make_string("PINFTY");
        }

        else if ( ta == NUM_MINFTY )
        {
            return make_string("+inf");
        }

        else if ( ta == NUM_UNDEF )
        {
            return make_string("-inf");
        }

        char* str;
        sprintf(str,"%g", num);
        return make_string(str);

    }
    else
    {
        WARNING_MSG("number->string impossible");
        return Error;
    }
}

object prim_cons(object Liste)
{
    DEBUG_MSG("CONS");
    if (car(Liste)==nil)
    {
        WARNING_MSG("2 Arguments missing for CONS");
        return Error;
    }
    if (cdr(Liste)==nil)
    {
        WARNING_MSG("1 Argument missing for CONS");
        return Error;
    }
    object res=make_pair();
    res->this.pair.car=car(Liste);
    res->this.pair.cdr=car(cdr(Liste));
    if ( cdr(cdr(Liste)) != nil )
    {
        WARNING_MSG("Too many arguments for cons");
        return Error;
    }
    return res;
}

object prim_car(object Liste)
{
    return car(car(Liste));
}

object prim_cdr(object Liste)
{
    return cdr(car(Liste));
}

object prim_setcar(object Liste)
{
    DEBUG_MSG("Set-car!");
    if (Liste->type != SFS_PAIR)
    {
        WARNING_MSG("Bad argument of set-car!");
        return Error;
    }

    if ( (car(Liste)->type) != SFS_PAIR )
    {
        WARNING_MSG("non-pair argument to set-car!");
        return Error;
    }

    sfs_print_atom(car(Liste));

    if ( (cdr(Liste)->type) != SFS_PAIR )
    {
        WARNING_MSG("Immutable argument to set-car!");
        return Error;
    }

    (Liste->this.pair.car)->this.pair.car = car(cdr(Liste));
    return car(Liste);
}

object prim_setcdr(object Liste)
{
    DEBUG_MSG("Set_cdr!");
    if ( (car(Liste)->type) != SFS_PAIR )
    {
        WARNING_MSG("non-pair argument to set-car!");
        return Error;
    }
    if ( (cdr(Liste)->type) != SFS_PAIR )
    {
        WARNING_MSG("Immutable argument to set-car!");
        return Error;
    }

    (Liste->this.pair.car)->this.pair.cdr = car(cdr(Liste));
    return car(Liste);
}

object prim_list(object Liste)
{
    return Liste;
}

object prim_eq(object Liste)
{
    DEBUG_MSG("EQ?");
    if (cdr(Liste)==nil)
    {
        WARNING_MSG("Not enough arguments");
        return Error;
    }

    object test=cdr(Liste);

    object A=nil;
    object B=car(Liste);
    int ta;
    int tb;
    int egal;

    while (test!=nil)
    {
        A=B;
        B=car(test);
        ta=A->type;
        tb=B->type;

        if (ta!=tb)
        {
            egal=-1;
        }

        else if (ta==SFS_NUMBER)
        {
            egal=egalite_num(A,B);
        }

        else if ( (ta==SFS_CHARACTER) || (ta==SFS_STRING) || (ta==SFS_SYMBOL) )
        {
            egal=compare_char(A,B);
        }

        else if (  (ta==SFS_NIL) || ( (A==true) && (B==true) ) || ( (A==false) && (B==false) ) )
        {
            egal=0;
        }

        else if ( ta==SFS_PRIMITIVE)
        {
            if (A->this.primitive.function == B->this.primitive.function)
            {
                egal=0;
            }
            else
            {
                egal=-1;
            }
        }

        else
        {
            egal=-1;
        }


        if (egal!=0)
        {
            return false;
        }

        test=cdr(test);
    }
    return true;
}

object prim_abs(object Liste)
{
    DEBUG_MSG("ABS");
    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for number->string");
        return Error;
    }
    object A=car(Liste);

    if ( A->type != SFS_NUMBER )
    {
        return Error;
    }


    object res;

    int ta=A->this.number.numtype;

    if (ta==NUM_INTEGER || ta==NUM_UINTEGER)
    {
        int x=(A->this.number.this.integer);
        if (x<0)
        {
            res=make_integer(-x);
        }
        else
        {
            res=A;
        }

    }

    else if (ta==NUM_REAL)
    {
        float x=(A->this.number.this.real);
        if (x<0)
        {
            res=make_real(-x);
        }
        else
        {
            res=A;
        }
    }

    else if (ta==NUM_MINFTY   ||  ta==NUM_PINFTY  || ta==NUM_UNDEF )
    {
        res=make_undef();
    }

    else
    {
        WARNING_MSG("ABS impossible");
        return Error;
    }
    return res;
}


object prim_round (object Liste)
{
    DEBUG_MSG("Round");

    if (cdr(Liste)!=nil)
    {
        WARNING_MSG("Too many arguments for round");
    }

    object A=car(Liste);

    if (A->type != SFS_NUMBER)
    {
        WARNING_MSG("Argument of round is not a number");
        return NULL;
    }
    num B=A->this.number;
    uint ta=B.numtype;

    if (ta==NUM_REAL)
    {
        int a=B.this.real;
        return (make_integer(a));
    }
    else
    {
        return A;
    }
}







