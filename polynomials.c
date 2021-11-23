#include <stdio.h>
#include <ctype.h>

#define MAXIMUM_DEGREE 10
#define true 1
#define false 0

void add_polynomials(int *current_char, int *coefficients);
void multiply_polynomials(int *current_char, int *coefficients);
void empty_arrays(int *array);
void transform_arrays(int *multiplied_coefficients, int *coefficients);
void print_polynomials(int *coefficients);
int get_exponent(int *current_char);
int get_number(int *current_char, int *negative);
int get_absolute_value(int number);

void empty_arrays(int *array) {
    for (int i = 0; i < MAXIMUM_DEGREE + 1; i++) {
        array[i] = 0;
    }
}

/*overwrites the current array of coefficients with the updated, multiplied coefficients*/
void transform_arrays(int *multiplied_coefficients, int *coefficients) {
    for(int i = 0; i < MAXIMUM_DEGREE + 1; i++) {
        coefficients[i] = multiplied_coefficients[i];
    }
}

/*after reading '+' at the start of the line reads characters until '\n'
converts the coefficients into numbers and adds them into the corresponding elements of an array (declared as coefficients)
the exponents are defined by analysing the characters following 'x' (if there is no x, the exponent doesn't change, it's equal to zero)
after adding the coefficient (number) to the element of the array (coefficients[exponent]) 
sets the value of both number and exponent to zero to be able to start the process again*/
void add_polynomials(int *current_char, int *coefficients) {
    int negative = false;
    int number = 0;
    int exponent = 0;

    while (*current_char != '\n') {
        if (*current_char == 'x') { //after reading an 'x' runs get_exponent()
            exponent = get_exponent(current_char);
            if (number == 0 && negative) { //if there is an 'x' with no visible coefficient and the negative is set to true, the coefficient is -1
                number = -1;
                negative = false;
            }
            else if (number == 0) { //if there is an 'x' with no visible coefficient and the negative is set to false, the coefficient is 1
                number = 1;
            }
            coefficients[exponent] += number;
            exponent = 0;
            number = 0;
        }
        else if (*current_char >= '0' && *current_char <= '9') { //runs get_number() after reading a digit
            number = get_number(current_char, &negative);
        }
        else if (*current_char == '-') { //sets negative to true after reading a '-'
            negative = true;
            number = -1;
            *current_char = getchar();
        }
        else if (*current_char == '+') { //sets negative back to false after reading a '+', resets number to 0
            negative = false;
            number = 0;
            *current_char = getchar();
        }
        else { //when there is ' ' omit and continue
            *current_char = getchar();
        }
    }
    coefficients[0] += number; //adds the constant, since the constant is not followed by an 'x'

    print_polynomials(coefficients); //prints the polynomial using the correct syntax
}

/*after reading '*' at the start of the line reads characters until '\n'
converts the coefficients into numbers and defines their coefficients,
then multiplies the array of (coefficients) by the term and adds each of the terms
into an array of (multiplied_polynomials)*/
void multiply_polynomials(int *current_char, int *coefficients) {
    int negative = false;
    int number = 0;
    int exponent = 0;
    int multiplied_coefficients[MAXIMUM_DEGREE + 1];

    empty_arrays(multiplied_coefficients);

    while (*current_char != '\n') {
        if (*current_char == 'x') { //after reading an 'x' runs get_exponent()
            exponent = get_exponent(current_char);
            if (number == 0 && negative) { //if there is an 'x' with no visible coefficient and the negative is set to true, the coefficient is -1
                number = -1;
                negative = false;
            }
            else if (number == 0) { //if there is an 'x' with no visible coefficient and the negative is set to false, the coefficient is 1
                number = 1;
            }
            for (int i = 0; i + exponent < MAXIMUM_DEGREE + 1; i++) { //multiplies the term by each term of the previous polynomial (coefficients)
                multiplied_coefficients[i + exponent] += coefficients[i] * number; //then adds to the new polynomial (multiplied_coefficients)
            }
            exponent = 0;
            number = 0;
        }
        else if (*current_char >= '0' && *current_char <= '9') { //runs get_number() after reading a digit
            number = get_number(current_char, &negative);
        }
        else if (*current_char == '-') { //sets negative to true after reading a '-'
            number = -1;
            negative = true;
            *current_char = getchar();
        }
        else if (*current_char == '+') { //sets negative back to false after reading a '+', resets number to 0
            number = 0;
            *current_char = getchar();
        }
        else { //when there is ' ' omit and continue
            *current_char = getchar();
        }
    }
    for (int i = 0; i < MAXIMUM_DEGREE + 1; i++) { //multiplies the constant by each term of the previous polynomial (coefficients)
        multiplied_coefficients[i] += coefficients[i] * number; //then adds to the new polynomial (multiplied_coefficients)
    }

    transform_arrays(multiplied_coefficients, coefficients); //overwrites the current array of coefficients with the multiplied coefficients
    print_polynomials(coefficients); //prints the polynomial using the correct syntax
}

void print_polynomials(int *coefficients) {
    int absolute_value;
    int is_first_term = true;

    for (int i = MAXIMUM_DEGREE; i >= 0; i--) { //prints terms starting from the one with the highest exponent
        if (coefficients[i] != 0) { //checks if the term should be printed
            if (is_first_term) { //prints the first term without the '+' before it
                is_first_term = false;
                if (i == 0) {
                    printf("%d", coefficients[i]);
                }
                else if (i == 1) {
                    if (coefficients[i] == 1) { 
                        printf("x");
                    }
                    else if (coefficients[i] == -1) {
                        printf("-x");
                    }
                    else {
                        printf("%dx", coefficients[i]);
                    }
                }
                else if (coefficients[i] == 1) {
                    printf("x^%d", i);
                }
                else if (coefficients[i] == -1) {
                    printf("-x^%d", i);
                }
                else {
                    printf("%dx^%d", coefficients[i], i);
                }
            }
            else {
                if (i == 0) {
                    if (coefficients[i] > 0) {
                        printf(" + %d", coefficients[i]);
                    }
                    else {
                        absolute_value = coefficients[i];
                        absolute_value = get_absolute_value(absolute_value);
                        printf(" - %d", absolute_value);
                    }
                }
                else if (i == 1) {
                    if (coefficients[i] == 1) {
                        printf(" + x");
                    }
                    else if (coefficients[i] == -1) {
                        printf(" - x");
                    }
                    else if (coefficients[i] < 0) {
                        absolute_value = coefficients[i];
                        absolute_value = get_absolute_value(absolute_value);
                        printf(" - %dx", absolute_value);
                    }
                    else if(coefficients[i] > 0) {
                        printf(" + %dx", coefficients[i]);
                    }
                }
                else if (coefficients[i] == 1) {
                    printf(" + x^%d", i);
                }
                else if (coefficients[i] == -1) {
                    printf(" - x^%d", i);
                }
                else if (coefficients[i] < 0) {
                    absolute_value = coefficients[i];
                    absolute_value = get_absolute_value(absolute_value);
                    printf(" - %dx^%d", absolute_value, i);
                }
                else if (coefficients[i] > 0) {
                    printf(" + %dx^%d", coefficients[i], i);
                }
            }
        }
    }
    if (is_first_term) { //if the constant is the first term and is equal to 0, prints 0
        printf("%d", coefficients[0]);
    }
    printf("\n");
}

int get_exponent(int *current_char) {
    int exponent = 1; //since there was an 'x', the exponent is not smaller than 1
    int negative = false;
    while (*current_char != '+' && *current_char != '-' && *current_char != '\n') { //reads chars after 'x' until '^', '\n' or the start of a new term
        if (*current_char == '^') {
            while (*current_char < '0' || *current_char > '9') { //ignores white characters
                *current_char = getchar();
            }
            exponent = get_number(current_char, &negative);
        }
        else {
            *current_char = getchar();
        }
    }
    return exponent;
}

int get_number(int *current_char, int *negative) {
    int number = 0;
    while (*current_char >= '0' && *current_char <= '9') { //reads digits of the number
        int new_digit; 
        new_digit = *current_char - '0'; //converts the character into a digit
        number = number * 10 + new_digit; //number * 10 since we're converting it to a decimal number
        *current_char = getchar();
    }
    if (*negative) { //converts the number to negative if needed
        *negative = false;
        number = number * (-1);
    }
    return number;
}

int get_absolute_value(int number) {
    if (number < 0) number = number * (-1);
    return number;
}

int main(void) {
    int coefficients[MAXIMUM_DEGREE + 1];
    int current_char = 0;

    empty_arrays(coefficients);

    while (current_char != '.') {
        current_char = getchar();
        if (current_char == '*') {
            multiply_polynomials(&current_char, coefficients);
        }
        if (current_char == '+') {
            add_polynomials(&current_char, coefficients);
        }
    }
    return 0;
}
