import sys
from multiprocessing.connection import Client
import msgpack
import contextlib


def get_name():
    param = msgpack.packb(0)
    return msgpack.packb(["Playlist.playlist_get_name", param])


def test_default():
    with contextlib.closing(Client('\\\\.\\pipe\\foobar2000')) as client:
        l = msgpack.packb("")
        client.send_bytes(get_name())
        f = client.recv_bytes()
        assert msgpack.unpackb(f) == 'Default'

