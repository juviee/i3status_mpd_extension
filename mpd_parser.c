#include <mpd/client.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
//#ifndef TIMEOUT_MS
//    #define TIMEOUT_MS 200
//#endif
//#define MAX_LENGTH_ARTIST 10
//#define MAX_LENGTH_TITLE 15
enum parsing_lengths{
    MAX_LENGTH_ARTIST = 10,
    MAX_LENGTH_TITLE = 25,
    MAX_LENGTH_RESPONSE = 60,
};

enum err_codes{
    OK = 0,
    WRONG_AMOUNT_OF_ARGS = 1,
    WRONG_SECOND_ARG = 2,
    OUT_OF_MEMERY = 3,
    RECV_FAIL = 4,
};

int parse_status(struct mpd_connection*, char*, size_t len); //conn->statusbar
int parse_seconds_to_minutes(unsigned sec, char*, size_t len); //seconds->min:sec
int get_player_options(struct mpd_status*, char*, size_t len);//status->rzsc(guaranty: len<=7)
int get_song_tag(struct mpd_song*,char* tag_name, int idx, char*, size_t len );//song->song_title


int main(int argc, char* argv[]){
    unsigned port = 0;
    char* host = NULL;
    char* password = NULL;
    //printf("%d", argc);
    if(argc >= 3){
        //if args presented
        host = argv[1];
        unsigned port = 0;
        if(!sscanf(argv[2], "%u", &port)){
            fprintf(stderr, "[error] wrong second arg\n");
            return WRONG_SECOND_ARG;
        }
    }
    if(argc == 4)
        //if password exists too
        password = argv[3];
    
    //printf("%s/%d/%s", host, port, password);
    //opening connection
    struct mpd_connection* mpd_con = mpd_connection_new(host, port, 0);
    

    if(!mpd_con){
        //if connection opening failed, then likely OOM happened
        fprintf(stderr, "[error]55 out of memery\n");
        return OUT_OF_MEMERY;
    }

    //checking, if opened connection is valid
    int error_code = mpd_connection_get_error(mpd_con); 
    if(error_code != MPD_ERROR_SUCCESS){
        fprintf(stderr, "[error]%s", \
                mpd_connection_get_error_message(mpd_con)); 
        mpd_connection_free(mpd_con);
        return error_code;
    }
    //if password presented - gain privelege
    if(password)
        mpd_run_password(mpd_con, password);
    
    char* response = malloc(MAX_LENGTH_RESPONSE);
    if(!response){
       fprintf(stderr, "[error]73 out of memery\n");
    }
    error_code = parse_status(mpd_con, response, MAX_LENGTH_RESPONSE);
    
    //checking if function is failed with error or smth
    if(!error_code){
    //no errors, all gut
        printf("%s", response);
    }
    else if(error_code == RECV_FAIL){
    //failed at recieving(insufficient privelegies or smth)
        printf("%s", response);
        fprintf(stderr, "[error]%s\n", \
                mpd_connection_get_error_message(mpd_con));

    }
    else if(error_code == OUT_OF_MEMERY){
    //something very bad happened
        fprintf(stderr, "[error]88 out of memery\n");
        fprintf(stderr, "[error]%s\n", \
                mpd_connection_get_error_message(mpd_con));

    }
    mpd_connection_free(mpd_con);
    free(response);
    return error_code;
}

int parse_status(struct mpd_connection* mpd_con, char* string_ptr, size_t len){
    
    len--;
    int succ = mpd_send_status(mpd_con);
    if(!succ){
        snprintf(string_ptr, len, "RecvFail\n");
        return RECV_FAIL;
    }

    struct mpd_status* mpd_stat = mpd_recv_status(mpd_con);
    if(!mpd_stat){
        string_ptr[0] = '\0';
        return OUT_OF_MEMERY;
    }
    int state = mpd_status_get_state(mpd_stat);
    if(state == MPD_STATE_STOP){
        mpd_status_free(mpd_stat);
        snprintf(string_ptr, len, "Stop");
        return OK;
    }
    else if(state == MPD_STATE_UNKNOWN){
        mpd_status_free(mpd_stat);
        snprintf(string_ptr, len, "Unknown");
        return OK;
    }
    //where plays the track
    unsigned elapsed_time = mpd_status_get_elapsed_time(mpd_stat);
    unsigned song_length_time = mpd_status_get_total_time(mpd_stat);
    
    //char's of player
    snprintf(string_ptr++, len--, "[");
    string_ptr[0] = 70;
    int ln_op = 0;
    ln_op = get_player_options(mpd_stat, string_ptr, len);
    len -= ln_op;
    mpd_status_free(mpd_stat);
    strncat(string_ptr, "]", len--);
//    strncat(string_ptr++, " ", len--);
    //parsing the time
    strncat(string_ptr, "(", len--);
    ln_op = parse_seconds_to_minutes(elapsed_time, string_ptr, len);
    len -= ln_op;
    strncat(string_ptr++, "|", len--);
    ln_op = parse_seconds_to_minutes(song_length_time, string_ptr, len);
    len -= ln_op;
    strncat(string_ptr++, ")", len--);
//    strncat(string_ptr++, " ", len--);

    //name of song and name of artist
    struct mpd_song* song = mpd_run_current_song(mpd_con);
    if(!song){
        mpd_status_free(mpd_stat);
        mpd_song_free(song);
        return OUT_OF_MEMERY;
    }
    //parsing artist and track

    if(len < MAX_LENGTH_ARTIST)
        ln_op = get_song_tag(song, "ARTIST", 0, string_ptr, len);
    else
        ln_op = get_song_tag(song, "ARTIST", 0,\
                string_ptr, MAX_LENGTH_ARTIST);
    len-=ln_op;
    
    strncat(string_ptr++, " ", len--);
    strncat(string_ptr++, "-", len--);
    strncat(string_ptr++, " ", len--);
    if(len < MAX_LENGTH_TITLE)
        ln_op = get_song_tag(song, "TITLE", 0, string_ptr, len);
    else
        ln_op = get_song_tag(song, "TITLE", 0,\
                string_ptr, MAX_LENGTH_TITLE);
    len-=ln_op;
    string_ptr+=ln_op;

    mpd_song_free(song);
    return OK;
}

int get_player_options(struct mpd_status* mpd_stat, char* str_ptr, size_t len){
    unsigned play = mpd_status_get_state(mpd_stat);
    unsigned repeat = mpd_status_get_repeat(mpd_stat);
    //printf("%zd", len);
    unsigned random = mpd_status_get_random(mpd_stat);
    unsigned single = mpd_status_get_single(mpd_stat);
    unsigned consume = mpd_status_get_consume(mpd_stat);
    int i = 0;
    if((MPD_STATE_PLAY == play) && (len>0)){
        str_ptr[i++] = '>';
        --len;
    }
    else if((MPD_STATE_PAUSE == play) && (len>0)){
        str_ptr[i++] = '~';
        --len;
    }
    else if((MPD_STATE_STOP== play) && (len>0)){
        str_ptr[i++] = 'S';
        --len;
    }
    if((repeat)&&(len>0)){
        str_ptr[i++] = 'r';
        --len;
    }
    if(random&&len>0){
        str_ptr[i++] = 'z';
        --len;
    }
    if(single&&len>0){
        str_ptr[i++] = 'I';
        --len;
    }
    if(consume&&len>0){
        str_ptr[i++] = 'c';
        --len;
    }
    str_ptr[i] = '\0';
    return i;
}

int parse_seconds_to_minutes(unsigned sec, char* str_ptr, size_t len){
    //133:54
    int minutes = sec/60;
    int sec_remained = sec%60;
    char time[20];
    snprintf(time, len, "%d:%d", minutes, sec_remained);
    int ln = strlen(time);
    strncat(str_ptr, time, len);
    return ln;
}

int get_song_tag(struct mpd_song* song, char* tag_name,int idx,\
        char* string_ptr, size_t len){//song->song_title
    int tag = mpd_tag_name_iparse(tag_name);
    int ln_s = 0;
//    printf("%d", tag);
    const char* tag_str = mpd_song_get_tag(song, tag, 0);
    if(tag_str){
        strncat(string_ptr, tag_str,len);
        ln_s = strlen(tag_str);
    }
    //free(tag_str);
    return ln_s;
}
