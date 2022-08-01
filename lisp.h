typedef enum {NUMBER, BOOL, ERROR} object_type;

typedef struct object {

    object_type type;

    union{
        
        struct {
            int value;
        } number;
    
        struct {
            char value;
        }bool;
    
    } data;

} object;

