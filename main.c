#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static void
print_tag(const struct mpd_song *song, enum mpd_tag_type type, const char *label){
	unsigned i = 0;
	const char *value;
	while ((value = mpd_song_get_tag(song, type, i++)) != NULL){

	char buf[255];
	sprintf(buf, "echo \"%s\" >> ~/dev/mpdclient/test", value);
	system(buf);
	}
}

int main(int argc, char* argv[]){
	struct mpd_connection *conn;
	conn = mpd_connection_new(NULL, 6000, 30000);
	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		fprintf(stderr,"%s\n",mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	mpd_command_list_begin(conn, true);
	mpd_send_status(conn);
	mpd_send_current_song(conn);
	mpd_command_list_end(conn);
	struct mpd_status * status;
	struct mpd_song *song;
        status = mpd_recv_status(conn);
	if (status == NULL) {
		fprintf(stderr,"%s\n",
		mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	mpd_response_next(conn);
	while ((song = mpd_recv_song(conn)) != NULL) {
		print_tag(song, MPD_TAG_TITLE, "title");
		mpd_song_free(song);
	}
	return 0;
}
