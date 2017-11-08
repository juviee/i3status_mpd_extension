## Synopsis

Wrapper extension for i3status that supports Music Player Daemon. Makes sweet output to your i3status bar; like this: 
\[>rz\](1:04|2:10)Gidropony - Paradise

## Code Example

Show what the library does as concisely as possible, developers should be able to figure out **how** your project solves their problem by looking at the code example. Make sure the API you are showing off is obvious, and that your code is short and concise.

## Motivation

I turned off toolbars in my ncmpcpp or smth like that, lol.

## Installation
1. Build c file
'sudo apt-get install libmpdclient-dev
gcc -lmpdclient -o i3mpdstatus mpd_parser.c
sudo cp i3mpdstatus /usr/local/bin'
Provide code examples and explanations of how to get the project.
2. Setup your wrapper
change passwords, host and port in wrapper.sh
'chmod a+x wrapper.sh' 
3. Setup your i3/config
'bar {
    status_command wrapper.sh
}'
4. Reload your i3wm

## License
WTFPL
