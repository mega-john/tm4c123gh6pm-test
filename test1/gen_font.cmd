ftrasterize -f Verdana -s 24 -c charmap.txt -u -r c:\windows\fonts\Verdana.ttf
ftrasterize -f Arial -s 24 -c charmap.txt -u -r c:\windows\fonts\Arial.ttf
ftrasterize -f iso8859_5 -s 24 -p 32 -e 255 -t 128 -o 0x400 -u c:\windows\fonts\Verdana.ttf
