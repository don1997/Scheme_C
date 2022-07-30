//CONS holds lists
//ATOM atoms and digits
//FUNC reference to a c func
//LAMBDA holds lambda expr
//

enum type {CONS, ATOM, FUNC, LAMBDA};

typedef struct {
    
    enum type type;

} object;


typedef struct {
    
    enum type type;
    
    char *name;
} atom_object;


typedef struct {
    
    enum type type;

    object *car;
    object *cdr;

} cons_object;


typedef struct {
    
    enum type type;

    object* (*fn)(object*, object*);

} func_object;


typedef struct {
    
    enum type type;

    object* args;
    object* sexp;

} lambda_object;

