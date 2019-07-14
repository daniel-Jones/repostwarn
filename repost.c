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

#include "repost.h"

/*
 *
 * urls
 *
 */

int
is_link(char *url)
{
	/*
	 * terribly check if something has a "protocol://"
	 * if it does, we assume it is a link
	 */
	char *p = NULL;
	if ((p = strstr(url, "://")))
	{
		int s;
		s = (p-url);
		/*
		 * check if the number of characters before the '://' is >0 && <10 && contains a '.'
		 * 10 is a reasonable number plucked out of thin air, RFC 3986 does not state a maximum
		 * we would like to avoid false positives, not that it matters much
		 */
		if (s > 0 && s <= 10 && strchr(url, '.') != NULL)
			/* it's probably a link */
			return 1;
	}
	return 0;
}

int
get_url_count(char *str)
{
	/*
	 * returns number of urls found
	 * returns -1 on error
	 */
	char *token, *store;
	int urls;
	urls = 0;
	store = malloc(strlen(str)+1);
	if (!store)
		return -1;
	/* strtok mangles original pointer */
	token = store;
	token = strcpy(token, str);
	token = strtok(token, " ");
	while (token)
	{
		if (is_link(token))
			urls++;
		token = strtok(NULL, " ");
	}
	free(store);
	return urls;
}

struct url_node 
*get_urls(char *str)
{
	/*
	 * returns head to linked list of urls
	 * NULL on error
	 */
	 struct url_node *head;
	char *token, *store;
	head = NULL;
	store = malloc(strlen(str)+1);
	if (!store)
		return NULL;
	/* strtok mangles original pointer */
	token = store;
	token = strcpy(token, str);
	token = strtok(token, " ");
	while (token)
	{
		if (is_link(token))
		{
			append_url_node(&head, create_url_node(token));
		}
		token = strtok(NULL, " ");
	}
	free(store);
	return head;
}

/*
 *
 * url linked list
 *
 */

struct url_node
*create_url_node(char *url)
{
	struct url_node *n;
	char *s;
	n = malloc(sizeof(struct url_node));
	s = malloc(strlen(url)+1);
	if (!n || !s) {free(n);free(s);return NULL;}
	s = strcpy(s, url);
	n->url = s;
	n->next = NULL;
	return n;
}

void
append_url_node(struct url_node **head, struct url_node *node)
{
	struct url_node **finger;
	finger = head;
	while (*finger)
	{
		finger = &(*finger)->next;
	}
	node->next = *finger;
	*finger = node;
}

void
free_url_nodes(struct url_node *head)
{
	struct url_node *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->url);
		free(tmp);
	}
}

void
print_urls(struct url_node *head)
{
	struct url_node *finger;
	finger = head;
	while (finger)
	{
		puts(finger->url);
		finger = finger->next;
	}
}

/*
 *
 * database
 *
 */

sqlite3
*dbconnect(void)
{
	/*
	 * return an sqlite3 database structure
	 */
	 sqlite3 *db;
	 int rc;
	 rc = sqlite3_open("test.db", &db);
	 if (rc != SQLITE_OK)
	 {
		sqlite3_close(db);
		fprintf(stderr, "cannot open database\n");
		return NULL;
	 }
	 return db;
}

void
dbdisconnect(sqlite3 *db)
{
	 sqlite3_close(db);
}

sqlite3_stmt
*generic_statement(sqlite3 *db, char *stmt)
{
	sqlite3_stmt *res;
	int rc;
	rc = sqlite3_prepare_v2(db, stmt, -1, &res, 0);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "%s\n", sqlite3_errmsg(db));
		return NULL;
	}
	return res;
}

/*
 *
 * message functions
 *
 */

int
message_posted(struct message *msg)
{
	/*
	 *
	 */
	struct url_node *head, *finger;
	sqlite3 *db;
	db = NULL;
	head = get_urls(msg->str);
	if (!head)
	{
		return 0;
	}
	/* go through each url and check if posted */
	finger = head;
	while (finger)	
	{
		if (!db)
			db = dbconnect();
		is_url_posted(db, msg->channel, finger->url);
		finger = finger->next;
	}
	free_url_nodes(head);
	dbdisconnect(db);
	return 0;
}

int
insert_url(sqlite3 *db, struct message *msg)
{
	return 0;
}

int
is_url_posted(sqlite3 *db, char *channel, char *url)
{
	int rc;
	sqlite3_stmt *stmt;
	printf("stub check url %s in %s\n", url, channel);
	stmt = generic_statement(db, "SELECT SQLITE_VERSION()");
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		printf("%s\n", sqlite3_column_text(stmt, 0));
	}
	sqlite3_finalize(stmt);
	return 1;
}

/*
 *
 * general functions
 *
 */

 int
 url_hash(char *url)
 {
	return 0;
 }
