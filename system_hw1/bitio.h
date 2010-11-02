/*
 * Listing 3 -- bitio.h
 *
 * This header file contains the function prototypes needed to use
 * the bitstream i/o routines.
 *
 */
extern FILE *compressed_file;
short int input_bit();
void initialize_output_bitstream( void );
void output_bit( int bit );
void flush_output_bitstream(  );
void initialize_input_bitstream( void );
long bit_ftell_output( );
long bit_ftell_input( );
int divide(int, int);

