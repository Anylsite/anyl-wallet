from collections import namedtuple

Mapping = namedtuple('Mapping', 'type encode_fn array dynamic')


TYPE_MAPPINGS = {
    'uint256': Mapping('uint256_t *', 'encode_uint256', 0, 0),
    'uint192': Mapping('uint256_t *', 'encode_uint256', 0, 0),
    'uint128': Mapping('uint128_t *', 'encode_uint128', 0, 0),
    'uint64': Mapping('uint64_t', 'encode_uint', 0, 0),
    'uint32': Mapping('uint32_t', 'encode_uint', 0, 0),
    'uint16': Mapping('uint16_t', 'encode_uint', 0, 0),
    'uint8': Mapping('uint8_t', 'encode_uint', 0, 0),
    'uint': Mapping('uint64_t', 'encode_uint', 0, 0),

    'int64': Mapping('int64_t', 'encode_int', 0, 0),
    'int32': Mapping('int32_t', 'encode_int', 0, 0),
    'int16': Mapping('int16_t', 'encode_int', 0, 0),
    'int8': Mapping('int8_t', 'encode_int', 0, 0),
    'int': Mapping('int64_t', 'encode_int', 0, 0),

    'address': Mapping('address_t *', 'encode_address', 0, 0),

    'bytes32': Mapping('uint8_t *', 'encode_data', 1, 0),
    'bytes': Mapping('uint8_t *', 'encode_data', 1, 1),
}
