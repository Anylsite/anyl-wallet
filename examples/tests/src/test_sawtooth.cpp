/**
* @brief 
* @file test_sawtooth.cpp
* @author J.H. 
* @date 2018-11-16
*/

/* module header */

/* system includes C */

/* system includes C++ */
#include <gtest/gtest.h>


/* local includes */
#include "sawtooth/st_transaction.h"
#include "helpers/hextobin.h"
#include "eth/sign.h"


TEST(TEST_SAWTOOTH, TEST_ENCODE_TX)
{
    st_tx_t tx;
    uint8_t buf[2048];
    st_privkey_t privkey;
    st_pubkey_t pubkey;
    st_address_t input, output;
    hextobin("846d5d0886020cd0733b0bb857400b33c4605da0dad322fe45114a1c59e67054", privkey.k, sizeof(privkey.k));
    privkey_to_pubkey(privkey.k, pubkey.k);

    memset(&tx, 0, sizeof(tx));
    strncpy((char*)&tx.header.family_name, "FAM1", sizeof(tx.header.family_name));
    strncpy((char*)&tx.header.family_version, "1.52", sizeof(tx.header.family_version));
    strncpy((char*)&tx.header.nonce, "42", sizeof(tx.header.nonce));
    hextobin(
            "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
            tx.header.payload_sha512, sizeof(tx.header.payload_sha512)
           );
    tx.header.signer_pubkey = pubkey;
    tx.header.batcher_pubkey = pubkey;
    hextobin(
            "1cf1266e282c41be5e4254d8820772c5518a2c5a8c0c7f7eda19594a7eb539453e1ed7",
            input.a, sizeof(input.a)
            );
    tx.header.inputs.lst = &input;
    tx.header.inputs.size = 1;
    hextobin(
            "239d56a63cb75e823314599115aa86553d9c7cd1abb51b32ed14484e06e220be5d2d3e",
            output.a, sizeof(output.a)
            );
    tx.header.outputs.lst = &output;
    tx.header.outputs.size = 1;

    ASSERT_GT(st_encode_header(&tx.header, buf, sizeof(buf)), 0);

    const char payload[] = "";
    int ret = 0;
    tx.payload.buf = (const uint8_t *)&payload;
    tx.payload.size = strlen(payload);
    ASSERT_EQ(st_sign_tx(&tx, &privkey, buf, sizeof(buf)), 0);
    ret = st_encode_batch(
            &tx, 1,
            &privkey, &pubkey,
            buf, sizeof(buf));
    ASSERT_GT(ret, 0);
#if 1
    FILE *fp = fopen("/tmp/koko", "w");
    fwrite(buf, 1, ret, fp);
    fclose(fp);
#endif
}
