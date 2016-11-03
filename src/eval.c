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

object sfs_eval( object input )        /* Tentative de prise en compte de l'environnement, Attention au main !!! */
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
            WARNING_MSG("La variable \"%s\" n'est pas definie",input->this.symbol);
            return nil;
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
                        WARNING_MSG("Trop de parametres pour la fonction define");
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
                        WARNING_MSG("La variable n'est pas definie");
                        output=nil;
                    }
                }


                else if  (  0 == est_ident( symb, "quote" )  )                           /*  QUOTE  */

                {
                    output=car(cdr(input));
                }

                else if  ( 0 == est_ident( symb, "cons" ) )    /* A prendre comme une primitive plus tard */
                {
                    output->this.pair.car=sfs_eval(car(cdr(input)));
                    object P=make_pair();
                    P->this.pair.car=sfs_eval(car(cdr(cdr(input))));
                    output->this.pair.cdr=P;
                }


                else if ( 0 == est_ident( symb, "-" ) )                                 /*  Soustraction  */
                {
                    object res=make_integer(0);
                    object test=cdr(input);
                    res=add_num(sfs_eval(res),sfs_eval(car(test)));
                    test=cdr(test);
                    while (test!=nil)
                    {
                        res=sous_num(sfs_eval(res),sfs_eval(car(test)));
                        test=cdr(test);
                    }
                    output=res;
                }

                else if ( 0 == est_ident( symb, "*" ) )                                 /*  Multiplication  */
                {
                    object res=make_integer(0);
                    object test=cdr(input);
                    res=add_num(sfs_eval(res),sfs_eval(car(test)));
                    test=cdr(test);
                    while (test!=nil)
                    {
                        res=mult_num(sfs_eval(res),sfs_eval(car(test)));
                        test=cdr(test);
                    }
                    output=res;
                }

                else if ( 0 == est_ident( symb, "/" ) )                                 /*  Division  */
                {
                    object res=make_integer(0);
                    object test=cdr(input);
                    res=add_num(sfs_eval(res),sfs_eval(car(test)));
                    test=cdr(test);
                    while (test!=nil)
                    {
                        res=div_num(sfs_eval(res),sfs_eval(car(test)));
                        test=cdr(test);
                    }
                    output=res;
                }


                else if  (  0 == est_ident( symb, "car" )  )             /*  CAR  */
                {
                    output=sfs_eval(car(car(cdr(input))));
                }


                else if  (  0 == est_ident( symb, "cdr" )  )             /*  CDR  */
                {
                    output=sfs_eval(cdr(car(cdr(input))));
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
                    }

                }


                else if (  0== est_ident( symb, "and" )  )              /*  AND  */
                {
                    if ( cdr(input)==nil )
                    {
                        ERROR_MSG("Pas assez d'argument pour \"and\"\n");
                    }
                    if ( cdr(cdr(input))==nil )
                    {
                        ERROR_MSG("Pas assez d'argument pour \"and\"\n");
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
                        ERROR_MSG("Pas assez d'argument pour \"or\"\n");
                    }
                    if ( cdr(cdr(input))==nil )
                    {
                        ERROR_MSG("Pas assez d'argument pour \"or\"\n");
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

                else if (  0== est_ident( symb, "<" ) || 0== est_ident( symb, ">" ) ||  0== est_ident( symb, "=" ) )
                {
                    output=eval_bool(input);
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
                        WARNING_MSG("La variable \"%s\" n'est pas definie",input->this.symbol);
                    }

                }

            }


        }

        else
        {

            output->this.pair.car=sfs_eval(car(input));
            output->this.pair.cdr=sfs_eval(cdr(input));
        }

        return output;

        break;

    }

    return nil ; /*fonciton doit renvoyer dans tous les cas un objet*/
}
