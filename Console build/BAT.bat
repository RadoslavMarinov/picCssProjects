echo off:
echo Hello Riko!
SET CCSC_path="C:\Program Files\PICC\Ccsc"
SET WINDOW_OPEN=+P
SET show_messages=+EA
SET stdout=+STDOUT
SET no_debug_file=-D
SET pcm=+FM
SET err_warn_mes=+EX
SET export=+EXPORT
SET version=+V
SET include_paths="source"

::SOURCE FILES::
SET main="StartButton_main"
SET start_button="start_button.c"

::OBJECT FILES::
SET object_files=StartButton_main.o,start_button.o

::HEX FILE::
SET hex_name=main.hex

SET link_objects=LINK="%hex_name%=%object_files%"

echo on:
%CCSC_path% %pcm% %includes% %stdout% %export% %start_button%
%CCSC_path% %pcm% %includes% %stdout% %export% %main%
%CCSC_path% %pcm% %includes% %stdout% %stdout% %link_objects%
type *.err
pause
end