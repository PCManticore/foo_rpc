import multiprocessing.connection

import msgpack
import pytest


@pytest.fixture
def client():
    return multiprocessing.connection.Client('\\\\.\\pipe\\foobar2000')


def get_name():
    param = msgpack.packb(0)
    return msgpack.packb(["Playlist.playlist_get_name", param])


def playback_format_title_complete():
    param = msgpack.packb(0)
    return msgpack.packb(["PlaybackControl.playback_format_title_complete", param])


def play_or_pause():
    param = msgpack.packb(0)
    return msgpack.packb(["PlaybackControl.play_or_pause", param])


def set_volume(volume):
    param = msgpack.packb(volume)
    return msgpack.packb(["PlaybackControl.set_volume", param])


def next():
    param = msgpack.packb(0)
    return msgpack.packb(["PlaybackControl.next", param])


def start(enum):
    param = (enum, False)
    packed_param = msgpack.packb(param)
    return msgpack.packb(["PlaybackControl.start", packed_param])


def sort_by_format():
    format = "%length%"
    packed_param = msgpack.packb((format, False))
    return msgpack.packb(["UIEdit.sort_by_format", packed_param])


def reorder(order):
    order = msgpack.packb(order)
    return msgpack.packb(["Playlist.reorder", order])


def get_now_playing():
    return msgpack.packb(["PlaybackControl.get_now_playing"])


def insert_items(playlist, location, songs):
    param = msgpack.packb([playlist, location, songs])
    return msgpack.packb(["Playlist.playlist_insert_items", param])


def get_item_handle(playlist, item):
    param = msgpack.packb([playlist, item])
    return msgpack.packb(["Playlist.playlist_get_item_handle", param])


def create_playlist(name, length, playlist, songs):
    param = msgpack.packb([name, length, playlist, songs])
    return msgpack.packb(["Playlist.create_playlist_ex", param])


def add_items(playlist, songs):
    param = msgpack.packb([playlist, songs])
    return msgpack.packb(["Playlist.playlist_add_items", param])


def playlist_item_format_title(playlist, item):
    param = msgpack.packb([playlist, item, "[%artist%]"])
    return msgpack.packb(["Playlist.playlist_item_format_title", param])


def get_playing_item_location():
    param = msgpack.packb(0)
    return msgpack.packb(["Playlist.get_playing_item_location", param])


def play_sort_by_format(playlist, format, selection_only):
    param = msgpack.packb([playlist, format, selection_only])
    return msgpack.packb(["Playlist.playlist_sort_by_format", param])


def queue_add_item_playlist(playlist, item):
    param = msgpack.packb([playlist, item])
    return msgpack.packb(["Playlist.queue_add_item_playlist", param])


def queue_add_item(song):
    param = msgpack.packb(song)
    return msgpack.packb(["Playlist.queue_add_item", param])


def queue_get_contents():
    param = msgpack.packb(0)
    return msgpack.packb(["Playlist.queue_get_contents", param])


def queue_remove_mask(mask):
    param = msgpack.packb(mask)
    return msgpack.packb(["Playlist.queue_remove_mask", param])


def playlist_get_selection_mask():
    param = msgpack.packb(0)
    return msgpack.packb(["Playlist.playlist_get_selection_mask", param])


def load_playlist(playlist):
    param = msgpack.packb(playlist)
    return msgpack.packb(["PlaylistLoader.load_playlist", param])


def save_playlist(playlist, songs):
    param = msgpack.packb([playlist, songs])
    return msgpack.packb(["PlaylistLoader.save_playlist", param])


def format_title(item):
    param = msgpack.packb(item)
    return msgpack.packb(["MetadbHandle.format_title", param])


def is_playing():
    param = msgpack.packb(0)
    return msgpack.packb(["PlaybackControl.is_playing", param])


def determine_length(obj):
    if any(ord(c) > 127 for c in obj):
        return len(obj) * 2
    return len(obj)
