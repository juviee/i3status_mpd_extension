## Synopsis

Wrapper extension for i3status that supports Music Player Daemon. Makes sweet output to your i3status bar; like this: 
`[>rz\](1:04|2:10)Gidropony - Paradise`

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
1. Build c file
```
sudo apt-get install libmpdclient-dev
gcc -lmpdclient -o i3mpdstatus mpd_parser.c
sudo cp i3mpdstatus /usr/local/bin
```
Provide code examples and explanations of how to get the project.
2. Setup your wrapper
change passwords, host and port in wrapper.sh
`chmod a+x wrapper.sh` 
3. Setup your i3/config
```
bar {
    status_command wrapper.sh
}
```
4. Reload your i3wm

## License
WTFPL
