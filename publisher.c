/*
 * publisher - реализация подобия паттерна наблюдателя
 *
 * Copyright (C) 2020 Naidolinsky Dmitry <naidv88@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY of FITNESS for A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------/
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef void (*subs) (void *event, void *data);

static struct publisher {
	int *types;
	int size;
	struct sub {
		int type;
		int size;
		subs *subscribe;
		void **data;
	} *sub;
} *pb;

static int cmpsub ( const void *p1, const void *p2 ) {
	int type_p1 = ( int ) ( ( struct sub * ) p1 )->type;
	int type_p2 = ( int ) ( ( struct sub * ) p2 )->type;
	return type_p1 > type_p2;
}
static int cmptypes ( const void *p1, const void *p2 ) {
	int types_p1 = *( ( int * ) p1 );
	int types_p2 = *( ( int * ) p2 );
	return types_p1 > types_p2;
}

static int search_bin ( const int type ) {
	int low, high, middle;
	low = 0;
	high = pb->size - 1;
	while ( low <= high ) {
		middle = ( low + high ) / 2;
		if ( type < pb->types[middle] ) high = middle - 1;
		else if ( type > pb->types[middle] ) low = middle + 1;
		else return middle;
	}
	return -1;
}

void init_publisher ( int type, void (*subscribe) ( void *event, void *data ), void *data ) {
	if ( !pb ) {
		pb = calloc ( 1, sizeof ( struct publisher ) );
		pb->types = calloc ( 0, sizeof ( int ) );
		pb->sub = calloc ( 0, sizeof ( struct sub ) );
	}	
	do {
		int i = search_bin ( type );
		if ( i == -1 ) break;

		pb->sub[i].subscribe = realloc ( pb->sub[i].subscribe, sizeof ( subs ) * ( pb->sub[i].size + 1 ) );
		pb->sub[i].data = realloc ( pb->sub[i].data, sizeof ( void * ) * ( pb->sub[i].size + 1 ) );
		int size = pb->sub[i].size;
		pb->sub[i].subscribe[size] = subscribe;
		pb->sub[i].data[size] = data;
		pb->sub[i].size++;

		return;
	} while ( 0 );

	pb->types = realloc ( pb->types, sizeof ( int ) * ( pb->size + 1 ) );
	pb->sub = realloc ( pb->sub, sizeof ( struct sub ) * ( pb->size + 1 ) );
	pb->sub[pb->size].type = type;
	pb->sub[pb->size].subscribe = calloc ( 1, sizeof ( subs ) );
	pb->sub[pb->size].data = calloc ( 1, sizeof ( void * ) );
	pb->types[pb->size] = type;
	pb->sub[pb->size].subscribe[0] = subscribe;
	pb->sub[pb->size].data[0] = data;
	pb->sub[pb->size].size = 1;
	pb->size++;

	qsort ( &pb->sub[0], pb->size, sizeof ( struct sub ), cmpsub );
	qsort ( &pb->types[0], pb->size, sizeof ( int ), cmptypes );

}

void delete_publisher ( int type, void (*subscribe) ( void *event, void *data ), void *data ) {
	int i = search_bin ( type );
	if ( i == -1 ) return;

	for ( int ii = 0; ii < pb->sub[i].size; ii++ ) {
		if ( pb->sub[i].subscribe[ii] == subscribe && pb->sub[i].data[ii] == data ) {
			int size = pb->sub[i].size;
			int pos = size == ii - 1 ? ii : ii + 1;
			int ss = size == ii - 1 ? ii - 1 : ii;
			int dst = ss;
			int src = pos;
			for ( int iii = pos; iii < size; iii++ ) {
				pb->sub[i].subscribe[dst] = pb->sub[i].subscribe[src];
				pb->sub[i].data[dst] = pb->sub[i].data[src];
				dst++; src++;
			}
			size = --pb->sub[i].size;
			pb->sub[i].subscribe = realloc ( pb->sub[i].subscribe, size * sizeof ( subs ) );
			pb->sub[i].data = realloc ( pb->sub[i].data, size * sizeof ( void * ) );
			ii--;
		}
	}
}

void delete_all_publisher ( int type ) {
	int i = search_bin ( type );
	if ( i == -1 ) return;

	pb->sub[i].size = 0;
	pb->sub[i].subscribe = realloc ( pb->sub[i].subscribe, 0 );
	pb->sub[i].data = realloc ( pb->sub[i].data, 0 );
}
void delete_all_subscribe ( int type, void (*subscribe) ( void *event, void *data ) ) {
	int i = search_bin ( type );
	if ( i == -1 ) return;

	for ( int ii = 0; ii < pb->sub[i].size; ii++ ) {
		if ( pb->sub[i].subscribe[ii] == subscribe ) {
			int size = pb->sub[i].size;
			int pos = size == ii - 1 ? ii : ii + 1;
			int ss = size == ii - 1 ? ii - 1 : ii;
			int dst = ss;
			int src = pos;
			for ( int iii = pos; iii < size; iii++ ) {
				pb->sub[i].subscribe[dst] = pb->sub[i].subscribe[src];
				pb->sub[i].data[dst] = pb->sub[i].data[src];
				dst++; src++;
			}
			size = --pb->sub[i].size;
			pb->sub[i].subscribe = realloc ( pb->sub[i].subscribe, size * sizeof ( subs ) );
			pb->sub[i].data = realloc ( pb->sub[i].data, size * sizeof ( void * ) );
			ii--;
		}
	}
}

void send_event ( int type, void *event ) {
	int i = search_bin ( type );
	if ( i == -1 ) return;

	for ( int ii = 0; ii < pb->sub[i].size; ii++ ) {
		pb->sub[i].subscribe[ii] ( event, pb->sub[i].data[ii] );
	}
}
