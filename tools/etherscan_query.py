import http.client
import json
import click

def get_inputs(rpc, address, startblock, api_key=''):
    conn = http.client.HTTPSConnection(rpc)

    conn.request(
        "GET",
        f"/api?module=account&action=txlist&address={address}"
        f"&startblock={startblock}&page=1&sort=asc&apikey={api_key}"
    )

    r = conn.getresponse()
    data = r.read()
    return json.loads(data)

def sanitize_inputs(inputs):
    known_txh = []
    new_result = []
    for x in inputs['result']:
        if x['hash'] not in known_txh:
            new_result.append(x)
            known_txh.append(x['hash'])

    inputs['result'] = new_result
    return inputs

def format_hex(values):
    for blknum, f, t, val in values:
        print(f"{blknum}\t{f}\t{t}\t{val}")

def format_temp(values):
    print("#blknum\ttxhash\t\t\t\t\t\t\t\t\ttemp\thumidity")
    for blknum, f, t, txhash, val in values:
        temperature = int(val[2:4], 16)
        humidity = int(val[4:7], 16)
        print(f"{blknum}\t{txhash}\t{temperature}\t{humidity}")

@click.command()
@click.option(
    '--rpc',
    default="api-rinkeby.etherscan.io",
)
@click.option(
    '--address',
    required=True,
)
@click.option(
    '--api-key',
    default='',
    required=False,
)
@click.option(
    '--decode',
    default='hex',
    type=click.Choice(['temp', 'hex']),
)
@click.option(
    '--startblock',
    default=0,
    type=int,
    required=False,
)
def main(rpc, address, api_key, decode, startblock):
    inputs = get_inputs(rpc, address, startblock, api_key)
    inputs = sanitize_inputs(inputs)
    values = [
        (tx['blockNumber'], tx['from'], tx['to'], tx['hash'], tx['input'])
        for tx in inputs['result']
        if tx['input'] != '0x'  # exclude empty inputs
    ]
    if decode == "hex":
        format_hex(values)
    elif decode == "temp":
        format_temp(values)

if __name__ == "__main__":
    main()
