/*
 * Listing 2 -- coder.c
 *
 * This file contains the code needed to accomplish arithmetic
 * coding of a symbol.  All the routines in this module need
 * to know in order to accomplish coding is what the probabilities
 * and scales of the symbol counts are.  This information is
 * generally passed in a SYMBOL structure.
 *
 * This code was first published by Ian H. Witten, Radford M. Neal,
 * and John G. Cleary in "Communications of the ACM" in June 1987,
 * and has been modified slightly for this article.  The code
 * is  published here with permission.
 */

#include <stdio.h>
#include "coder.h"
#include "bitio.h"
/*
 * This routine is called to encode a symbol.  The symbol is passed
 * in the SYMBOL structure as a low count, a high count, and a range,
 * instead of the more conventional probability ranges.  The encoding
 * process takes two steps.  First, the values of high and low are
 * updated to take into account the range restriction created by the
 * new symbol.  Then, as many bits as possible are shifted out to
 * the output stream.  Finally, high and low are stable again and
 * the routine returns.
 */
void encode_symbol( SYMBOL *s )
{
    long range;
/*
 * These three lines rescale high and low for the new symbol.
 */
    range = (long) ( high-low ) + 1;
    high = low + (unsigned short int )
                 (( range * s->high_count ) / s->scale - 1 );
    low = low + (unsigned short int )
                 (( range * s->low_count ) / s->scale );
/*
 * This loop turns out new bits until high and low are far enough
 * apart to have stabilized.
 */
    for ( ; ; )
    {
/*
 * If this test passes, it means that the MSDigits match, and can
 * be sent to the output stream.
 */
        if ( ( high & 0x8000 ) == ( low & 0x8000 ) )
        {
            output_bit( high & 0x8000 );
            while ( underflow_bits > 0 )
            {
                output_bit( ~high & 0x8000 );
                underflow_bits--;
            }
        }
/*
 * If this test passes, the numbers are in danger of underflow, because
 * the MSDigits don't match, and the 2nd digits are just one apart.
 */
        else if ( ( low & 0x4000 ) && !( high & 0x4000 ))
        {
            underflow_bits += 1;
            low &= 0x3fff;
            high |= 0x4000;
        }
        else
            return ;
        low <<= 1;
        high <<= 1;
        high |= 1;
    }
}

/*
 * At the end of the encoding process, there are still significant
 * bits left in the high and low registers.  We output two bits,
 * plus as many underflow bits as are necessary.
 */