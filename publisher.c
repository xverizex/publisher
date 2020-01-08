#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef void (*subs) (void *event, void *data);

static struct publisher {
	int *types;
	int size;
	struct sub {
		int size;
		subs *subscribe;
		void **data;
	} *sub;
} *pb;

void init_publisher ( int type, void (*subscribe) ( void *event, void *data ), void *data ) {
	if ( !pb ) {
		pb = calloc ( 1, sizeof ( struct publisher ) );
		pb->types = calloc ( 0, sizeof ( int ) );
		pb->sub = calloc ( 0, sizeof ( struct sub ) );
	}	
	for ( int i = 0; i < pb->size; i++ ) {
		if ( pb->types[i] != type ) continue;

		pb->sub[i].subscribe = realloc ( pb->sub[i].subscribe, sizeof ( subs ) * ( pb->sub[i].size + 1 ) );
		pb->sub[i].data = realloc ( pb->sub[i].data, sizeof ( void * ) * ( pb->sub[i].size + 1 ) );
		int size = pb->sub[i].size;
		pb->sub[i].subscribe[size] = subscribe;
		pb->sub[i].data[size] = data;
		pb->sub[i].size++;
		return;
	}
	pb->types = realloc ( pb->types, sizeof ( int ) * ( pb->size + 1 ) );
	pb->sub = realloc ( pb->sub, sizeof ( struct sub ) * ( pb->size + 1 ) );
	pb->sub[pb->size].subscribe = calloc ( 1, sizeof ( subs ) );
	pb->sub[pb->size].data = calloc ( 1, sizeof ( void * ) );
	pb->types[pb->size] = type;
	pb->sub[pb->size].subscribe[0] = subscribe;
	pb->sub[pb->size].data[0] = data;
	pb->sub[pb->size].size = 1;
	pb->size++;
}

void delete_publisher ( int type, void (*subscribe) ( void *event, void *data ) ) {
	for ( int i = 0; i < pb->size; i++ ) {
		if ( pb->types[i] != type ) continue;

		for ( int ii = 0; ii < pb->sub[i].size; ii++ ) {
			if ( pb->sub[i].subscribe[ii] == subscribe ) {
				int size = pb->sub[i].size;
				int pos = size == ii - 1 ? ii : ii + 1;
				int ss = size == ii - 1 ? 0 : size - ii;
				int dst = ii;
				int src = pos;
				for ( int iii = pos; iii < size; iii++ ) {
					pb->sub[i].subscribe[dst] = pb->sub[i].subscribe[src];
					pb->sub[i].data[dst], pb->sub[i].data[src];
					dst++; src++;
				}
				size = --pb->sub[i].size;
				pb->sub[i].subscribe = realloc ( pb->sub[i].subscribe, size * sizeof ( subs ) );
				pb->sub[i].data = realloc ( pb->sub[i].data, size * sizeof ( void * ) );
				return;
			}
		}
	}
}

void send_event ( int type, void *event ) {
	for ( int i = 0; i < pb->size; i++ ) {
		if ( pb->types[i] != type ) continue;

		for ( int ii = 0; ii < pb->sub[i].size; ii++ ) {
			pb->sub[i].subscribe[ii] ( event, pb->sub[i].data[ii] );
		}
		return;
	}
}
