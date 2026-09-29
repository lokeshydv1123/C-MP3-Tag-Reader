#include "mp3_tag.h"

// Function to open the MP3 file for reading
Status open_files(MP3Info *mp3)
{
    mp3->fptr = fopen(mp3->filename, "rb");

    if(mp3->fptr == NULL)
    {
        printf("ERROR : Unable to open %s\n", mp3->filename);
        return e_failure;
    }

    mp3->temp_fptr = fopen(mp3->temp_filename, "wb");

    if(mp3->temp_fptr == NULL)
    {
        printf("ERROR : Unable to create temporary file\n");
        fclose(mp3->fptr);
        return e_failure;
    }

    return e_success;
}

// Function to copy the 10-byte ID3 header from source to destination
Status copy_header(FILE *src, FILE *dest)
{
    char header[10];
    fread(header,1,10,src);
    fwrite(header,1,10,dest);
    return e_success;
}

// Function to get the frame ID based on the edit option
char *get_frame(char *option)
{
    if(strcmp(option,"-t")==0)
        return TITLE_TAG;

    else if(strcmp(option,"-a")==0)
        return ARTIST_TAG;

    else if(strcmp(option,"-A")==0)
        return ALBUM_TAG;

    else if(strcmp(option,"-y")==0)
        return YEAR_TAG;

    else if(strcmp(option,"-m")==0)
        return MUSIC_TAG;

    else if(strcmp(option,"-c")==0)
        return COMMENT_TAG;

    return NULL;
}

// Function to copy the remaining audio data from source to destination
Status copy_remaining_data(FILE *src, FILE *dest)
{
    char ch;
    while(fread(&ch,1,1,src) == 1)
        fwrite(&ch,1,1,dest);

    return e_success;
}

// Function to convert an integer to a 4-byte big-endian buffer
void int_to_big_endian(int size, unsigned char *buffer)
{
    buffer[0] = (size >> 24) & 0xFF;
    buffer[1] = (size >> 16) & 0xFF;
    buffer[2] = (size >> 8) & 0xFF;
    buffer[3] = size & 0xFF;
}

// Function to edit the tag of the MP3 file based on the provided information
Status edit_tag(MP3Info *mp3)
{
    char frame_id[5];
    unsigned char size_buffer[4];
    char flags[2];
    char encoding;
    char *required_frame;
    int frame_size;
    int i;
    char buffer[BUFFER_SIZE];

    required_frame = get_frame(mp3->edit_option);

    // Check if the provided edit option is valid
    if(required_frame == NULL)
    {
        printf("ERROR : Invalid Edit Option\n");
        return e_failure;
    }

    if(open_files(mp3) == e_failure)
        return e_failure;

    if(validate_header(mp3) == e_failure)
    {
        fclose(mp3->fptr);
        fclose(mp3->temp_fptr);
        return e_failure;
    }

    /* Go back to beginning */
    rewind(mp3->fptr);

    /* Copy 10-byte header */
    copy_header(mp3->fptr, mp3->temp_fptr);

    /* Read six frames */
    for(i = 0; i < 6; i++)
    {
        fread(frame_id,1,4,mp3->fptr);
        frame_id[4] = '\0';

        fread(size_buffer,1,4,mp3->fptr);

        frame_size = big_endian_to_int(size_buffer);

        fread(flags,1,2,mp3->fptr);

        fread(&encoding,1,1,mp3->fptr);

        if(strcmp(frame_id, required_frame) == 0)
        {
            /* Write Frame ID */
            fwrite(frame_id,1,4,mp3->temp_fptr);

            /* New Frame Size */
            int new_size = strlen(mp3->new_data) + 1;

            int_to_big_endian(new_size, size_buffer);

            fwrite(size_buffer,1,4,mp3->temp_fptr);

            /* Flags */
            fwrite(flags,1,2,mp3->temp_fptr);

            /* Encoding */
            fwrite(&encoding,1,1,mp3->temp_fptr);

            /* New Data */
            fwrite(mp3->new_data,1,strlen(mp3->new_data),mp3->temp_fptr);

            /* Skip old data */
            fseek(mp3->fptr, frame_size - 1, SEEK_CUR);
        }
        else
        {
            /* Write Frame ID */
            fwrite(frame_id,1,4,mp3->temp_fptr);

            /* Write Size */
            fwrite(size_buffer,1,4,mp3->temp_fptr);

            /* Write Flags */
            fwrite(flags,1,2,mp3->temp_fptr);

            /* Write Encoding */
            fwrite(&encoding,1,1,mp3->temp_fptr);

            fread(buffer,1,frame_size - 1,mp3->fptr);

            fwrite(buffer,1,frame_size - 1,mp3->temp_fptr);
        }
    }

    /* Copy remaining audio data */
    copy_remaining_data(mp3->fptr, mp3->temp_fptr);

    // Close files and replace original file with the updated temporary file
    fclose(mp3->fptr);
    fclose(mp3->temp_fptr);

    // Remove the original MP3 file and rename the temporary file to the original filename
    remove(mp3->filename);
    rename(mp3->temp_filename, mp3->filename);

    printf("Tag Updated Successfully\n");

    return e_success;
}