#include "mp3_tag.h"

// Function to display help message
void display_help(void)
{
    printf("\n------------ MP3 TAG READER & EDITOR ------------\n\n");

    printf("To View MP3 Tags:\n");
    printf("./a.out -v <file_name.mp3>\n\n");

    printf("To Edit MP3 Tags:\n");
    printf("./a.out -e -t <new_title> <file_name.mp3>\n");
    printf("./a.out -e -a <new_artist> <file_name.mp3>\n");
    printf("./a.out -e -A <new_album> <file_name.mp3>\n");
    printf("./a.out -e -y <new_year> <file_name.mp3>\n");
    printf("./a.out -e -m <new_genre> <file_name.mp3>\n");
    printf("./a.out -e -c <new_composer> <file_name.mp3>\n");

    printf("\n-----------------------------------------------\n");
}

// Function to check the operation type based on command line arguments
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-v") == 0)
        return VIEW;
    
    else if(strcmp(argv[1], "-e") == 0)
        return EDIT;
    
    else
        return INVALID;
}

// Function to validate if the file is an MP3 file
Status validate_mp3_file(char *filename)
{
    char *ptr;
    ptr = strstr(filename, ".mp3");

    if(ptr != NULL)
        return e_success;

    return e_failure;
}

// Function to open the MP3 file for reading
Status open_mp3(MP3Info *mp3)
{
    mp3->fptr = fopen(mp3->filename, "rb");

    if(mp3->fptr == NULL){
        printf("ERROR : Unable to open %s\n", mp3->filename);
        return e_failure;
    }

    return e_success;
}

// Function to validate the ID3 header of the MP3 file
Status validate_header(MP3Info *mp3)
{
    char id[4];
    unsigned char version[2];

    /* Read first 3 bytes */
    fread(id, 1, 3, mp3->fptr);

    id[3] = '\0';

    /* Check ID3 */
    if(strcmp(id, "ID3") != 0)
    {
        printf("ERROR : ID3 Header Not Found\n");
        return e_failure;
    }

    /* Read Version */
    fread(version, 1, 2, mp3->fptr);

    /* Validate Version 2.3 */
    if(version[0] != 3 || version[1] != 0)
    {
        printf("ERROR : Only ID3v2.3 is Supported\n");
        return e_failure;
    }

    fseek(mp3->fptr, 5, SEEK_CUR);

    return e_success;
}

// Function to convert a 4-byte big-endian buffer to an integer
int big_endian_to_int(unsigned char *buffer)
{
    int size;
    size = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8)  | buffer[3];

    return size;
}

// Function to print the tag information based on frame ID
void print_tag(char *frame_id, char *data)
{
    if(strcmp(frame_id, TITLE_TAG) == 0)
        printf("Title     : %s\n", data);
    
    else if(strcmp(frame_id, ARTIST_TAG) == 0)
        printf("Artist    : %s\n", data);
    
    else if(strcmp(frame_id, ALBUM_TAG) == 0)
        printf("Album     : %s\n", data);
    
    else if(strcmp(frame_id, YEAR_TAG) == 0)
        printf("Year      : %s\n", data);
    
    else if(strcmp(frame_id, MUSIC_TAG) == 0)
        printf("Genre     : %s\n", data);
    
    else if(strcmp(frame_id, COMMENT_TAG) == 0)
        printf("Composer  : %s\n", data);
}

// Function to read a frame from the MP3 file and print its information
Status read_frame(FILE *fp)
{
    char frame_id[5];
    unsigned char size_buffer[4];
    char data[BUFFER_SIZE];
    int frame_size;

    /* Read Frame ID */
    fread(frame_id, 1, 4, fp);
    frame_id[4] = '\0';

    /* Read Frame Size */
    fread(size_buffer, 1, 4, fp);

    /* Convert Big Endian to Integer */
    frame_size = big_endian_to_int(size_buffer);

    /* Skip 2-byte Flags */
    fseek(fp, 2, SEEK_CUR);

    /* Skip Encoding Byte */
    fseek(fp, 1, SEEK_CUR);

    if(strcmp(frame_id, "COMM") == 0)
    {
        char ch;

        /* Skip language (3 bytes) */
        fseek(fp, 3, SEEK_CUR);

        /* Skip description until NULL */
        do
        {
            fread(&ch, 1, 1, fp);
        }while(ch != '\0');

        /* Read actual comment */
        int bytes = frame_size - 5;   /* 1 encoding + 3 language + 1 NULL */

        fread(data, 1, bytes, fp);

        data[bytes] = '\0';
    }
    else
    {
        fread(data, 1, frame_size - 1, fp);
        data[frame_size - 1] = '\0';
    }

    /* Print Tag */
    print_tag(frame_id, data);

    return e_success;
}

// Function to view the tags of the MP3 file
Status view_tag(MP3Info *mp3)
{
    int i;

    /* Open MP3 File */
    if(open_mp3(mp3) == e_failure)
        return e_failure;

    /* Validate ID3 Header */
    if(validate_header(mp3) == e_failure)
    {
        fclose(mp3->fptr);
        return e_failure;
    }

    printf("\n");
    printf("---------------------------------------------\n");
    printf("       MP3 TAG READER (ID3v2.3)\n");
    printf("---------------------------------------------\n\n");

    // Read and Print 6 Frames
    for(i = 0; i < 6; i++)
    {
        if(read_frame(mp3->fptr) == e_failure){
            fclose(mp3->fptr);
            return e_failure;
        }
    }

    printf("\n---------------------------------------------\n");
    fclose(mp3->fptr);
    return e_success;
}