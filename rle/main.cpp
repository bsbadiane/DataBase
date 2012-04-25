/*
 * main.cpp
 *
 *  Created on: 24.04.2012
 *      Author: night
 */

#include <cstdio>
#include <memory>
#include <cstdlib>
#include <cctype>
#include <cstring>
using namespace std;


/* Коды ошибок */
#define NO_ERROR      0
#define BAD_FILE_NAME 1
#define BAD_ARGUMENT  2

/* входной и выходной файлы */
FILE *source_file, *dest_file;

/*
 'byte_stored_status' равен 'TRUE'если байт
 прочитан 'fgetc'или 'FALSE' если нет дейтвительного
 байта, не прочитано и не содержится в 'val_byte_stored'
 */

int byte_stored_status = false;
int val_byte_stored;

/* псевдопроцедуры */

#define end_of_data() (byte_stored_status?false:\
	!(byte_stored_status=\
	((val_byte_stored=fgetc(source_file))!=EOF)))

#define read_byte()  (byte_stored_status?\
	byte_stored_status=false,(unsigned char)val_byte_stored:\
	(unsigned char)fgetc(source_file))

#define write_byte(byte)  ((void)fputc((byte),dest_file))

#define write_array(array,byte_nb_to_write)\
	  ((void)fwrite((array),1,(byte_nb_to_write),dest_file))

#define write_block(byte,time_nb)\
  {	\
unsigned char array_to_write[129];\
(void)memset(array_to_write,(byte),(time_nb));	\
write_array(array_to_write,(time_nb));	\
}
//=========================================================
// Сжатие методом RLE
void compress() {
	unsigned char byte1, byte2, frame_size, array[129];

	if (!end_of_data()) {
		byte1 = read_byte();
		frame_size = 1;
		if (!end_of_data()) {
			byte2 = read_byte();
			frame_size = 2;
			do {
				if (byte1 == byte2) {
					while ((!end_of_data()) && (byte1 == byte2)
							&& (frame_size < 129)) {
						byte2 = read_byte();
						frame_size++;
					}
					if (byte1 == byte2)
					{
						write_byte(126+frame_size);
						write_byte(byte1);
						if (!end_of_data()) {
							byte1 = read_byte();
							frame_size = 1;
						} else
							frame_size = 0;
					} else {
						write_byte(125+frame_size);
						write_byte(byte1);
						byte1 = byte2;
						frame_size = 1;
					}
					if (!end_of_data()) {
						byte2 = read_byte();
						frame_size = 2;
					}
				} else
				{
					*array = byte1;
					array[1] = byte2;
					while ((!end_of_data())
							&& (array[frame_size - 2] != array[frame_size - 1])
							&& (frame_size < 128)) {
						array[frame_size] = read_byte();
						frame_size++;
					}
					if (array[frame_size - 2] == array[frame_size - 1])
					/* встретилась ли последовательность из разных байт,
					 следующих за одинаковыми? */
					{ /* Да, тогда не считаем 2 последних байта */
						write_byte(frame_size-3);
						write_array(array, frame_size-2);
						byte1 = array[frame_size - 2];
						byte2 = byte1;
						frame_size = 2;
					} else {
						write_byte(frame_size-1);
						write_array(array, frame_size);
						if (end_of_data())
							frame_size = 0;
						else {
							byte1 = read_byte();
							if (end_of_data())
								frame_size = 1;
							else {
								byte2 = read_byte();
								frame_size = 2;
							}
						}
					}
				}
			} while ((!end_of_data()) || (frame_size >= 2));
		}
		if (frame_size == 1) {
			write_byte(0);
			write_byte(byte1);
		}
	}
}
//---------------------------------------------------------
// Декомпрессия методом RLE
void decompress() {
	unsigned char header;
	register unsigned char i;

	while (!end_of_data()) {
		header = read_byte();
		switch (header & 128) {
		case 0:
			if (!end_of_data())
				for (i = 0; i <= header; i++)
					write_byte(read_byte());
			/* else INVALID FILE */
			break;
		case 128:
			if (!end_of_data())
				write_block(read_byte(), (header & 127)+2);
			/* else INVALID FILE */
			break;
		}
	}
}
//---------------------------------------------------------
// главная функция
int main(int argc, char* argv[]) {
	if (argc != 4) {
		//help();
		printf("Неверные аргументы.\n");
		exit(BAD_ARGUMENT);
	} else if (tolower(argv[1][0]) != 'e' && tolower(argv[1][0]) != 'd') {
		//help();
		printf("Неверные аргументы.\n");
		exit(BAD_ARGUMENT);
	} else if ((source_file = fopen(argv[2], "rb")) == NULL) {
		//help();
		printf("Ошибка входного файла.\n");
		exit(BAD_FILE_NAME);
	} else if ((dest_file = fopen(argv[3], "wb")) == NULL) {
//		/help();
		printf("Ошибка выходного файла.\n");
		exit(BAD_FILE_NAME);
	} else {
		if (tolower(argv[1][0]) == 'e')
			compress();
		else
			decompress();
		fclose(source_file);
		fclose(dest_file);
	}
	printf("Сделано.\n");
	return (NO_ERROR);
}

