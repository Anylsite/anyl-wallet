pragma solidity ^0.5;

contract TestContract {
    function foo(uint16 a, uint8 b, uint32 c) public returns (uint64) {
        return a +b + c;
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
