/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include "object.h"

object sfs_eval( object input )
{
    object output;
    int TEST;


debut:

    TEST=(input->type);


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

    case SFS_PRIMITIVE:
        return input;
        break;

    case SFS_SYMBOL:
        ;
        object VAL=cherche_symbol(meta_env,input);
        if (VAL != nil)
        {
            return (cdr(VAL));
        }
        else
        {
            WARNING_MSG("La variable n'est pas definie");
            return Error;
        }
        break;

    case SFS_PAIR:
        output=make_pair();

        object gauche=car(input);


        if (gauche->type==SFS_SYMBOL)
        {
            object NoeudPrim=cherche_symbol(meta_env,gauche);



            if (  (NoeudPrim != nil)   &&   (cdr(NoeudPrim))->type == SFS_PRIMITIVE   )  /*  CAS DES PRIMITIVES */
            {
                object (*ptrPrim)(object);
                ptrPrim=(cdr(NoeudPrim))->this.primitive.function;

                object Liste=cdr(input);
                Liste=eval_arg(Liste);
                if (Liste==nil)
                {
                    WARNING_MSG("Argument missing");
                    if (est_ident(symbol(gauche),"+")==0 )
                    {
                        return make_integer(0);
                    }
                    else if (est_ident(symbol(gauche),"*")==0 )
                    {
                        return make_integer(1);
                    }
                    else
                    {
                        return nil;
                    }

                }
                output=(*ptrPrim)(Liste);
            }

            else
            {
                char*symb=gauche->this.symbol;

                if (  0 == est_ident( symb, "define" )  )                              /*  Procédure de la fonction DEFINE  */
                {
                    object test=is_symb(meta_env,car(cdr(input)));

                    if (test == nil)
                    {
                        object var=car(cdr(input));

                        object val= sfs_eval(car(cdr(cdr(input))));  /* Evalue la valeur implémentée */

                        add_symb(meta_env,var,val);

                        /*sfs_print_env(meta_env);*/    /* Affiche l'environnement */

                        output=var;
                    }
                    else
                    {
                        test->this.pair.cdr= sfs_eval(car(cdr(cdr(input))));
                        /*output=cdr(test);*/
                        output=(car(cdr(input)));
                        /*sfs_print_env(env);*/    /* Affiche l'environnement */
                    }


                    if(cdr(cdr(cdr(input)))!=nil)
                    {
                        WARNING_MSG("Too many arguments for DEFINE");
                        output=Error;
                    }
                }

                else if  (  0 == est_ident( symb, "set!" )  )                           /*  SET! */
                {

                    object test=cherche_symbol(meta_env,car(cdr(input)));
                    if (test!=nil)
                    {
                        /*free(test->this.pair.cdr);*/
                        test->this.pair.cdr=sfs_eval(car(cdr(cdr(input))));
                        output=cdr(test);

                        /*sfs_print_env(meta_env);*/    /* Affiche l'environnement */
                    }
                    else
                    {
                        WARNING_MSG("Symbol not defined");
                        output=Error;
                    }
                }


                else if  (  0 == est_ident( symb, "quote" )  )                           /*  QUOTE  */

                {
                    output=car(cdr(input));
                }


                else if  (  0== est_ident( symb, "if" )  )              /*  IF  */
                {
                    object test=sfs_eval(predicat(input));

                    if ( test == true )
                    {
                        /*output=sfs_eval(consequence(input),env);*/
                        input=consequence(input);
                        goto debut;
                    }
                    else if ( test == false )
                    {
                        if ( alternative(input) != nil)
                        {
                            /*output=sfs_eval(alternative(input),env);*/
                            input=alternative(input);
                            goto debut;
                        }
                        else
                        {
                            output=false;
                        }
                    }
                    else
                    {
                        WARNING_MSG("Le prédicat n'est pas un booléen");
                        return Error;
                    }

                }


                else if (  0== est_ident( symb, "and" )  )              /*  AND  */
                {
                    if ( cdr(input)==nil )
                    {
                        ERROR_MSG("Not enough argument for AND\n");
                        return Error;
                    }
                    if ( cdr(cdr(input))==nil )
                    {
                        ERROR_MSG("Not enough argument for AND\n");
                        return Error;
                    }

                    object test=cdr(input);

                    output=true;

                    while (test != nil)
                    {
                        if ( car(test)==false )
                        {
                            output= false;
                        }
                        test=cdr(test);
                    }

                }


                else if (  0== est_ident( symb, "or" )  )              /*   OR   */
                {
                    if ( cdr(input)==nil )
                    {
                        WARNING_MSG("Not enough argument for OR\n");
                        return Error;
                    }
                    if ( cdr(cdr(input))==nil )
                    {
                        WARNING_MSG("Not enough argument for OR\n");
                        return Error;
                    }

                    object test=cdr(input);

                    output=false;

                    while (test != nil)
                    {
                        if ( car(test)==true )
                        {
                            return true;
                        }
                        test=cdr(test);
                    }

                }

                else
                {
                    object test=cherche_symbol(meta_env,gauche);
                    if (test != nil)
                    {
                        output->this.pair.car=sfs_eval(car(input));
                        output->this.pair.cdr=sfs_eval(cdr(input));
                    }
                    else
                    {
                        WARNING_MSG("La variable n'est pas definie");
                        return Error;
                    }

                }

            }


        }

        else
        {
            WARNING_MSG("Fonction inconnue");
            output=Error;
            /*
            output->this.pair.car=sfs_eval(car(input));
            output->this.pair.cdr=sfs_eval(cdr(input));
            */
        }

        return output;

        break;

    }

    return nil ; /*fonction doit renvoyer dans tous les cas un objet*/
}
