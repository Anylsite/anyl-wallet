import json
import random
from binascii import hexlify

import click
from eth_abi import encode_abi
from eth_utils import encode_hex, function_abi_to_4byte_selector

from .utils import indent


def random_bytes(ret_len):
    return bytes([random.randint(0, 0xff) for _ in range(ret_len)])

data_generators = {
    'uint8': lambda : random.randint(0, 0xff),
    'uint16': lambda: random.randint(0, 0xffff),
    'uint32': lambda: random.randint(0, 0xffffffff),
    'uint64': lambda: random.randint(0, 0xffffffffffffffff),
    'uint128': lambda: random.randint(0, 0xffffffffffffffffffffffffffffffff),
    'uint256': lambda: random.randint(0, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff),
    'bytes32': lambda: random_bytes(32),
}

def gen_input(inputs):
    ret = []
    for i in inputs:
        gen = data_generators[i]
        ret.append(gen())
    return ret

def sanitize_data(data):
    ret = []
    for x in data:
        if isinstance(x, bytes):
            ret.append(x.hex())
        else:
            ret.append(x)
    return ret

def encode_call(test_data):
    params = ',\n'.join([str(d) for d in test_data])
    params += '\n, &block'
    return indent(params)

def encode_test_data(test_data):
    ret = ''
    for x in test_data:
        ret += '\\x%02x' % x
    return ret

def wrap_assert(assert_type, data, value):
    return f'''
{assert_type}(
    {data},
    {value}
);
'''

def gen_method_call(contract, method ,data):
    method_params = indent(encode_call(data))
    return f'''
encoded_len = {contract}_{method['name']}
(
{method_params}
)
'''

def gen_method_test(contract, method):
    inputs = [i['type'] for i in method['inputs']]
    data = gen_input(inputs)
    method_call = indent(gen_method_call(contract, method, data))
    method_call = indent(wrap_assert('ASSERT_GT', method_call, 0))
    compare_call = 'memcmp(test_buf, block.data, encoded_len)'
    compare_call = indent(wrap_assert('ASSERT_EQ', compare_call, 0))

    selector = function_abi_to_4byte_selector(method)
    encoded_data = encode_test_data(selector + encode_abi(inputs, data))
    return f"""
TEST(
TEST_ABI_COMPILER,
TEST_{contract.upper()}_{method['name'].upper()}
)
{{
    const size_t DATA_BUFSIZE = 1024;
    const uint8_t test_buf[] = "{encoded_data}";
    uint8_t data_buf[DATA_BUFSIZE];
    uint16_t encoded_len = 0;
    data_block_t block = {{DATA_BUFSIZE, data_buf}};
    {method_call}
    ASSERT_LE(encoded_len, MIN(sizeof(test_buf), DATA_BUFSIZE));
    {compare_call}
}}
"""


def compile_contract_test(contract, abi):
    ret = '''
#include <gtest/gtest.h>
#include "eth/data.h"
#include "contracts.h"
#include "helpers/macros.h"
'''
    for method in abi['abi']:
        if method['type'] != 'function':
            continue
#         sanitized = sanitize_data(data)
        ret += gen_method_test(contract, method)
    return ret
