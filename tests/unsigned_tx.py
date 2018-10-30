from eth_account.internal.transactions import UnsignedTransaction, Transaction
from eth_utils import decode_hex, encode_hex, to_checksum_address
from coincurve import PrivateKey
import rlp
import sys
tx = UnsignedTransaction(1, 1, 300, decode_hex(to_checksum_address('0x7e5f4552091a69125d5dfcb7b8c2659029395bdf')), 1, b'')
print("unsigned: " + encode_hex(rlp.encode(tx)))
value = 'Hello'
#update the path to the anyledger-wallet/tests/ folder
saveFile = open('/home/vishav/anyledger-wallet/tests/data.txt','w')
saveFile.write(encode_hex(rlp.encode(tx)))

# sign (for key 000.00001)
k = PrivateKey.from_hex(31*'00' + '01')
sign = k.sign_recoverable(rlp.encode(tx))
(r, s, v) = int.from_bytes(sign[:32], byteorder='big'), int.from_bytes(sign[32:64], byteorder='big'), sign[64]
signed_tx = Transaction(*tx.as_dict().values(), r, s, v)
print("signed: " + encode_hex(rlp.encode(signed_tx)))
saveFile.write('\n')
saveFile.write(encode_hex(rlp.encode(signed_tx)))
saveFile.close()
