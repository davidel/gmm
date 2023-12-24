/*    Copyright 2023 Davide Libenzi
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * 
 */


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>



static void do_overflow(char *str) {
	char buf[32];

	sprintf(buf, "%s", str);
	strcpy(buf, str);
}

static int run_test(char *str) {

	do_overflow(str);

	return 0;
}

int main(int argc, char **argv) {

	run_test(argv[1]);

	return 0;
}

