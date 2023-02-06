typedef enum {NUMBER, BOOL, CHAR_LITERAL, STRING, EMPTYLIST, ERROR, PAIR, SYMBOL, PRIMITIVE_PROC, COMPOUND_PROC} object_type;

typedef struct object {

    object_type type;

    union{
        
        struct {
            int value;
        } number;
    
        struct {
            char value;
        }boolean;
    
        struct {    
            char value;
        } character_literal;

        struct {
            char *value;
        } string_val;

        struct {
            struct object *car;
            struct object *cdr;

        } pair;

        struct {

            char *value;

        } symbol;

        struct {
            struct object *(*fn)(struct object *arguements);

        }primitive_proc;

        struct{
          struct object *parameters;
          struct object *body;
          struct object *env;
        } compound_proc;

    } data;

} object;

extern object *symbol_table;
extern object *false;                                
extern object *true;      
extern object *empty_list;
extern object *quote_symbol;

extern object *define_symbol;
extern object *set_symbol;
extern object *set_symbol;
extern object *ok_symbol;
extern object *the_empty_environment;
extern object *global_environment;

extern object *if_symbol;
extern object *lambda_symbol;
object *car(object *pair);

void set_car(object *obj, object *value);

object *cdr(object *pair);

void set_cdr(object *obj, object *value);


#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caar(obj)   car(car(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))  
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))


/*      DATA        */

void add_binding_to_frame(object *var, object *val, object *frame);
object *enclosing_environment(object *env);
object *first_frame(object *env);

object *make_frame(object *var, object *val);

object *frame_variables(object *frame);
object *frame_values(object *frame);
object *make_obj();
object *makeNum(int a);
object *make_error();
object *make_char_literal(char a);
object *make_string(char *a);
object *make_symbol(char *a);
object *cons(object *car, object *cdr);
object *extend_environment(object *var, object *values, object *base_env);
void define_variable(object *var, object *value, object *env);
object *lookup_variable_value(object *var, object *env);
void set_variable_value(object *var, object *value, object *env);

object *setup_environment(void);

object *add_proc(object *arguments);
object *make_primitive_proc(
           object *(*fn)(struct object *arguments));

//STD PROCS
//

char is_primitive_proc(object *obj);

object *is_null_proc(object *arguments);
object *is_boolean_proc(object *arguments);
object *is_symbol_proc(object *arguments);

object *is_integer_proc(object *arguments);

object *is_char_proc(object *arguments);

object *is_string_proc(object *arguments);

object *is_pair_proc(object *arguments);

object *is_procedure_proc(object *arguments);

object *char_to_integer_proc(object *arguments);

object *integer_to_char_proc(object *arguments);

object *number_to_string_proc(object *arguments);

object *string_to_number_proc(object *arguments);

object *symbol_to_string_proc(object *arguments);

object *string_to_symbol_proc(object *arguments);

object *add_proc(object *arguments);
    
object *sub_proc(object *arguments);

object *mul_proc(object *arguments);
    
object *quotient_proc(object *arguments);

object *remainder_proc(object *arguments);

object *is_number_equal_proc(object *arguments);

object *is_less_than_proc(object *arguments);

object *is_greater_than_proc(object *arguments);
object *cons_proc(object *arguments);

object *car_proc(object *arguments);

object *cdr_proc(object *arguments);

object *set_car_proc(object *arguments);

object *set_cdr_proc(object *arguments);

object *list_proc(object *arguments);

object *is_eq_proc(object *arguments);

object *make_compound_proc(object *parameters, object *body,
                           object* env);

char is_compound_proc(object *obj);

char is_last_exp(object *seq);
//STD PROCS

/*      READ        */

char isBool(object *obj);
char isFalse(object *obj);
char isTrue(object *obj);
int isNum(object *obj);
char isCharLiteral(object *obj);
int isdelim(int a);
int isNewLine(int a);
char isString(object *obj);
char isEmptyList(object *obj);
char isPair(object *obj);
char isSymbol(object *obj);
char is_initial(int c);
int peek();
void expect_delim();
void eat_expect_string(char *str);
void eat_whitespace();
object *read_character(void);

object *read(void);

object *read_pair();



/*      EVAL        */

char is_self_eval(object *exp);
char is_variable(object *expression);
char is_tagged_list(object *exp, object *tag);

char is_quoted(object *exp);
object *text_of_quotation(object *exp);
char is_assignment(object *exp);
object *assignment_value(object *exp);
char is_definition(object *exp);
object *definition_variable(object *exp);
object *definition_value(object *exp);
char is_if(object *expression);
object *if_predicate(object *exp);
object *if_consequent(object *exp);
object *if_alternative(object *exp);

object *eval_assignment(object *exp, object *env);
object *eval_definition(object *exp, object *env);
object *eval(object *exp, object *env);
/*      WRITE       */
void write(object *obj);
void write_pair(object *pair);
void write(object *obj);


