/*******************************************************************//**
 *	@file	hsv2rgb.c
 *
 *	@brief	HSV to RGB implementation(s) …
 *
 *	@author	Alexander Dahl <post@lespocky.de>
 *
 *	@copyright	2013 Alexander Dahl
 *
 *	This file is part of hsv2rgb.
 *
 *	_hsv2rgb_ is free software: you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	_hsv2rgb_ is distributed in the hope that it will be useful, but
 *	WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *	General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with _hsv2rgb_. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#include "hsv2rgb.h"

#include <stdint.h>
#include <stdlib.h>

const uint8_t buckets_8[6] = { 0, 43, 86, 128, 171, 214 };
const uint16_t buckets_10[6] = { 0, 171, 342, 512, 683, 854 };
uint8_t bsize_8[6];

uint8_t f8( uint8_t h ) {
	uint8_t hi = hi8( h );

	return (uint16_t) (h - buckets_8[hi]) * 255 / bsize_8[hi];
}

uint8_t hi8( uint8_t h ) {
	if ( h < buckets_8[3] ) {
		if ( h < buckets_8[1] ) {
			return 0;
		} else if ( h < buckets_8[2] ) {
			return 1;
		} else {
			return 2;
		}
	} else {
		if ( h < buckets_8[4] ) {
			return 3;
		} else if ( h < buckets_8[5] ) {
			return 4;
		} else {
			return 5;
		}
	}
}

uint8_t hi10( uint16_t h ) {
	return -1;
}

void init8( void ) {
    uint8_t lpc;
    for ( lpc = 0; lpc < 5; lpc++ ) {
        bsize_8[lpc] = buckets_8[lpc+1] - buckets_8[lpc] - 1;
    }
    bsize_8[5] = 256 - buckets_8[5] - 1;
}

uint8_t p8( uint8_t v, uint8_t s ) {
	return (uint16_t) v * ( 255 - s ) / 255;
}

uint8_t q8( uint8_t v, uint8_t s, uint8_t f ) {
	return (uint16_t) v * ( 255 - ( ( (uint16_t) s * (uint16_t) f ) / 255 ) ) / 255;
}

void rgb( uint8_t h, uint8_t s, uint8_t v,
		uint8_t *r, uint8_t *g, uint8_t *b )
{
	uint8_t f, hi;

	hi = hi8( h );
	f = f8( hi );

	switch ( hi ) {
	case 0:
		*r = v;
		*g = t8( v, s, f );
		*b = p8( v, s );
		break;
	case 1:
		*r = q8( v, s, f );
		*g = v;
		*b = p8( v, s );
		break;
	case 2:
		*r = p8( v, s );
		*g = v;
		*b = t8( v, s, f );
		break;
	case 3:
		*r = p8( v, s );
		*g = q8( v, s, f );
		*b = v;
		break;
	case 4:
		*r = t8( v, s, f );
		*g = p8( v, s );
		*b = v;
		break;
	case 5:
		*r = v;
		*g = p8( v, s );
		*b = q8( v, s, f );
		break;
	default:
		/*	not reached	*/
		break;
	}
}

uint8_t t8( uint8_t v, uint8_t s, uint8_t f ) {
	return (uint16_t) v * ( 255 - ( ( (uint16_t) s * (255 - f) ) / 255 ) ) / 255;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
