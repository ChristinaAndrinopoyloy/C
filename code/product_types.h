#ifndef _PRODUCT_TYPES_H_
#define _PRODUCT_TYPES_H_

// structures of project
// typedefs
typedef struct component
{
    int ID;
    int type_of_component; //type_of_component = 1 or 2 or 3
    char colour;        //colour = b for black or w for white
    char checked;
    clock_t start;
    clock_t end;
} component;

typedef struct final_product
{
    char *determinant;
    double average_total_time;
} final_product;

#endif