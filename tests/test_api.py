import msgpack

from fixtures import client
from fixtures import get_name


def test_default_playlist(client):
    with client:
        client.send_bytes(get_name())
        result = client.recv_bytes()

        assert msgpack.unpackb(result) == b"Default"
