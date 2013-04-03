// A simple parser of arithmetic expressions.
// author: Osamu Kobayashi

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

enum expression_type {
    expression_type_number,
    expression_type_mult,
    expression_type_add
};

struct expression
{
    enum expression_type type;
    int value;
    char operator;
    struct expression *left_operand;
    struct expression *right_operand;
};

struct expression *parse_expression_add(
    const char input_buffer[],
    int *input_index
);

int is_space( char c )
{
    if( ' '  == c ) return 1;
    if( '\t' == c ) return 1;
    if( '\n' == c ) return 1;
    if( '\r' == c ) return 1;
    return 0;
}

void skip_space(
    const char input_buffer[],
    int *input_index
)
{
    for( ; is_space( input_buffer[*input_index] ); (*input_index)++ )
        ;
}

int is_number( char c )
{
    if( ('0' <= c) && (c <= '9') )
        return 1;
    return 0;
}

struct expression *parse_number(
    const char input_buffer[],
    int *input_index
)
{
    char output_buffer[128];
    int output_index = 0;
    struct expression *expr = 0;
    if( ('+' == input_buffer[ *input_index ]) ||
        ('-' == input_buffer[ *input_index ]) ){
        output_buffer[ output_index ++ ] = input_buffer[ (*input_index)++ ];
        skip_space( input_buffer, input_index );
    }
    if( is_number( input_buffer[ *input_index ] ) ){
        output_buffer[ output_index ++ ] = input_buffer[ (*input_index)++ ];
        for( ; is_number( input_buffer[ *input_index ] ); ){
            output_buffer[ output_index ++ ] = input_buffer[ (*input_index)++ ];
        }
        output_buffer[ output_index ++ ] = '\0';
        expr = (struct expression *)malloc( sizeof( struct expression ) );
        expr->type = expression_type_number;
        sscanf( output_buffer, "%d", &(expr->value) );
        expr->left_operand = 0;
        expr->right_operand = 0; 
        return expr;
    }
    return 0;
}

struct expression *parse_operand(
    const char input_buffer[],
    int *input_index
)
{
    struct expression *expr = 0;
    int index = *input_index;
    skip_space( input_buffer, &index );
    expr = parse_number( input_buffer, &index );
    if( 0 != expr ){
        *input_index = index;
        return expr;
    }
    if( '(' == input_buffer[ *input_index ] ){
        (*input_index) ++;
        skip_space( input_buffer, input_index );
        expr = parse_expression_add( input_buffer, input_index );
        if( 0 == expr ){
            fprintf( stderr, "no expression at index %d\n", *input_index );
            return 0;
        }
        skip_space( input_buffer, input_index );
        if( ')' != input_buffer[ *input_index ] ){
            fprintf( stderr, "no ')' at index %d\n", *input_index );
            return 0;
        }
        (*input_index) ++;
        return expr;
    }
    return 0;
}

struct expression *parse_expression_mult(
    const char input_buffer[],
    int *input_index
)
{
    struct expression *expr = 0;
    skip_space( input_buffer, input_index );
    expr = parse_operand( input_buffer, input_index );
    if( 0 == expr ){
        fprintf( stderr, "no expression at index %d\n", *input_index );
        return 0;
    }
    while( 1 ){
        struct expression *expr2 = 0;
        skip_space( input_buffer, input_index );
        if( ('*' != input_buffer[ *input_index ]) &&
            ('/' != input_buffer[ *input_index ]) )
            break;
        expr2 = malloc( sizeof( struct expression ) );
        expr2->type = expression_type_mult;
        expr2->operator = input_buffer[ (*input_index)++ ];
        expr2->left_operand = expr;
        skip_space( input_buffer, input_index );
        expr2->right_operand = parse_operand( input_buffer, input_index );
        if( 0 == expr2->right_operand ){
            fprintf( stderr, "no expression after operator '%c' at index %d\n",
                     expr2->operator, *input_index );
            return 0;
        }
        expr = expr2;
    }    
    return expr;
}

struct expression *parse_expression_add(
    const char input_buffer[],
    int *input_index
)
{
    struct expression *expr = 0;
    skip_space( input_buffer, input_index );
    expr = parse_expression_mult( input_buffer, input_index );
    if( 0 == expr ){
        fprintf( stderr, "no expression at index %d\n", *input_index );
        return 0;
    }
    while( 1 ){
        struct expression *expr2 = 0;
        skip_space( input_buffer, input_index );
        if( ('+' != input_buffer[ *input_index ]) &&
            ('-' != input_buffer[ *input_index ]) )
            break;
        expr2 = malloc( sizeof( struct expression ) );
        expr2->type = expression_type_add;
        expr2->operator = input_buffer[ (*input_index)++ ];
        expr2->left_operand = expr;
        skip_space( input_buffer, input_index );
        expr2->right_operand = parse_expression_mult( input_buffer, input_index );
        if( 0 == expr2->right_operand ){
            fprintf( stderr, "no expression after operator '%c' at index %d\n",
                     expr2->operator, *input_index );
            return 0;
        }
        expr = expr2;
    }    
    return expr;
}

struct expression *parse_expression(
    const char input_buffer[]
)
{
    int index = 0;
    return parse_expression_add( input_buffer, &index );
}

void print_expression( struct expression *expr )
{
    if( 0 == expr ){
        printf( "no expression\n" );
    }
    else if( expression_type_number == expr->type ){
        printf( "%d", expr->value );
    }
    else{
        printf( "(" );
        print_expression( expr->left_operand );
        printf( " %c ", expr->operator );
        print_expression( expr->right_operand );
        printf( ")" );
    }
}

int evaluate_expression( struct expression *expr )
{
    if( 0 == expr ){
        printf( "no expression\n" );
        return 0;
    }
    else if( expression_type_number == expr->type ){
        return expr->value;
    }
    else{
        int left_value = evaluate_expression( expr->left_operand );
        int right_value = evaluate_expression( expr->right_operand );
        switch( expr->operator ){
          case '+':
            return left_value + right_value;
          case '-':
            return left_value - right_value;
          case '*':
            return left_value * right_value;
          case '/':
            return left_value / right_value;
          default:
            return 0;
        }
    }
}

void free_expression( struct expression *expr )
{
    if( (expression_type_add == expr->type) ||
        (expression_type_mult == expr->type) ){
        free_expression( expr->left_operand );
        free_expression( expr->right_operand );
    }
    free( expr );
}

