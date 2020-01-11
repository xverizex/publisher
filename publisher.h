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
#ifndef __PUBLISHER_H
#define __PUBLISHER_H
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
 * @data = указатель на данные. удалиться подписчик именно с этими данными.
 */
void delete_publisher ( int type, void (*subscribe) ( void *event, void *data ), void *data );
/*
 * удалить всех подписчиков связанных с данной функцией. */
void delete_all_subscribe ( int type, void (*subscribe) ( void *event, void *data ) );
/* удалить абсолютно всех подписчиков привязанных к определенному издателю. */
void delete_all_publisher ( int type );
#endif
