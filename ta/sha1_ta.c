#include <inttypes.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <sha1_ta.h>

TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t ptype,
				    TEE_Param param[4],
				    void **session_id_ptr)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                                TEE_PARAM_TYPE_NONE,
                                                TEE_PARAM_TYPE_NONE,
                                                TEE_PARAM_TYPE_NONE);

    DMSG("has been called");

    if (ptype != exp_param_types)
        return TEE_ERROR_BAD_PARAMETERS;

    (void)&param;
    (void)&session_id_ptr;
    
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ptr)
{
	(void)&sess_ptr;
    DMSG("has been called");
}

static void print_buffer(const char *buf, uint32_t len)
{
    uint32_t i;
    for (i=0; i<len; i++)
    {
        DMSG("0x%02x, ", buf[i]);
        if (i%0x10 == 15)
            DMSG("\n");
    }
    DMSG("\n");
}

static int sha1_digest(char *input, uint32_t in_len, char *output, uint32_t *out_len)
{
    TEE_Result ret;
    TEE_OperationHandle handle;
    uint32_t algorithm_id;

    DMSG("do sha1 digest operation\n");

    algorithm_id = TEE_ALG_SHA1;

    ret = TEE_AllocateOperation(&handle, algorithm_id, TEE_MODE_DIGEST, 0);
    if (ret != TEE_SUCCESS)
    {
        DMSG("Allocate SHA1 operate handle failed\n");
        return -1;
    }

    TEE_DigestUpdate(handle, input, in_len);

    ret = TEE_DigestDoFinal(handle, NULL, 0, output, out_len);
    if (ret != TEE_SUCCESS)
    {
        DMSG("Digest Final failed\n");
        return -1;
    }

    DMSG("Digest data:\n");
    print_buffer(output, *out_len);

    TEE_FreeOperation(handle);

    return 0;
}

static sha1_operation(uint32_t paramTypes, TEE_Param params[4])
{
    char *input = NULL;
    char *output = NULL;
    uint32_t in_len = 0;
    uint32_t out_len = 0;

    uint32_t temp;

    temp = paramTypes;

    DMSG("%d\n", temp);

    input = params[0].memref.buffer;
    in_len = params[0].memref.size;
    output = params[1].memref.buffer;
    out_len = params[1].memref.size;

    DMSG("Input Data:\n");
    print_buffer(input, in_len);

    sha1_digest(input, in_len, output, &out_len);

    return 0;
}

TEE_Result TA_InvokeCommandEntryPoint(void *session_id,
				      uint32_t command_id,
				      uint32_t parameters_type,
				      TEE_Param parameters[4])
{
	(void)&session_id;
 
    switch (command_id)
    {
    case TA_SHA1_CMD_INIT:
        DMSG("TA_SHA1_CMD_INIT");
        break;
    case TA_SHA1_CMD_UPDATE:
        DMSG("TA_SHA1_CMD_UPDATE");
        sha1_operation(parameters_type, parameters);
        break;
    case TA_SHA1_CMD_FINAL:
        DMSG("TA_SHA1_CMD_FINAL");
        break;
    default:
        return TEE_ERROR_BAD_PARAMETERS;
    }

	return TEE_SUCCESS;
}

