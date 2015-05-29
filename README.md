#ws2txt

Utility for converting WordStar 4.0 .DOC files to plaintext

##Description

WordStar 4.0 has had a recent resurgence of interest after George R.R. Martin, author of the A Song of Ice and Fire series (Game of Thrones), [revealed](http://www.youtube.com/watch?v=X5REM-3nWHg) that he has been using, and still does use, the DOS program from 1987 to write his novels.

This utility converts the WordStar .DOC format to plaintext. The WordStar format features no compression. It is aleady mostly plaintext, with a few tweaks to word endings and some file padding.

##Installation

```bash
make && sudo make install
```

##Usage

```bash
Usage: ws2txt <in.doc> [<out.txt>]
              [-h,--help]
```

Note: If no output is specified, the program will output to stdout.