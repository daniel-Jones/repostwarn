/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REPOST_H
#define REPOST_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

struct url_node
{
	char *url; 
	struct url_node *next;
};

struct message
{
	char *channel;
	char *str;
};

int get_url_count(char *str); /* check if the string contains at least one url */
struct url_node *get_urls(char *str); /* create linked list of urls from string */
struct url_node *create_url_node(char *url);
void append_url_node(struct url_node **head, struct url_node *node);
void free_url_nodes(struct url_node *head);
void print_urls(struct url_node *head);
sqlite3 *dbconnect(void); /* connect to database */
void dbdisconnect(sqlite3 *db); /* disconnect from database */
sqlite3_stmt *generic_statement(sqlite3 *db, char *stmt); /* insert generic entry */
int message_posted(struct message *msg); /* handle a message */
int insert_url(sqlite3 *db, struct message *msg); /* insert url into database with channel */
int is_url_posted(sqlite3 *db, char *channel, char *url); /* check if url has been posted */
int url_hash(char *url);

#endif
