/*



ICS 53 -- Lab 3 (Malloc)

Names						ID#
Christopher Christian		52697582
Kelly Zhao					77719128

*/



#include <string.h>
#include <stdio.h>
int heapsize = 128;
int block_num = 0;
int  blocks_alloc = 0;
 unsigned char * heap;
unsigned char* current_block;
unsigned char bsize, check_alloc_status;


printheap(int block_num, int read_num) {
	int flag = 1;
	char *cb = heap;
	int i, j;
	if (read_num >= 127) {
		printf("CANNOT READ BEYOND MEMORY BLOCK\n");
		return;
	}
	for (i = 0; i < blocks_alloc; i++) {
		check_alloc_status = (cb[1] & 0x01);
		if ((int)check_alloc_status == 1) {
			bsize = (cb[1] >> 1);
		}
		else {
			bsize = (cb[1] >> 1);
		}
		if (cb[0] == block_num) {

			if ((int)check_alloc_status == 1) {
				flag = 0;
				if (read_num > cb[1] >> 1) {
					printf("CANNOT READ BEYOND BLOCK\n");
					return;
				}
				for (j = 2; j < (read_num + 2); j++) {
					printf("%c", cb[j]);
				}
				printf("\n");
				break;
			}
			else {
				flag = 0;
				printf("BLOCK NOT ALLOCATED \n");
			}

		}
		cb = cb + (int)bsize;
	}
	if (flag != 0) {
		printf("BLOCK NOT FOUND \n");
	}




}

writeheap(int block_num, char charwrite, int copies) {
	int flag = 1;
	char *cb = heap;
	int i, j;
	for (i = 0; i < blocks_alloc; i++) {
		check_alloc_status = (cb[1] & 1);
		if ((int)check_alloc_status == 1) {
			bsize = (cb[1] >> 1);
		}
		else {
			bsize = (cb[1] >> 1);
		}
		if (cb[0] == block_num) {
			if ((int)check_alloc_status == 1) {
				flag = 0;
				if ((bsize - 2) >= copies) {
					for (j = 2; j < (copies + 2); j++) {
						cb[j] = charwrite;
					}
				}
				else {
					flag = 0;
					printf("WRITE TOO BIG\n");
				}

			}
			break;
		}
		cb = cb + (int)bsize;
	}

	if (flag != 0)
	{
		printf("BLOCK NOT FOUND \n");
	}
}

Free(int block_num) {
	char * cb = heap;
	int k = 0;
	int flag = 1;
	for (k = 0; k < blocks_alloc; k++) {
		check_alloc_status = (cb[1] & 0x01);
		if (cb[0] == block_num) {
			if ((int)check_alloc_status == 1) {
				flag = 0;
				printf(" %d\n", cb[0]);
				cb[1] = cb[1] ^ 0x01;
				break;
			}
			else {
				flag = 0;
				printf("BLOCK NOT ALLOCATED\n");
				break;
			}


		}
		cb = cb + (cb[1] >> 1);
	}

	if (flag != 0) {
		printf("BLOCK NOT FOUND\n");
	}
	
	



}



blocklist() {
	
	unsigned char * cb = heap;
	char* allocated;
	int i;
	printf("Size Allocated  Start		End\n");
	for (i = 0; i < blocks_alloc; i++) {
		if (cb[1] == 129) {
			bsize = cb[1] ^ 0x01;
		}
		else if (cb[1] == 128) {
			bsize = cb[1];
		}
		else {
			bsize = cb[1] >> 1;
		}
			allocated = "no";
		

		check_alloc_status = (cb[1] & 1);

		if ((int)check_alloc_status == 1) {
			allocated = "yes";
		}

		int int_size = (int)bsize;
		
	
			printf("%d\t%s\t%d\t\t%d\n", int_size, allocated, cb, (cb + int_size));
		

		cb = cb + int_size;
	}

	if (heapsize > 0) {
		allocated = "no";
		printf("%d\t%s\t%d\t\t%d\n", heapsize, allocated, cb, (cb + heapsize));
	}

}





printheader(int block_num) {
	char * cb = heap;
	int flag = 0;
	int i;
	for (i = 0; i < blocks_alloc; i++) {
		
		if (cb[0] == block_num) {
			flag = 1;
			if (cb[1] == 129) {
				printf("%02x%02x\n", cb[0], cb[1] - 1);
				break;
			}
			printf("%02x%02x\n", cb[0], (cb[1] ^ 0x01) -1);
			break;
		}
		cb = cb + (int)bsize;
	}

	if (flag <= 0) {
		printf("HEADER NOT FOUND\n");
	}

}

allocate(int size) {
	unsigned header_byte_2;
	if (size >= 127) {
		printf("ERROR: ALLOCATION TOO BIG\n");
		return;
	} current_block = heap;
	unsigned int true_size = size + 2;
	

	int flag = 1;
	int i;
	int alloc_status;
	if (heapsize >= true_size){
	
	if (blocks_alloc >= 1) {
		for (i = 0; i < blocks_alloc; i++) {
			alloc_status = current_block[1] & 1;

			if (alloc_status == 0) {
				if (current_block[1] >> 1 > true_size) {
					block_num++;
					flag = 0;

					int diff = (current_block[1] >> 1) - true_size;

					if (diff == 1 || diff == 2) {

						true_size += diff;
					}
					else {
						blocks_alloc++;
						char next_block_header_1 = 0;
						char next_block_header_2 = (diff << 1);
						current_block[true_size] = next_block_header_1;
						current_block[true_size + 1] = next_block_header_2;
					}
					char header_byte_1 = (char)block_num;
					char header_byte_2 = (true_size << 1) | 0x01;
					current_block[0] = header_byte_1;
					current_block[1] = header_byte_2;
					break;
				}
				else if (current_block[1] >> 1 == true_size && flag) {
					block_num++;
					flag = 0;
					char header_byte_1 = (char)block_num;
					char header_byte_2 = (true_size << 1) | 0x01;
					current_block[0] = header_byte_1;
					current_block[1] = header_byte_2;
					break;
				}
			}
			else {
				current_block = current_block + (current_block[1] >> 1);
			}


		}

	}
	if (flag == 0) {
		printf("%d\n", block_num);

	}
	else if (flag == 1) {
		int diff = heapsize - true_size;

		if (diff == 1 || diff == 2) {

			true_size += diff;
		}
	
		block_num++;
		blocks_alloc++;
		heapsize -= true_size;
		 unsigned char header_byte_1 = (char)block_num;
		
		 if (true_size == 128) {
			
			 header_byte_2 = true_size | 0x01;
		 }
		 else {
			 header_byte_2 = (true_size << 1) | 0x01;
		 }
		

		current_block[0] = header_byte_1;
		current_block[1] = header_byte_2;

		printf("%d\n", block_num);


	}
}
	else {
		printf("CANNOT ALLOCATE -- NOT ENOUGH SPACE\n");
	}
}


quit() {
	free(heap);
	exit(0);
}
int main() {
	heap = malloc(128);
	current_block = heap;






	char user_input[1000];
	printf(">");
	char * split;

	while (strcmp("quit", gets(user_input)) != 0) {
		
		split = strtok(user_input, " ");

		if (strcmp(split, "allocate") == 0) {
			char * temp;
			int value;
			temp = strtok(NULL, " ");

			value = atoi(temp);
			allocate(value);

		}
		else if (strcmp(split, "free") == 0) {
			char * temp;
			int value;
			temp = strtok(NULL, " ");
			value = atoi(temp);
			Free(value);

		}
		else if (strcmp(split, "blocklist") == 0) {
			blocklist();
		}
		else if (strcmp(split, "writeheap") == 0) {
			char * arguments[3];
			char * value = strtok(NULL, " ");

			int i = 0;
			int j;
			int count = 0;
			int block_num_flag = 0;
			int write_char_flag = 0;
			int num_copies_flag = 0;

			while (value != NULL) {
				count++;
				arguments[i++] = value;
				value = strtok(NULL, " ");
			}

			if (count != 3) {
				printf("%s\n", "Incorrect number of arguments.");
			}
			if (count == 3) {
				for (j = 0; j < 3; j++) {
				}
				int i;

				for (i = 0; i < strlen(arguments[0]); i++) {
					if (isdigit(arguments[0][i]))
						block_num_flag = 1;
					else
						block_num_flag = 0;
				}

				for (i = 0; i < strlen(arguments[1]); i++) {
					if (isalpha(arguments[1][i]))
						write_char_flag = 1;
					else
						write_char_flag = 0;
				}

				for (i = 0; i < strlen(arguments[2]); i++) {
					if (isdigit(arguments[2][i]))
						num_copies_flag = 1;
					else
						num_copies_flag = 0;
				}
				int arg_1 = atoi(arguments[0]);
				int arg_3 = atoi(arguments[2]);



				if (block_num_flag == 1 && write_char_flag == 1 && num_copies_flag == 1) {
					char to_write = arguments[1][0];
					writeheap(arg_1, to_write, arg_3);

				}
				else {
					printf("%s\n", "Incorrect argument types passed.");
				}
			}

		}

		else if (strcmp(split, "printheap") == 0) {
			char * arguments[2];
			char * value = strtok(NULL, " ");
			int count = 0;
			int i = 0;
			int j;
			int block_num_flag = 0;
			int bytes_flag = 0;

			while (value != NULL) {
				count++;
				arguments[i++] = value;
				value = strtok(NULL, " ");
			}
			if (count != 2) {
				printf("%s\n", "Incorrect number of arguments.");
			}

			if (count == 2) {
				for (j = 0; j < 3; j++) {
				}
				int i;

				for (i = 0; i < strlen(arguments[0]); i++) {
					if (isdigit(arguments[0][i]))
						block_num_flag = 1;
					else
						block_num_flag = 0;
				}
				for (i = 0; i < strlen(arguments[1]); i++) {
					if (isdigit(arguments[1][i]))
						bytes_flag = 1;
					else
						bytes_flag = 0;
				}

				int arg_1 = atoi(arguments[0]);
				int arg_2 = atoi(arguments[1]);

				if (block_num_flag == 1 && bytes_flag == 1) {
					printheap(arg_1, arg_2);
				}
				else {
					printf("%s\n", "Incorrect argument types passed.");
				}
			}

		}
		else if (strcmp(split, "printheader") == 0) {
			char * value = strtok(NULL, " ");
			int valid_num_flag = 0;

			int i;

			for (i = 0; i < strlen(value); i++) {
				if (isdigit(value[i]))
					valid_num_flag = 1;
				else
					valid_num_flag = 0;
				int arg = atoi(value);
				if (valid_num_flag == 1)
					printheader(arg);
				else
					printf("%s\n", "Incorrect argument types passed.");
			}
		}

		else {
			printf("%s\n", "Command not recognized.");
		
		}
		printf(">");


	

	}
	quit();
}

