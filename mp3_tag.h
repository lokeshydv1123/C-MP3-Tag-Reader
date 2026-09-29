#ifndef MP3_TAG_H
#define MP3_TAG_H

#include "types.h"
#include "common.h"

// MP3 Information Structure
typedef struct
{
    FILE *fptr;
    FILE *temp_fptr;

    char *filename;
    char *temp_filename;

    char *edit_option;
    char *new_data;

} MP3Info;

// Function Prototypes for Validation
OperationType check_operation_type(char *argv[]);
Status validate_mp3_file(char *filename);

// Fuction Prototypes for View
Status open_mp3(MP3Info *mp3);
Status validate_header(MP3Info *mp3);
Status view_tag(MP3Info *mp3);
int big_endian_to_int(unsigned char *buffer);
Status read_frame(FILE *fp);
void print_tag(char *frame_id, char *data);

// Function Prototypes for Edit
Status open_files(MP3Info *mp3);
Status copy_header(FILE *src, FILE *dest);
char *get_frame(char *option);
Status edit_tag(MP3Info *mp3);
Status copy_remaining_data(FILE *src, FILE *dest);
void int_to_big_endian(int size, unsigned char *buffer);

// Function Prototypes for Help
void display_help(void);

#endif