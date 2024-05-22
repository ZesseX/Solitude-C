#include "exit_reason.h"

void exit_reason_print(exit_code code) {
    printf("Exited with code %d: ");
    switch (code) {
        default: {
            printf("Placeholder exit reason print message");
        } break;
    }
}
