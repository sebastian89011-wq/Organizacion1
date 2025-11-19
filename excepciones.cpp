

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

typedef enum {
    ERR_NONE = 0,
    ERR_CREDIT = 1,
    ERR_STOCK = 2
} ErrorCode;

jmp_buf jmp_env;

typedef struct {
    int id;
    double monto;
    int producto_id;
    int cantidad;
    double cliente_credit_limit;
} Orden;

int stock_por_producto(int producto_id) {
    
    if (producto_id == 1) return 5;
    if (producto_id == 2) return 0;
    return 10;
}

void lanzar_error(ErrorCode code) {
 
    longjmp(jmp_env, (int)code);
}

void procesar_orden(Orden *o) {
    printf("Procesando orden %d: monto=%.2f, prod=%d, cant=%d, credito=%.2f\n",
           o->id, o->monto, o->producto_id, o->cantidad, o->cliente_credit_limit);


    if (o->monto > o->cliente_credit_limit) {
        printf("  -> Error: monto (%.2f) supera credito (%.2f)\n", o->monto, o->cliente_credit_limit);
        lanzar_error(ERR_CREDIT);
    }

    int stock = stock_por_producto(o->producto_id);
    if (o->cantidad > stock) {
        printf("  -> Error: stock insuficiente (solic: %d, stock: %d)\n", o->cantidad, stock);
        lanzar_error(ERR_STOCK);
    }

 
    printf("  -> Orden %d procesada correctamente.\n", o->id);
}

int main(void) {
    Orden ordenes[] = {
        {1, 150.0, 1, 2, 200.0},   
        {2, 500.0, 1, 1, 300.0},   
        {3, 80.0, 2, 1, 200.0},    
        {4, 50.0, 1, 1, 100.0}     
    };
    size_t n = sizeof(ordenes)/sizeof(ordenes[0]);

    for (size_t i = 0; i < n; ++i) {
        int result = setjmp(jmp_env);
        if (result == 0) {
            
            procesar_orden(&ordenes[i]);
        } else {
            
            if (result == ERR_CREDIT) {
                printf("Manejador central: Error de crédito en orden %d. Registrar y notificar.\n", ordenes[i].id);
            } else if (result == ERR_STOCK) {
                printf("Manejador central: Error de stock en orden %d. Reordenar stock.\n", ordenes[i].id);
            } else {
                printf("Manejador central: Error desconocido (%d) en orden %d.\n", result, ordenes[i].id);
            }
            
        }
        printf("---------------------------------------------------\n");
    }

    printf("Procesamiento finalizado.\n");
    return 0;
}
