import json
import re
from collections import defaultdict

import click
from eth_utils import encode_hex, function_abi_to_4byte_selector

from web3.utils.abi import abi_to_signature

from .constants import TYPE_MAPPINGS, Mapping
from .test_data import compile_contract_test


def generate_extern_c(data):
    return '''
#include "eth/data.h"
#include "eth/address.h"

#ifdef  __cplusplus
extern "C" {
#endif

%s
#ifdef __cplusplus
}
#endif
''' % data

def generate_header_guard(data, contract_name):
    contract_name = contract_name.upper()
    return f'''
#ifndef _{contract_name}_H
#define _{contract_name}_H
{data}
#endif /* _{contract_name}_H_ */
'''

def generate_unsupported_method():
    return '''
{
    UNSUPPORTED_METHOD();
}

'''

class UnsupportedDataType(Exception):
    pass

def map_type(t, var):
    mapping = TYPE_MAPPINGS[t]
    if mapping.array == 1:
        return '%s %s, size_t %s_len' % (mapping.type, var, var)
    else:
        return mapping.type +  ' ' + var

def generate_data_assert(t, var):
    l = t.replace('bytes', '')
    return f'assert({var}_len == {l});'

def generate_data_asserts(method):
    ret = ''
    for i in method['inputs']:
        mapping = TYPE_MAPPINGS.get(i['type'], None)
        if mapping is None:
            continue
        if mapping.array == 1 and mapping.dynamic == 0:
            ret += generate_data_assert(i['type'], i['name']) + '\n'
    return ret


def compute_data_len(method):
    ret = len(method['inputs'] * 32)
    for i in method['inputs']:
        if i['type'] in TYPE_MAPPINGS or re.match('^bytes\d+$', i['type']):
            continue
        raise UnsupportedDataType(i['type'])
    return ret


def compile_method_declaration(contract_name, method_name, method):
    assert method['type'] == 'function'
    ret = ""


    inputs = [
        map_type(inp['type'], inp['name'])
        for inp in method['inputs']
    ]
    inputs += ['data_block_t *out']
    inputs = ', '.join(inputs)
    ret += 'int %s_%s(%s)' % (
        contract_name,
        method_name,
        inputs,
    )
    return ret

def generate_assign_block(method):
    idx = 0
    # 1) generate basic params
    ret = ''
    dynamic_data = []
    for i in method['inputs']:
        var = i['name']
        m = Mapping('', 'default', 0, 0)
        mapping = TYPE_MAPPINGS.get(i['type'], m)
        if mapping.array == 1 and mapping.dynamic == 0:
            ret +=  f'{mapping.encode_fn}(out, {idx}, {var}, {var}_len);\n'
        elif mapping.array == 1 and mapping.dynamic == 1:
            ret += f'encode_uint(out, {idx}, (dyn_idx * 32) + 4); // location of {var}\n'
            ret += f'encode_uint(out, dyn_idx++, {var}_len); // length of {var}'
            ret += f'''
for(size_t i = 0; i < ({var}_len / DATA_ITEM_LEN); i++) {{
    {mapping.encode_fn}(out+DATA_ITEM_LEN, dyn_idx++, {var}, MIN(DATA_ITEM_LEN, {var}_len - (i*DATA_ITEM_LEN)));
    dyn_idx++;
}}
'''
        else:
            ret +=  f'{mapping.encode_fn}(out, {idx}, {var});\n'

        idx += 1
    ret += 'return (dyn_idx * 32) + 4;';
    return ret

def compile_method_definition(contract_name, method_name, method):
    selector = encode_hex(function_abi_to_4byte_selector(method))
    signature = abi_to_signature(method)
    data_asserts = generate_data_asserts(method).replace('\n', '\n' + 4*' ')
    assign_block = generate_assign_block(method).replace('\n', '\n' + 4*' ')
    try:
        data_len = compute_data_len(method)
    except UnsupportedDataType:
        data_len = None

    ret = compile_method_declaration(contract_name, method_name, method)
    if data_len is None:
        return ret + generate_unsupported_method()

    return ret + f'''
{{
    // {signature}
    uint32_t data_len __attribute__((unused)) = {data_len} + 4; // basic size of the arguments = len(args) * 32 + 4
    size_t dyn_idx = {data_len // 32}; // index of first dynamic item

    {data_asserts}
    assert(out != NULL);
    assert(out->data_len >= data_len + 4);
    data_set_selector(out, {selector});

    {assign_block}
}}
'''


def compile_contract_h(name, abi):
    ret = ''
    for method in abi['abi']:
        if method['type'] != 'function':
            continue
        ret += compile_method_declaration(name, method['name'], method) + ';\n'
    if ret == '':
        return ''
    ret = generate_extern_c(ret)
    ret = generate_header_guard(ret, name)
    return ret

def compile_contract_c(name, abi):
    ret = '''
#include <string.h>
#include <assert.h>
#include "eth/data.h"
#ifndef MIN
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#endif
'''
    compiled = defaultdict(int)
    for method in abi['abi']:
        if method['type'] != 'function':
            continue
        if method['name'] in compiled:
            method_name = method['name'] + f"_{compiled[method['name']]}"
        else:
            method_name = method['name']
        ret += compile_method_definition(name, method_name, method) + '\n'
        compiled[method_name] += 1
    ret = generate_extern_c(ret)
    return ret


@click.command()
@click.option(
    '--abi',
    required=True,
)
@click.option(
    '--output',
    type=click.Choice(['c', 'h', 'test']),
    default='h',
)
def main(abi, output):
    # read abi
    contracts = json.loads(open(abi, 'r').read())['contracts']

    # generate C code
    outputs_map = {
        'h': compile_contract_h,
        'c': compile_contract_c,
        'test': compile_contract_test,
    }

    for contract, abi in contracts.items():
        abi['abi'] = json.loads(abi['abi'])
        contract = contract.split(':', 1)[1]
        print(outputs_map[output](contract, abi))

    # ???

    # profit


if __name__ == "__main__":
    main()
