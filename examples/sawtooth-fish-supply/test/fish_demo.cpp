#include "fish_encoders.h"
#include "time.h"
#include <stdio.h>
#include "sawtooth/st_transaction.h"

#define PAYLOAD_BUF_SIZE    2048

int main(int argc, char **argv)
{
    uint8_t buf[PAYLOAD_BUF_SIZE];
    int payload_written = 0;

    if(argc <=1 ) {
        fprintf(stderr, "Usage: %s <action>\n", argv[0]);
        return 1;
    }

    const char *action = argv[1];
    if(strcmp(action, "createAgent") == 0) {
        payload_written = encode_create_agent("POKUS", buf, sizeof(buf));
    } else if(strcmp(action, "createRecord") == 0) {
        payload_written = encode_properties(buf, sizeof(buf));
    } else if(strcmp(action, "updateRecord") == 0) {
        payload_written = encode_updateProperties(buf, sizeof(buf));
    } else {
        fprintf(stderr, "invalid action\n");
        return -1;
    }

    if(payload_written <= 0) {
        return -1;
    }

    int tx_written = sign_and_encode(buf, payload_written, buf+payload_written, sizeof(buf)-payload_written);
    if(tx_written <= 0) {
        return -1;
    }

    fwrite(buf+payload_written, 1, tx_written, stdout);
    return 0;
}
