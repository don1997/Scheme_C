typedef enum {NUMBER} object_type;

typedef struct object {

    object_type type;

    union{
        
        struct {
            int value;
        } number;

    } data;

} object;
