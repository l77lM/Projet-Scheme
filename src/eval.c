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

object sfs_eval( object input, object env )
{


    DEBUG_MSG("input %p %d", input, input->type);
 /*   printf("EVAL ****");
    sfs_print(input);
    printf("***\n");  */

    if (input==Error)
    {
        return NULL;
    }

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
        DEBUG_MSG("Eval symbol");

        /*    DEBOGAGE    */

        object VAL=cherche_symbol(env,input);

        if (VAL != nil)
        {
            return (cdr(VAL));
        }
        else
        {
            WARNING_MSG("Symbol n'est pas definie");
            return NULL;
        }
        break;

    case SFS_PAIR:
        output=make_pair();

        object gauche=car(input);

        if (gauche->type==SFS_PAIR)
        {
            if (car(gauche)->type==SFS_SYMBOL)
            {
                if ( 0 == est_ident( symbol(car(gauche)), "lambda" ))
                {
                    DEBUG_MSG("lambda detecté aprés 2 parenthese");
                    input->this.pair.car=sfs_eval(gauche, env);
                    output=sfs_eval(input, env);

                }
                else
                {
                    WARNING_MSG("Two paranthesis = not good !");
                    return Error;
                }
            }
        }


        else if (gauche->type==SFS_SYMBOL)
        {
            object NoeudPrim=cherche_symbol(env,gauche);


            if (  (NoeudPrim != nil)   &&   (cdr(NoeudPrim))->type == SFS_PRIMITIVE   )  /*  CAS DES PRIMITIVES */
            {
                object (*ptrPrim)(object);
                ptrPrim=(cdr(NoeudPrim))->this.primitive.function;

                object Liste=cdr(input);
                Liste=eval_arg(Liste, env);
                if (Liste==nil)               /* Respect de la norma quand pas d'argument */
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

                    else if (est_ident(symbol(gauche),"list")==0 )
                    {
                        return nil;
                    }

                    else
                    {
                        return Error;
                    }

                }
                output=(*ptrPrim)(Liste);
            }

            else if (  (NoeudPrim != nil)   &&   (cdr(NoeudPrim))->type == SFS_COMPOUND   )    /*  Evaluation d'un symbole associé à une fonction LAMBDA */
            {
                input->this.pair.car=cdr(NoeudPrim);
                goto debut;
            }

            else
            {
                char*symb=gauche->this.symbol;

                if (  0 == est_ident( symb, "define" )  )                        /*  Procédure de la fonction DEFINE  */
                {
                    object arg1=car(cdr(input));
                    object arg2=car(cdr(cdr(input)));

                    if (arg1==Error)
                    {
                        WARNING_MSG("Argument 1 missing for DEFINE");
                        return Error;
                    }

                    if (arg2==Error)
                    {
                        WARNING_MSG("Argument 2 missing for DEFINE");
                        return Error;
                    }

                    if (arg1->type==SFS_PAIR)                                /* Deuxieme definition d'une fonction lambda */
                    {
                        if ( car(arg1)->type != SFS_SYMBOL)
                        {
                            WARNING_MSG("Wrong definition of lambda");
                            return Error;
                        }
                        else
                        {
                            object nomProc=car(arg1);
                            object varProc=cdr(arg1);
                            object corpProc=arg2;
                            add_symb(env,nomProc,make_compound(varProc,corpProc,env));
                            output=nomProc;
                        }

                    }
                    else if (arg1->type == SFS_SYMBOL)
                    {
                        object test=is_symb(meta_env,car(cdr(input)));   /* Permet de ne pas modidier de symbole associé aux formes ou primitives */

                        if (test != nil)
                        {
                            WARNING_MSG("Not the rights to define this symbol");
                            return Error;
                        }

                        test=is_symb(env,car(cdr(input)));

                        if (test == nil)
                        {
                            object var=car(cdr(input));

                            object val= sfs_eval(car(cdr(cdr(input))), env);

                            add_symb(env,var,val);

                            output=var;
                        }
                        else
                        {
                            test->this.pair.cdr= sfs_eval(car(cdr(cdr(input))), env);
                            output=(car(cdr(input)));
                        }


                        if(cdr(cdr(cdr(input)))!=nil)
                        {
                            WARNING_MSG("Too many arguments for DEFINE");
                            output=Error;
                        }
                    }
                    else
                    {
                        WARNING_MSG("Impossible to define this argument");
                        return Error;
                    }
                }

                else if  (  0 == est_ident( symb, "set!" )  )                           /*  SET! */
                {

                    object test=cherche_symbol(env,car(cdr(input)));
                    if (test!=nil)
                    {
                        test->this.pair.cdr=sfs_eval(car(cdr(cdr(input))), env);
                        output=cdr(test);
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


                else if  (  0 == est_ident( symb, "if" )  )              /*  IF  */
                {
                    if (cdr(input)==nil)
                    {
                        WARNING_MSG("Argument 1 missing in 'if'");
                        return Error;
                    }

                    object pred=predicat(input, env);
                    if(pred==Error)
                    {
                        WARNING_MSG("Error on predicat in 'if'");
                        return Error;
                    }

                    object test=sfs_eval(pred, env);

                    if ( test == true )
                    {
                        /*output=sfs_eval(consequence(input),env);*/
                        input=consequence(input, env);
                        if (input==Error)
                        {
                            WARNING_MSG("Consequence missing in 'if'");
                            return Error;
                        }

                        goto debut;
                    }
                    else if ( test == false )
                    {
                        object test2=alternative(input, env);

                        if ( test2 != Error)
                        {
                            input=test2;
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

                else if  (  0 == est_ident( symb, "begin" )  )                           /*  BEGIN  */
                {
                    if ( cdr(input)==nil )
                    {
                        WARNING_MSG("Not enough argument for BEGIN\n");
                        return Error;
                    }
                    object res=nil;
                    object test=cdr(input);
                    while (test!=nil)
                    {
                        res=sfs_eval(car(test), env);
                        test=cdr(test);
                    }
                    return res;
                }

                else if  (  0 == est_ident( symb, "lambda" )  )                           /*  LAMBDA  */
                {
                    DEBUG_MSG("lambda détecté aprés 1 paranthese");
                    if ( cdr(input)==nil )
                    {
                        WARNING_MSG("Not enough argument for LAMBDA\n");
                        return Error;
                    }
                    if ( cdr(cdr(input))==nil )
                    {
                        WARNING_MSG("Not enough argument for LAMBDA\n");
                        return Error;
                    }
                    object parms=car(cdr(input));
                    object body=car(cdr(cdr(input)));

                    return make_compound(parms,body,env);
                }

                else if  (  0 == est_ident( symb, "let" )  )                           /*  LET  */

                {
                    object rep=let_lambda(cdr(input),env);

                    if (rep==Error)
                    {
                        return Error;
                    }
                    else
                    {
                        output=sfs_eval(rep,env);
                    }
                }



                else
                {
                    object test=cherche_symbol(env,gauche);
                    if (test != nil)
                    {
                        output->this.pair.car=sfs_eval(car(input), env);
                        output->this.pair.cdr=sfs_eval(cdr(input), env);
                    }
                    else
                    {
                        WARNING_MSG("La variable n'est pas definie");
                        return Error;
                    }

                }

            }


        }

        else if (gauche->type==SFS_COMPOUND)
        {
            DEBUG_MSG("Evaluation of lambda");
            object fonct=gauche;

            object env_local=fonct->this.compound.envt;
            env_local=make_newENV(env_local);

            object param=fonct->this.compound.parms;


            object var=cdr(input);

            while (param!=nil)
            {
                if (var!=nil)
                {
                    add_symb(env_local,car(param),sfs_eval(car(var),env));
                    param=cdr(param);
                    var=cdr(var);
                }
                else
                {
                    WARNING_MSG("Not Enough Arguments for the function");
                    return Error;
                }
            }

            if (var!=nil)
            {
                WARNING_MSG("Too many arguments for the function");
            }

            output=sfs_eval(fonct->this.compound.body, env_local);

        }

        else
        {

            WARNING_MSG("Unknown Function");
            output=Error;

            /*
            output->this.pair.car=sfs_eval(car(input), env);
            output->this.pair.cdr=sfs_eval(cdr(input), env);
            */
        }

        return output;

        break;

    }

    return Error ; /*fonction doit renvoyer dans tous les cas un objet*/
}
