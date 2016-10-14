
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

    int test=(input->type);


    switch (test)
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

    case SFS_SYMBOL:
        return input;
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
                object val= sfs_eval(car(cdr(cdr(input))));  /* Evalue la valeur implémentée */
                add_symb(env,symb,val);

                if(cdr(cdr(cdr(input)))!=nil)
                {
                    WARNING_MSG("Trop de parametres pour la fonction define")
                }
                output=var;
                /*return output;*/
            }

            else if  (  0 == est_ident( symb, "quote" )  )
            {
                output=cdr(input);
            }

        }
        return output;
        break;

    }



}
