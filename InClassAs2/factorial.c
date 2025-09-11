#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

void process_array(int *arr, int size, int multiplier) {
    for (int i = 0; i < size; i++) {
        arr[i] = arr[i] * multiplier + i;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number> <multiplier>\n", argv[0]);
        return 1;
    }
    
    int num = atoi(argv[1]);
    int mult = atoi(argv[2]);
    
    printf("Computing factorial of %d\n", num);
    int fact_result = factorial(num);
    printf("Factorial: %d\n", fact_result);
    
    int data[] = {10, 20, 30, 40, 50};
    int size = sizeof(data) / sizeof(data[0]);
    
    printf("Original array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    process_array(data, size, mult);
    
    printf("Modified array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    return 0;
}
