/*
 * init_publisher - инициализировать издателя. 
 * @type = это тип издателя. Например в enum указывает enum { TRADE, NEWS }, к каждому издателю можно подписаться нескольким функциям.
 * @subscribe = это функция указатель. в эту фукнцию издатель будет отправлять события и данные.
 */
void init_publisher ( int type, void (*subscribe) (void *event, void *data), void *data );
/*
 * отправить событие.
 * @type = если например в init_publisher вы задали TRADE, то в send_event в TRADE отправяться событие, которое вы укажите.
 * @event = событие. можно передать любые данные.
 */
void send_event ( int type, void *event );
/*
 * отписаться от издателя.
 * @type = тип издателя.
 * @subscribe = функция указатель. её надо указать, чтобы в этом типе удалить эту функцию от подписки.
 */
void delete_publisher ( int type, void (*subscribe) ( void *event, void *data ) );
