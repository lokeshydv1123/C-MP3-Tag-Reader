#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Common Constants
#define ID3_TAG        "ID3"

// Frame IDs
#define TITLE_TAG      "TIT2"
#define ARTIST_TAG     "TPE1"
#define ALBUM_TAG      "TALB"
#define YEAR_TAG       "TYER"
#define MUSIC_TAG      "TCON"
#define COMMENT_TAG    "COMM"

// Buffer Size
#define HEADER_SIZE        10
#define FRAME_ID_SIZE      4
#define FRAME_SIZE         4
#define FRAME_FLAG_SIZE    2
#define ENCODE_SIZE        1
#define BUFFER_SIZE        100

#endif