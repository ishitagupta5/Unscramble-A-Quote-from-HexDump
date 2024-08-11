# Unscramble-A-Quote-from-HexDump

Details
In this phase we are going to use the Word and Word_List structs you wrote to do some work. The work will be to go through a piece of memory and unscramble the quote that is found. Below is a hexdump of one of the data files. This was created using the command hexdump -C data.bin > dump.txt. This can be useful so you can open the hex dump in your editor so you can “see” what is going on. I added the row for the column numbers to make it a little easier to find the row x column intersection. Again this data is not in a 2D array.

               0  1  2  3  4  5  6  7   8  9  a  b  c  d  e  f
    00000000  1f 00 d7 00 06 cc 00 77  68 6f 05 1d 00 74 6f 07  |.......who...to.|
    00000010  87 00 73 61 69 64 07 0a  00 61 62 6c 65 06 4a 00  |..said...able.J.|
    00000020  73 65 65 07 b7 00 72 65  61 6c 06 b1 00 74 68 65  |see...real...the|
    00000030  07 79 00 4a 75 61 6e 09  5f 00 69 64 65 61 73 22  |.y.Juan._.ideas"|
    00000040  05 0f 00 69 73 05 2a 00  69 73 0c d4 00 74 6f 6d  |...is.*.is...tom|
    00000050  6f 72 72 6f 77 2e 09 00  00 4c 65 73 73 69 67 05  |orrow....Lessig.|
    00000060  98 00 62 79 09 8e 00 66  75 74 75 72 65 0c c5 00  |..by...future...|
    00000070  52 65 76 69 65 77 69 6e  67 08 6d 00 4d 65 6e 67  |Reviewing.m.Meng|
    00000080  2c 06 23 00 74 68 65 07  81 00 74 68 61 74 05 37  |,.#.the...that.7|
    00000090  00 6f 66 05 30 00 2d 2d  0b 00 00 4c 61 77 72 65  |.of.0.--...Lawre|
    000000a0  6e 63 65 08 ab 00 74 6f  64 61 79 06 16 00 62 75  |nce...today...bu|
    000000b0  74 06 04 00 6f 6e 65 09  45 00 77 69 6e 6e 65 72  |t...one.E.winner|
    000000c0  05 a3 00 69 6e 07 64 00  22 54 68 65 08 c0 00 6c  |...in.d."The...l|
    000000d0  69 76 65 73 03 93 00 05  40 00 49 74              |ives....@.It|
    000000dc
Let’s talk about this data so you can understand what is going on here. First, there are some two byte numbers in here, like the first two bytes, and the two bytes after that. Since this is a little ended system, the low byte is written first and the high byte is second. So even though we see 1f00 the actual data is 001f which converted from hex to decimal is the number 31. That means this quote has 31 words in it. The next two bytes are 00d7 and that is the offset of the first record. So if you go down to the intersection of the 000000d0 row and the 7 column you will find 05. This number indicates the number of bytes in the word record. The next two bytes after that is the offset for the next word record. Finally, the remaining bytes in the record are the letters in the word.

Bytes	Meaning
First two bytes in the file	The number of words in the quote
Second two bytes in the file	The offset for the first word record
First byte of word record	The number of bytes in the word record
Second two bytes of the word record	The offset to the following word record
Reaming bytes in word record	The letters that are in the word
How to get bytes from memory.
You can (should) use pointer casting to get the bytes from memory. For example, suppose you have a pointer to a byte of memory, but you want to get out 2 bytes. You can cast the pointer as a 2 byte pointer and then dereference that cast and store the result into a 2 byte variable.

This pointer casting will handle “flipping” two byte words back to their correct representation. Remember, the data is stored in little ended order but that is handled for you when you read the data using pointer casting.

For getting the letters out of memory, think about how you loop and get a byte at at time from memory. If you store the byte in a character, or array of characters, C will automatically interpret those bytes as characters, even if the byte is not a valid character. So if you get strange symbols in your output, it probably means you’ve got non-character data in your word.

Reading the data from the binary file
Part of your job will be to read the data file and store it in the array to start this whole process off. To do this, you will need to know how many bytes are in the data file. The input file to your program will tell you so you can create an array of uint8_t. Then you can use fread to read in the data. This is the signature for fread

size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
So the first parameter is the array, or the pointer to the array that you have made that is big enough to hold the amount of data. The next size the size of the data it is reading, think sizeof(uint8_t) goes there. The 3rd parameter is how many of these items is it reading, this is the number we give you and you have used to make the array. The final parameter is a pointer to the FILE that contains the data. You will open this file in a very similar fashion to how you have opened other files for reading, but you need to open it in binary mode. So you will add a b to the mode string. The name of the file will also be given to you in the input file to your program

FILE* binary_file = fopen( filename, "rb" );
Where the rb means read and binary. Now you can use this binary_file in the fread function call.

Then the data that is read from the fread is the data that will be like the example given above.

Fuzzy
This project will also have a requirement to read the data that has been fuzzed. What this means is that we have done a little extra work to the data before we put it in the file so it will not be easy to read. The other version is clear text, because you can read it, and this is not clear, or unclear, or fuzzy.

So the layout of the data is the same. So you can use the same algorithm, with some added unfuzzing steps added.

How is the data fuzzed?
The data in the input is fuzzed in 2 ways.

We use the first letter from the word and use XOR (^) on each of the bytes in the next offset. I recommend making a mask that is 2 bytes and put the first letter in each byte. This can be done using | and <<. See below.

We fuzz the letters in the word by using the length of the record. But we don’t just use the length, we first flip the nybbles.

What is flipping the nybbles? If you think of a byte as being composed of 8 bits, and you number them from left to right bit 7, bit 6, … bit 1, bit 0, then we take bits 7-4 and swap them with bits 3-0, so we start with b7b6b5b4 b3b2b1b0 and end up with b3b2b1b0 b7b6b5b4. Now, using this flipped length, we loop over the letters in the word and use XOR (^) on each of the letters.
You will need to reverse the order we used but you can use XOR (^) like we did. That is one of the nice properties of XOR. If you apply XOR to a value using another value and then apply XOR using the same two values, you will get back to the original.

int x = 7, y = 9;
x = x ^ y; //will yield 0xe, or 14 in decimal
x = x ^ y; //will yield 7 on this application
To make a two byte mask, I used a uint16_t and put the first letter in it. Think about where that will assign it. Then I shifted the two byte mask and used bitwise or (|) to put the first letter back in the two byte mask. Then when you xor a two byte mask with a two byte offset, it unmasks both bytes at the same time.

Input and Output
Input
The first argument to your main function will be the input or command file. The input will consist of 3 lines in the following order.

The first line is the number of bytes in the data file.
Use this to create a uint8_t array dynamically to hold the data from the data file.
Use this in the fread function call as the count parameter
The name of the data file.
This will be opened in the "rb" mode and will become the last parameter to the fread function.
The last line will either be clear or fuzzy. This indicates if the data is in clear text or has been fuzzed.
220
data-clear.bin
clear
Output
Essentially it will be a list of the words and the offset of where the word was found in the file. Be careful about the offset of the word record and the offset of the word itself. They are not the same.

Bytes in file:  141
Name of file:   data-clear-2.bin
Clear|Fuzzy:    clear
Any                  @ Offset: 4c
fool                 @ Offset: 07
can                  @ Offset: 74
make                 @ Offset: 7a
the                  @ Offset: 6e
simple               @ Offset: 3c
complex,             @ Offset: 31
only                 @ Offset: 45
a                    @ Offset: 0e
smart                @ Offset: 12
person               @ Offset: 87
can                  @ Offset: 5c
make                 @ Offset: 67
the                  @ Offset: 81
complex              @ Offset: 1d
simple.              @ Offset: 52
                     @ Offset: 1a
--                   @ Offset: 62
unknown              @ Offset: 27
Data for these inputs and outputs
    00000000  13 00 49 00 07 71 00 66  6f 6f 6c 04 0f 00 61 08  |..I..q.fool...a.|
    00000010  84 00 73 6d 61 72 74 03  5f 00 0a 4f 00 63 6f 6d  |..smart._..O.com|
    00000020  70 6c 65 78 0a 00 00 75  6e 6b 6e 6f 77 6e 0b 42  |plex...unknown.B|
    00000030  00 63 6f 6d 70 6c 65 78  2c 09 2e 00 73 69 6d 70  |.complex,...simp|
    00000040  6c 65 07 0b 00 6f 6e 6c  79 06 04 00 41 6e 79 0a  |le...only...Any.|
    00000050  17 00 73 69 6d 70 6c 65  2e 06 64 00 63 61 6e 05  |..simple..d.can.|
    00000060  24 00 2d 2d 07 7e 00 6d  61 6b 65 06 39 00 74 68  |$.--.~.make.9.th|
    00000070  65 06 77 00 63 61 6e 07  6b 00 6d 61 6b 65 06 1a  |e.w.can.k.make..|
    00000080  00 74 68 65 09 59 00 70  65 72 73 6f 6e           |.the.Y.person|
    0000008d
