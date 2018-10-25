import http.client
import json
import click

def get_inputs(address, api_key=''):
    conn = http.client.HTTPSConnection("api-rinkeby.etherscan.io")

    conn.request(
        "GET",
        f"/api?module=account&action=txlist&address={address}"
        "&startblock=0&endblock=99999999&page=1&sort=asc&apikey={api_key}"
    )

    r = conn.getresponse()
    data = r.read()
    return json.loads(data)


@click.command()
@click.option(
    '--address',
    required=True,
)
@click.option(
    '--api-key',
    default='',
    required=False,
)
def main(address, api_key):
    inputs = get_inputs(address)
    values = [
        (tx['blockNumber'], tx['from'], tx['to'], tx['input'])
        for tx in inputs['result']
        if tx['input'] != '0x'  # exclude empty inputs
    ]
    for blknum, f, t, val in values:
        print(f"{blknum}\t{f}\t{t}\t{val}")

if __name__ == "__main__":
    main()
