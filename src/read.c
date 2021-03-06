/**
 * @file read.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:31:05 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */


#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "read.h"





void flip( uint *i )
{

    if ( *i == FALSE )
    {
        *i = TRUE;
    }
    else
    {
        *i = FALSE;
    }
}

/*
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line)
{

    int i=0;
    if (line == NULL)
    {
        return NULL;
    }
    i = 0;
    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i]))
    {
        i++;
    }
    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0')
    {
        return NULL;
    }
    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';')
    {
        return NULL;
    }
    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que
 *        - la ligne est fini, la fonction retourne S_OK
 * 				- la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum
{
    NOTHING,        /* rien n'a ete trouve encore.. */
    STRING_ATOME,   /* la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /* la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /* la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /* on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp )
{
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do
    {
        ret = NULL;
        chunk = NULL;

        /* si en mode interactif*/
        if ( stdin == fp )
        {
            uint nspaces = 2*parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i= 0; i< nspaces; i++ )
            {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces>0)
            {
                input[strlen(input)+1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            chunk = readline( sfs_prompt );
        }
        /*si en mode fichier*/
        else
        {
            chunk=k;
            memset( chunk, '\0', BIGSTRING );
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret )
            {
                /* fin de fichier...*/
                if ( parlevel != 0 )
                {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }
                return S_END;
            }

            if (strlen(chunk) == BIGSTRING-1
                    && chunk[BIGSTRING-1] != '\n'
                    && !feof(fp))
            {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile
        	=> on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL)
        {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 )
        {
            if (strlen(input) + s > BIGSTRING-1 )
            {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i< strlen(chunk); i++ )
            {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL )
                {
                    chunk[i]='\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i])
                {
                case '(':
                    if (in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) )
                    {
                        parlevel++;
                        typeOfExpressionFound = S_EXPR_PARENTHESIS;
                    }
                    break;
                case ')':
                    if ( in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) )
                    {
                        parlevel--;
                        if (parlevel == 0 )
                        {
                            typeOfExpressionFound = FINISHED;
                        }
                        if ( parlevel < 0 )
                        {
                            WARNING_MSG( "Parse error : cannot start with ')'" );
                            return S_KO;
                        }
                    }
                    break;
                case '"':
                    if ( i<2 || chunk[i-1] != '\\' )
                    {
                        if ( in_string == FALSE )
                        {
                            if(typeOfExpressionFound == BASIC_ATOME)
                            {
                                WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk+i);
                                return S_KO;
                            }
                            in_string = TRUE;
                            if(typeOfExpressionFound != S_EXPR_PARENTHESIS)
                            {
                                typeOfExpressionFound = STRING_ATOME;
                            }
                        }
                        else
                        {
                            in_string = FALSE;
                            if(typeOfExpressionFound == STRING_ATOME)
                            {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                    }
                    break;
                default:
                    if(in_string == FALSE)
                    {
                        if(isspace(chunk[i]))
                        {
                            if(typeOfExpressionFound == BASIC_ATOME)
                            {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                        else if(typeOfExpressionFound != S_EXPR_PARENTHESIS)
                        {
                            typeOfExpressionFound = BASIC_ATOME;
                        }
                    }
                    break;
                }


                if(typeOfExpressionFound == FINISHED)
                {
                    char *first_useful = first_usefull_char(chunk + i + 1);
                    if( first_useful != NULL)
                    {
                        if(*first_useful == ')' )
                        {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        }
                        else
                        {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'", chunk+i);
                        }
                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }
            if(in_string == TRUE)
            {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin )
        {
            if ( feof( fp ) )
            {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
        }
    }
    while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input)-1])) input[strlen(input)-1] = '\0';

    if(stdin == fp)
    {
        add_history( input );
    }
    return S_OK;
}

object sfs_read( char *input, uint *here )
{
    char car1 = input[(*here)];

    while ((car1==' ')|| (car1=='\t')||(car1=='\n'))
    {
        (*here)++;
        car1 = input[(*here)];
    } /*Permet d'éliminer les espaces et tabulations qui generaient la lecture du code*/

    if ( input[*here] == '(' )
    {

        if ( input[(*here)+1] == ')' )
        {
            *here += 2;
            return nil;
        }
        else
        {
            *here += 1;
            return sfs_read_pair( input, here );
        }
    }

    else
    {
        return sfs_read_atom( input, here );
    }
}

object sfs_read_atom( char *input, uint *here )
{

    object atom = NULL;

    char car1 = input[(*here)];
    char car2 = input[(*here)+1];
    char car3 = input[(*here)+2];
    char car4 = input[(*here)+3];
    /*long int nb;*/
    double nb;




    while  (  (car1=='\t') ||  (car1==' ') || (car1=='\n'))
    {
        (*here)++;
        car1=car2;
        car2=car3;
        car3=input[(*here)+2];
    }   /*Permet d'éliminer les espaces et tabulations qui generaient la lecture du code*/



    if   (    ( (car1=='+'||car1=='-') && ( ( ('0' <= car2) && ( car2 <= '9') ) || (car2=='i'  &&  car3=='n' &&  car4=='f')  ) )   ||  ( ('0' <= car1) && ( car1 <= '9')   )  )
    {
        if ( car2=='i' )
        {
            if (car1=='+')
            {
                atom=make_infty(0);
            }
            else if (car1=='-')
            {
                atom=make_infty(-1);
            }
            (*here)+=4;
        }

        else
        {
            char* ptr;

            /*nb = strtol( (input+(*here)), &ptr, 10);*/
            nb = strtod( (input+(*here)), &ptr);

            (*here)=ptr-input;

            if (errno == ERANGE )
            {

                atom=make_infty(nb);

            }
            if (errno != ERANGE)
            {
                if (est_entier(nb)==0)
                {
                    atom=make_integer(nb);
                }
                else
                {
                    atom=make_real(nb);
                }

                /*atom=make_integer(nb);*/
            }
            errno=0;
        }

    }


    else if ( car1=='"' )  /* Cas des chaines de caractères */
    {
        car1=car2;
        car2=input[(*here)++];
        /*char *ptr_realloc=NULL;*/

        char* LU=malloc(strlen(input+(*here))*sizeof(char));     /* Une réallocation permetrait ensuite de limiter la taille allouée à la chaine lu à sa taille réel */
        int cpt=0;

        while ( input[(*here)] != '"')
        {
            car1=car2;
            car2=input[(*here)++];

            if (  (car2=='\\') &&  (car1!='#')  )
            {
                car2=input[(*here)++];
            }

            LU[cpt++]=car2;

            /*
                        if (ptr_realloc = realloc(LU, (strlen(LU)+1)*sizeof(char)))
                        {
                            LU = ptr_realloc;
                        }
                        else
                        {
                            printf("Erreur d'affectation");
                            free(ptr_realloc);
                            break;
                        }
            */

        }
        (*here)++;
        atom=make_string(LU);

    }



    else if ( car1=='#' )      /* Cas des symboles */
    {
        if (  (car2=='t') && (car3=='\0' || car3==' ' || car3==')')  )
        {
            atom=true;
            (*here)+=2;
        }

        else if (  (car2=='f') && (car3=='\0' || car3==' ' || car3==')')  )
        {
            atom=false;
            (*here)+=2;

        }

        else if (car2=='\\')
        {
            /*on suppose le mot lu de taille < 50  et on stocke newline et space ss forme de symbole 		 				#backslashnewline*/
            /*char lu[50];
            sscanf(input+(*here),"%s",lu);*/
            /*lu=strtok(lu,")");*/
            DEBUG_MSG("Lecture charactere");


            char *lu=malloc((strlen(input+(*here))+1)*sizeof(char));
            strcpy(lu, (input+(*here)) );

            sscanf(input+(*here),"%s",lu);
            lu=strtok(lu,")");

            if   (strcmp(lu,"#\\newline")==0)
            {
                DEBUG_MSG("newline");
                atom=make_character('\n');
                *(here)=*(here)+strlen(lu);
            }

            else if   (strcmp(lu,"#\\space")==0)
            {
                DEBUG_MSG("space");
                atom=make_character(' ');
                *(here)=*(here)+strlen(lu);
            }



            else if (strlen(lu)==3)
            {
                DEBUG_MSG("char taille 3");
                atom=make_character(input[(*here)+2]);
                (*here)+=3;
            }

            else
            {
                ERROR_MSG("Character unknown");
                return NULL;
            }
        }
    }

    else if (car1=='\'')
    {
        (*here)=(*here)+1;

        atom=make_pair();
        (atom->this).pair.car=make_symbol("quote");

        object paire=make_pair();
        (paire->this.pair.car)= sfs_read(input,here);


        (atom->this.pair.cdr)=paire;
    }

    else
    {

        /*cas des chaines de caracteres : allocation dynamique + copie de la chaine
        strtok afin de couper le mot dans le cas ou il y a une ")" */

        char *lu=malloc((strlen(input)+1)*sizeof(char));
        strcpy(lu, (input+(*here)) );

        sscanf(input+(*here),"%s",lu);
        lu=strtok(lu,")");

        *(here)=*(here)+strlen(lu);
        atom=make_symbol(lu);
    }

    return atom;
}



object sfs_read_pair( char *stream, uint *i )
{


    char car1 = stream[(*i)];

    while ((car1==' ')|| (car1=='\t') || (car1=='\n') )
    {
        (*i)++;
        car1 = stream[(*i)];
    } /*Permet d'éliminer les espaces et tabulations qui generaient la lecture du code*/


    if( stream[(*i)]==')')
    {
        (*i)+=1;
        return nil;
    }


    object pair= make_pair();

    (pair->this.pair.car)= sfs_read(stream,i);

    if (car(pair)->type==SFS_SYMBOL)
    {
        DEBUG_MSG("car(pair) = symb");
        if   (  est_ident(symbol(car(pair)),".") == 0 )
        {
            DEBUG_MSG("Paire . incomplete");
            /*(*i)+=1;*/
            object retour=sfs_read(stream,i);

            /*sfs_print_atom(retour);*/

            while ((car1==' ')|| (car1=='\t') || (car1=='\n') )
            {
                (*i)++;
                car1 = stream[(*i)];
            } /*Permet d'éliminer les espaces et tabulations qui generaient la lecture du code*/

            if (stream[(*i)] == ')')
            {
                return retour;
            }

            else
            {
                (*i)++;
                return sfs_read_pair(stream,i);
                /*(pair->this.pair.cdr)= sfs_read_pair(stream,i);*/
            }
            /*return sfs_read(stream,i);*/


            /*
            sfs_print_atom(cdr(pair));
            return pair;
            */
        }
    }

    (pair->this.pair.cdr)= sfs_read_pair(stream,i);
    return pair;
}




