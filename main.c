#include "mp3_tag.h"

int main(int argc, char *argv[])
{
    MP3Info mp3;
    OperationType op;

    // Check for minimum number of arguments
    if (argc < 3)
    {
        display_help();
        return 1;
    }

    // Check operation type
    op = check_operation_type(argv);

    switch (op)
    {
        case VIEW:

            if (argc != 3)
            {
                printf("ERROR : Invalid number of arguments\n\n");
                display_help();
                return 1;
            }

            /* Validate mp3 file */
            if (validate_mp3_file(argv[2]) == e_failure)
            {
                printf("ERROR : Invalid MP3 file\n");
                printf("Please use MP3 format only for the Music file.\n");
                return 1;
            }

            mp3.filename = argv[2];

            if (view_tag(&mp3) == e_failure)
            {
                printf("ERROR : Unable to read MP3 tags\n");
                return 1;
            }

            break;

        case EDIT:

            if (argc != 5)
            {
                printf("ERROR : Invalid number of arguments\n\n");
                display_help();
                return 1;
            }

            /* Validate mp3 file */
            if (validate_mp3_file(argv[4]) == e_failure)
            {
                printf("ERROR : Invalid MP3 file\n");
                printf("Please use MP3 format only for the Music file.\n");
                return 1;
            }

            // Set MP3 information
            mp3.filename = argv[4];
            mp3.temp_filename = "temp.mp3";
            mp3.edit_option = argv[2];
            mp3.new_data = argv[3];

            if(edit_tag(&mp3) == e_failure)
            {
                printf("ERROR : Unable to Edit Tag\n");
                return 1;
            }

            break;

        default:

            printf("ERROR : Invalid Option\n\n");
            display_help();
            return 1;
    }

    return 0;
}