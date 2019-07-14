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

#include <stdio.h>
#include <stdlib.h>
#include "repost.h"

int
main(void)
{
	char *str;
	struct message msg;
	str = "https://gnupluslinux.com/~daniel/7836923817.png this is a string with a url https://i.4cdn.org/g/1556161890444.png in it https://www.youtube.com/watch?v=4mi0kLLLAOs";
	printf("# urls: %d\n", get_url_count(str));
	msg.channel = "#test";
	msg.str = str;
	message_posted(&msg);
	return 0;
}
