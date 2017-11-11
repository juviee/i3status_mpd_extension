## Synopsis

Wrapper extension for i3status that supports Music Player Daemon. Makes sweet output to your i3status bar; like this: 
```
[>rz\](1:04|2:10)Gidropony - Paradise
```

## Code Example

Binary should be invoked in one of 3 following ways
```
i3mpdstatus <hostname> <port>
i3mpdstatus <hostname> <port> <password>
i3mpdstatus
```

## Motivation

I turned off toolbars in my ncmpcpp or smth like that, lol.

## Installation
###Build c file###
```
sudo apt-get install libmpdclient-dev
gcc -lmpdclient -o i3mpdstatus mpd_parser.c
sudo cp i3mpdstatus /usr/local/bin
```
###Setup your wrapper###
change passwords, host and port in wrapper.sh
```
chmod a+x wrapper.sh
```

###Setup your i3/config###

```
bar {
    status_command wrapper.sh
}
```
###Reload your i3wm###

##Issues
1. Failing at handling json while song/group contains double quotes
2. Bad handling maximum length of songs
MPD works with UTF-8 characters while program's max length hardcoded with 
support simple byte chars. It requires overwriting checking the length.
## License
WTFPL
