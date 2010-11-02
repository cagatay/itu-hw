/*
 * Listing 1 -- coder.h
 *
 * This header file contains the constants, declarations, and
 * prototypes needed to use the arithmetic coding routines.  These
 * declarations are for routines that need to interface with the
 * arithmetic coding stuff in coder.c
 *
 */

#define MAXIMUM_SCALE   16383  /* Maximum allowed frequency count */
#define ESCAPE          256    /* The escape symbol               */
#define DONE            -1     /* The output stream empty  symbol */
#define FLUSH           -2     /* The symbol to flush the model   */

/*
 * A symbol can either be represented as an int, or as a pair of
 * counts on a scale.  This structure gives a standard way of
 * defining it as a pair of counts.
 */
typedef struct {
                unsigned short int low_count;
                unsigned short int high_count;
                unsigned short int scale;
               } SYMBOL;

extern int underflow_bits;    /* The present underflow count in  */
                               /* the arithmetic coder.           */
extern unsigned short int code;  /* The present input code value       */
extern unsigned short int low;   /* Start of the current code range    */
extern unsigned short int high;

/*
 * Function prototypes.
 */
void initialize_arithmetic_decoder();
void remove_symbol_from_stream( SYMBOL *s );
void initialize_arithmetic_encoder();
void encode_symbol( SYMBOL *s );
void flush_arithmetic_encoder(  );
short int get_current_count( SYMBOL *s );

