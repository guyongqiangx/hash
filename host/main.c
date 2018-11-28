#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the UUID (found the TA's h-file(s)) */
#include <sha1_ta.h>

/*
 * $ echo -n "I LOVE CHINA!" | sha1sum           
 * 42e3308180c2d2bbe655532e86a1bca5b5518554  -
*/

/* string: "I LOVE CHINA!" */
static char test_data[] = {'I', ' ', 'L', 'O', 'V', 'E', ' ', 'C', 'H', 'I', 'N', 'A', '!'};

/* 160 bits: 42e3308180c2d2bbe655532e86a1bca5b5518554 */
static char sha1_result[]= {0x42, 0xe3, 0x30, 0x81, 0x80, 0xc2, 0xd2, 0xbb,
                            0xe6, 0x55, 0x53, 0x2e, 0x86, 0xa1, 0xbc, 0xa5,
                            0xb5, 0x51, 0x85, 0x54};

static void print_buffer(const char *buf, uint32_t len)
{
    uint32_t i;
    for (i=0; i<len; i++)
    {
        printf("0x%02x, ", buf[i]);
        if (i%0x10 == 15)
            printf("\n");
    }
    printf("\n");
}

#define SHA1_DIGEST_SIZE 20

int main(void)
{
    TEEC_Result res;
    TEEC_Context ctx;
    TEEC_Session se;
    TEEC_Operation op;
    TEEC_UUID uuid = TA_SHA1_UUID;

    uint32_t err_origin;

    char sha1_digest[SHA1_DIGEST_SIZE];
    memset(sha1_digest, 0, SHA1_DIGEST_SIZE);

    printf("SHA1 test begins...\n");

    res = TEEC_InitializeContext(NULL, &ctx);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

    res = TEEC_OpenSession(&ctx, &se, &uuid,
        TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_OpenSession failed with code 0x%x origin 0x%x", res, err_origin);

    memset(&op, 0, sizeof(op));
    op.started = 1;
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, 
                                    TEEC_MEMREF_TEMP_OUTPUT,
                                    TEEC_NONE,
                                    TEEC_NONE);
    op.params[0].tmpref.size = sizeof(test_data);
    op.params[0].tmpref.buffer = &test_data;
    op.params[1].tmpref.size = SHA1_DIGEST_SIZE;
    op.params[1].tmpref.buffer = &sha1_digest;

    printf("sha1 data: \n");
    print_buffer(test_data, sizeof(test_data));

    printf("begin to do sha1 digest...\n");
 
    res = TEEC_InvokeCommand(&se, TA_SHA1_CMD_UPDATE, &op, &err_origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);

    printf("finished to do sha1 digest!\n");

    printf("expected data: \n");
    print_buffer(sha1_result, sizeof(sha1_result));

    printf("digest from ta:\n");
    print_buffer(sha1_digest, sizeof(sha1_digest));

    TEEC_CloseSession(&se);
    TEEC_FinalizeContext(&ctx);

    printf("SHA1 test done!\n");
    return 0;
}
