# MP3 Tag Reader

A C based MP3 Tag Reader and Editor that allows users to view and modify metadata stored in MP3 files using the ID3v2.3 tag format.

The project demonstrates binary file handling, structures, pointers, command line arguments, file operations, and parsing of MP3 metadata.

## Features

* View MP3 metadata
* Edit MP3 metadata
* Supports ID3v2.3 tags
* Modify title
* Modify artist
* Modify album
* Modify year
* Modify genre
* Modify composer
* Modify comment
* Validate MP3 ID3 headers
* Binary file handling
* Preserve MP3 audio data while updating metadata
* Command line interface
* Error handling for invalid operations and files

## Supported Metadata

The application works with the following ID3 frames:

| Frame  | Metadata |
| ------ | -------- |
| `TIT2` | Title    |
| `TPE1` | Artist   |
| `TALB` | Album    |
| `TYER` | Year     |
| `TCON` | Genre    |
| `TCOM` | Composer |
| `COMM` | Comment  |

## How It Works

An MP3 file can contain an ID3 tag at the beginning of the file.

The program first validates the ID3 header and checks the version before processing the metadata.

For viewing, the program reads the tag information frame by frame and displays the corresponding metadata.

For editing, the program creates a temporary MP3 file, copies the required data while replacing the selected metadata frame, and then replaces the original file with the updated file.

## Project Flow

### View Operation

```text
MP3 File
   ↓
Validate ID3 Header
   ↓
Check ID3 Version
   ↓
Read Tag Header
   ↓
Read Metadata Frames
   ↓
Extract Frame Data
   ↓
Display Metadata
```

### Edit Operation

```text
Original MP3
     ↓
Validate ID3 Header
     ↓
Read Metadata Frames
     ↓
Find Selected Frame
     ↓
Replace Metadata
     ↓
Write to Temporary File
     ↓
Copy Remaining MP3 Data
     ↓
Replace Original File
```

## Command Line Usage

### View MP3 Tags

```bash
./mp3_tag_reader -v <file.mp3>
```

Example:

```bash
./mp3_tag_reader -v sample.mp3
```

### Edit Title

```bash
./mp3_tag_reader -e -t "New Title" <file.mp3>
```

### Edit Artist

```bash
./mp3_tag_reader -e -a "Artist Name" <file.mp3>
```

### Edit Album

```bash
./mp3_tag_reader -e -A "Album Name" <file.mp3>
```

### Edit Year

```bash
./mp3_tag_reader -e -y "2026" <file.mp3>
```

### Edit Genre

```bash
./mp3_tag_reader -e -m "Rock" <file.mp3>
```

### Edit Composer

```bash
./mp3_tag_reader -e -c "Composer Name" <file.mp3>
```

## Project Structure

```text
MP3-Tag-Reader/
│
├── README.md
├── main.c
├── view.c
├── edit.c
├── mp3_tag.h
├── types.h
└── common.h
```

### File Description

| File        | Description                                                       |
| ----------- | ----------------------------------------------------------------- |
| `main.c`    | Handles command line arguments and selects the required operation |
| `view.c`    | Implements MP3 metadata reading and display                       |
| `edit.c`    | Implements MP3 metadata modification                              |
| `mp3_tag.h` | Contains structures and declarations related to MP3 tags          |
| `types.h`   | Contains project data types and operation definitions             |
| `common.h`  | Contains common project definitions                               |

## Compilation

Compile the project using GCC:

```bash
gcc main.c view.c edit.c -o mp3_tag_reader
```

Run the program using:

```bash
./mp3_tag_reader -v sample.mp3
```

## Technologies and Concepts Used

* C Programming
* Structures
* Pointers
* Command Line Arguments
* File Handling
* Binary File Handling
* `fopen()`
* `fread()`
* `fwrite()`
* `fseek()`
* `fclose()`
* String Handling
* File Copying
* Temporary Files
* Metadata Parsing
* ID3v2.3 Tag Format

## Key Learning Outcomes

This project provided practical experience with:

* Reading and writing binary files
* Parsing structured binary data
* Working with file offsets
* Using structures to represent metadata
* Processing command line arguments
* Manipulating metadata without modifying the actual audio content
* Safely updating files using a temporary file

## Error Handling

The application validates:

* Command line arguments
* File availability
* MP3 file format
* ID3 header
* ID3 version
* Supported editing options
* File opening and closing operations

Invalid inputs are rejected with appropriate error messages.

## Limitations

* The implementation specifically targets the ID3v2.3 format.
* The project focuses on the supported metadata fields implemented in the source code.
* It is not a complete MP3 parser and does not decode or modify the actual audio stream.

## Future Improvements

Possible improvements include:

* Support for ID3v2.4
* Support for additional ID3 frames
* Add album artwork support
* Improve validation for different MP3 variants
* Add a graphical user interface
* Add batch metadata editing
* Add automated tests

## Author

**Lokesh Yadav**

B.Tech Computer Science and Engineering

## License

This project is intended for educational and learning purposes.
