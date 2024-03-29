C University coursework. Received an outstanding first class grade (87%).
Summary: main code is in "converter.c". Coursework requirements only allowed one c file of submission, so first bit of this file is just test data. Please scroll down the file to read the code. Run length encoding and quadtrees are used to losslessy compress data.
The "Render" folder renders a custom designed file format (for files that end in ".sk"). SK files are files that contain a series of lines and rectangles to encode an image or animation. The "Render" section displays this image or animation. Use "make test" and "./test" to test the renderer. Use "make sketch" and "./sketch FILENAME" to render file. Some example files have been included (E.g. "./sketch sketch00.sk").
The "Convert" section contains code that is used to convert SK files to a format known as PGM (".pgm"). PGM files simply store the grayscale brightness of each indvidual pixel sequentially. converter.c also converts PGM files to sk files. To do this, lossless compression is used. I have used run length encoding and quadtree compression to optimse the compression ratio. Use "make converter" to compile. "./converter" runs all of the unit tests. "./converter FILENAME.sk" produces the equivalent PGM file ("FILENAME.pgm"). "./converter FILENAME.pgm" produces the equivalent SK file ("FILENAME.sk"). Feel free to try "./converter fractal.pgm" and "./converter bands.pgm". You can view the corresponding sk files using the "Render" module".
"Data" stores a fresh copy of the PGM and SK files.
Example screenshots have been included.
Everything works when using linux (or WSL).

![image](https://user-images.githubusercontent.com/64362945/177980360-2bd296e4-459f-41de-8737-406e830999d7.png)

![image](https://user-images.githubusercontent.com/64362945/177980407-1e2abfb5-48ea-4fa4-8364-018ec970da34.png)
