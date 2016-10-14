
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"

object sfs_eval( object input,object env ) {      /* Tentative de prise en compte de l'environnement, Attention au main !!! */

    object output;

    int TEST=(input->type);


    switch (TEST)
    {
    case SFS_NUMBER:
        return input;
        break;

    case SFS_CHARACTER:
        return input;
        break;

    case SFS_STRING:
        return input;
        break;

    case SFS_NIL:
        return input;
        break;

    case SFS_BOOLEAN:
        return input;
        break;

    case SFS_SYMBOL:;
        object VAL=cherche_symbol(env,input);
        return (cdr(VAL));
        break;

    case SFS_PAIR:
        output=make_pair();

        object gauche=car(input);

        if (gauche->type==SFS_SYMBOL)
        {
            char*symb=gauche->this.symbol;

            if (  0 == est_ident( symb, "define" )  )    /*  Procédure de la fonction DEFINE  */
            {
                object var=(car(cdr(input)));

                object val= sfs_eval(car(cdr(cdr(input))),env);  /* Evalue la valeur implémentée */

                add_symb(env,var,val);
                printf("\n");
                sfs_print(env);
                if(cdr(cdr(cdr(input)))!=nil)
                {
                    WARNING_MSG("Trop de parametres pour la fonction define");
                }
                output=var;
                /*return output;*/
            }

            else if  (  0 == est_ident( symb, "set!" )  )
            {

                object test=cherche_symbol(env,car(cdr(input)));
                if (test!=nil)
                {
                    test->this.pair.cdr=sfs_eval(car(cdr(cdr(input))),env);
                    output=cdr(test);
                }
                else
                {
                    WARNING_MSG("La variable n'est pas definie");
                    output=nil;
                }
            }



            else if  (  0 == est_ident( symb, "quote" )  )
            {
                output=cdr(input);
            }

            else if  ( 0 == est_ident( symb, "cons" ) )    /* A prendre comme une primitive plus tard */
            {
                output->this.pair.car=sfs_eval(car(cdr(input)),env);
                object P=make_pair();
                P->this.pair.car=sfs_eval(car(cdr(cdr(input))),env);
                output->this.pair.cdr=P;
            }

            else if ( 0 == est_ident( symb, "+" ) )
            {
                object res=make_integer(0);
                object test=cdr(input);
                while (test!=nil)
                {
                    res=add_num(res,car(test));
                    test=cdr(test);
                }
                output=res;
            }

            else if ( 0 == est_ident( symb, "-" ) )
            {
                object res=make_integer(0);
                object test=cdr(input);
                res=add_num(res,car(test));
                test=cdr(test);
                while (test!=nil)
                {
                    res=sous_num(res,car(test));
                    test=cdr(test);
                }
                output=res;
            }

            else if ( 0 == est_ident( symb, "*" ) )
            {
                object res=make_integer(0);
                object test=cdr(input);
                res=add_num(res,car(test));
                test=cdr(test);
                while (test!=nil)
                {
                    res=mult_num(res,car(test));
                    test=cdr(test);
                }
                output=res;
            }

            else if ( 0 == est_ident( symb, "/" ) )
            {
                object res=make_integer(0);
                object test=cdr(input);
                res=add_num(res,car(test));
                test=cdr(test);
                while (test!=nil)
                {
                    res=div_num(res,car(test));
                    test=cdr(test);
                }
                output=res;
            }

            else
            {
                WARNING_MSG("Symbole inconu");
            }


        }
        else
        {
            output->this.pair.car=sfs_eval(car(input),env);
            output->this.pair.cdr=sfs_eval(cdr(input),env);
        }
        return output;
        break;

    }



}
