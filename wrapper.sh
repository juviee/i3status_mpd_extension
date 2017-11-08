#!/bin/sh
# shell script to prepend i3status with more stuff
state=""

update_state() {
    state="$(i3mpdstatus cuso4ek 0 itg3t5better)"
}


i3status | (read line && echo "$line" && read line && echo "$line" && read line && echo "$line" && update_state && while :
do
  read line
  update_state
  echo ",[{\"full_text\":\"${state}\" },${line#,\[}" || exit 1
done)
