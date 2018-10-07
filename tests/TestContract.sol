pragma solidity ^0.4.23;

contract TestContract {
    function foo(uint16 a, uint8 b, uint32 c) returns (uint64) {
        return a +b + c;
    }
    function function_int128(uint64 a, uint64 b) public view returns (uint128) {
        uint128 x = a + b;
        return x;
    }
    function function_int256(uint64 a, uint64 b, uint64 c, uint64 d) public view returns (uint256) {
        uint256 x = a + b + c + d;
        return x;
    }
//     function function_int(uint8 a, uint16 b, uint32 c, uint64 d, uint128 e, uint256 f) public view returns (uint256) {
//         uint256 x = a + b + c + d + e + f;
//         return x;
//     }

//     function function_int_bytes(uint64 a, uint256 b, bytes32 c) public view returns (uint256)
//     {
//         uint256 x = a + b;
//         return x;
//     }



}
