#publisher

Реализация подписчика и издателя на си. 

**как пользоваться**:

Укажи в ```enum``` номер издателя. добавить в ```init_publisher ( int type, void (*subscribe) (void *event, void *data), void *data );``` тип издателя и указатель на функцию, а также данные если хотите. На одного издателя можно подписать несколько функций.

Когда надо отправить нескольких подписчикам событие, вызовите ```send_event ( int type, void *event )```.

Чтобы отписаться от издателя вызовите ```delete_publisher ( int type, void (*subscribe) ( void *event, void *data ) );``` укажите тип издателя и функцию, чтобы удалить её из подписки.