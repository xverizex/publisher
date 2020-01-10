# publisher

Реализация подписчика и издателя на си. 

**как пользоваться**:

Укажи в ```enum``` номер издателя. добавить в ```init_publisher ( int type, void (*subscribe) (void *event, void *data), void *data );``` тип издателя и указатель на функцию, а также данные если хотите. На одного издателя можно подписать несколько функций.

Когда надо отправить нескольких подписчикам событие, вызовите ```send_event ( int type, void *event )```.

Чтобы отписаться от издателя вызовите ```delete_publisher ( int type, void (*subscribe) ( void *event, void *data ), void *data );``` укажите тип издателя и функцию, чтобы удалить её из подписки.


```
#include <stdio.h>
#include "publisher.h"

enum { TRADE, NEWS };

struct human {
	int number;
};

void sub_trade ( void *event, void *data ) {
	struct human *human = ( struct human * ) data;
	int money = ( int ) event;
	printf ( "%d получил %d\n", human->number, money );

}

void sub_news ( void *event, void *data ) {
	struct human *human = ( struct human * ) data;
	int news = ( int ) event;
	printf ( "%d читает %d\n", human->number, news );
}
int main ( ) {
	/* я создал две структуры, которые содержать некоторые данные */
	struct human human[4] = {
		{ 1 },
		{ 2 },
		{ 3 },
		{ 4 }
	};

	/* в эту функцию я передаю номер издателя.
	 * для удобства я использовал enum, чтобы обозначить издателя.
	 * во второй параметр я передаю указатель на функцию, это будет подписчик. с этого момента издатель TRADE будет в эту функцию
	 * отправлять события.
	 * в третий параметр я передаю структуру, какие нибудь пользовательские данные. эти данные будут использованы в подписчике.
	 */
	init_publisher ( TRADE, sub_trade, &human[0] );
	init_publisher ( TRADE, sub_trade, &human[1] );
	init_publisher ( TRADE, sub_trade, &human[2] );
	init_publisher ( TRADE, sub_trade, &human[3] );
	/* можно удалить подписчика с определенным указателем на данные. тогда один подписчик не будет существовать. */
	delete_publisher ( TRADE, sub_trade, &human[2] );
	/* можно удалить всех подписчиков указанных на данную функцию. */
	delete_all_subscribe ( TRADE, sub_trade );
	/* можно удалить абсолютно всех подписчиков, подписавшихся на TRADE например */
	delete_all_publisher ( TRADE );

	init_publisher ( NEWS, sub_news, &human[0] );

	for ( int i = 0; i < 100; i++ ) {
		/* здесь отправляет событие 10 издателя TRADE. оно будет отправлено в функцию sub_trade. */
		if ( i % 10 == 0 ) send_event ( TRADE, 10 );
		if ( i % 20 == 0 ) send_event ( NEWS, 1 );
	}

}
```
