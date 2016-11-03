/**
 * @file object.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 17:49:46 2012
 * @brief Object definition for SFS.
 *
 * Object definition for SFS.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "number.h"


typedef struct object_t
{

    uint type;

    union
    {

        num              number;
        char             character;
        string           string;
        string           symbol;

        struct pair_t
        {
            struct object_t *car;
            struct object_t *cdr;
        }                pair;

        struct object_t *special;

        struct
        {
            struct object_t* (*function)(struct object_t*);
        } primitive;

    } this;

} *object;




object make_object( uint type );
object make_primitive(object (*ptrPrim)(object));    /* Primitive */
object make_nil( void );
object make_symbol(char* symb);
object make_character(char car);
object make_string(char* str);
object make_pair();

object make_boolean( uint test );
object init_boolean( void );

int est_entier(double d);
object make_infty(int i);
object make_real(double X);
object make_integer(int N);
object make_undef(void);

object make_newENV(object env);                         /* Manipulation Environnements */
object add_symb(object env,object var,object val);
object is_symb(object env,object symb);
object cherche_symbol(object env,object symb);
object modif_symbole_env(object env,object symb,object val);

int est_ident(char* c1,char* c2);

object car(object paire);
object cdr(object paire);


object compare_num(object A,object B);


object predicat(object A);              /* Fonctions liées à if   */
object consequence(object A);
object alternative(object A);

object eval_bool(object A);

object exist_symb(char* nomsymb);       /* Fonctions liées à la table de symbole */
object add_table(char* nomsymb);
void init_table();
void affiche_table();




#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06
#define SFS_PRIMITIVE    0x07



extern object nil;

extern object true;

extern object false;

extern object empty_list;

extern object meta_env;

extern object table_symbol;

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
