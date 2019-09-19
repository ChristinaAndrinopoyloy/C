#include "helper.h"

/* makes the unique final product ID from item IDs */
void final_product_determinant(char *determinant, int IDa, int IDb, int IDc)
{
    char temp[5];

    sprintf(determinant, "%d", (10000 * IDc) + IDb);
    strcat(determinant, temp);

    sprintf(temp, "%d", IDa);
    strcat(determinant, temp);
}