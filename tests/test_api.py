# -*- coding: utf-8 -*-
import itertools
import os
import time
import tempfile

import pytest

from fixtures import (
    client,
    created_playlist,
    new_playlist_with_items,
    active_playlist_with_items,
    test_files,
    active_playlist_with_item,
    FoobarRPCError,
)


def test_default_playlist(client):
    playlist_name = client.activeplaylist_get_name()

    assert playlist_name == b"Default"


def test_create_playlist(client, created_playlist):

    assert created_playlist == 1


def test_create_playlist_and_count(client, created_playlist):

    assert created_playlist == 1

    # Now get the count and see if we actually created it.
    count = client.get_playlist_count()
    assert count == 2


def test_picking_active_playlist(client, created_playlist):

    active_playlist = client.get_active_playlist()
    assert active_playlist != created_playlist

    client.set_active_playlist(created_playlist)

    active_playlist = client.get_active_playlist()
    assert active_playlist == created_playlist

    # Go back to initial playlist
    client.set_active_playlist(0)


def test_remove_playlists(request, client):

    def playlists_finalizer():
        for playlist in playlists:
            client.remove_playlist(playlist)

    request.addfinalizer(playlists_finalizer)

    playlists = [client.create_playlist(letter, len(letter), index)
                 for index, letter in enumerate("testing", start=1)]

    # Check the count first.
    count = client.get_playlist_count()
    assert count == 8

    # Delete the first three and the last two
    masks = [1, 2, 3, 4, 8]
    client.remove_playlists(masks)

    new_count = client.get_playlist_count()
    assert new_count == 4

    # Delete the rest
    masks = [1, 2, 3]
    client.remove_playlists(masks)

    assert client.get_playlist_count() == 1


def test_create_playlist_with_unicode_name(request, client):

    request.addfinalizer(lambda: client.remove_playlist(playlist_index))

    # The romanian for "dirt".
    name = "țărână".encode()
    playlist_index = client.create_playlist(name, 12, 1)

    assert client.get_playlist_count() == 2

    actual_name = client.playlist_get_name(playlist_index)
    assert actual_name == name


def test_playlist_adding_items(client, new_playlist_with_items):
    count = client.playlist_get_item_count(new_playlist_with_items.index)

    assert count == len(new_playlist_with_items.files)


def test_create_playlist_ex(request, client, test_files):
    files = [test_file.path for test_file in test_files]
    playlist = client.create_playlist_ex("test", 4, 1, files)

    request.addfinalizer(lambda: client.remove_playlist(playlist))

    count = client.playlist_get_item_count(playlist)
    assert count == len(files)


def test_remove_items(client, new_playlist_with_items):
    mask = [1, 2, 4, 7]
    client.playlist_remove_items(new_playlist_with_items.index, mask)

    count = client.playlist_get_item_count(new_playlist_with_items.index)

    assert count == 2


def test_active_playlist_remove_items(client, active_playlist_with_items):
    mask = [1, 2, 4, 7]
    client.activeplaylist_remove_items(mask)

    count = client.playlist_get_item_count(client.get_active_playlist())

    assert count == 2


def test_playlist_rename(client, created_playlist):
    name_of_playlist = b"testing"
    length = len(name_of_playlist)
    client.playlist_rename(created_playlist, name_of_playlist, length)

    name = client.playlist_get_name(created_playlist)

    assert name == name_of_playlist


def test_active_playlist_rename(request, client):
    current_active_playlist_name = client.activeplaylist_get_name()
    current_playlist_name_length = len(current_active_playlist_name)
    request.addfinalizer(
        lambda: client.activeplaylist_rename(
            current_active_playlist_name,
            current_playlist_name_length)
    )

    name_of_playlist = b"testing"
    length = len(name_of_playlist)
    client.activeplaylist_rename(name_of_playlist, length)

    name = client.activeplaylist_get_name()

    assert name == name_of_playlist


def test_playlist_clear(client, new_playlist_with_items):
    client.playlist_clear(new_playlist_with_items.index)

    count = client.playlist_get_item_count(new_playlist_with_items.index)

    assert count == 0


def test_activeplaylist_clear(client, active_playlist_with_items):
    client.activeplaylist_clear()

    count = client.activeplaylist_get_item_count()

    assert count == 0


def test_get_now_playing(request, client, active_playlist_with_item):
    client.start(1, False)
    request.addfinalizer(lambda: client.stop())

    # Foobar needs time for getting the file and starting the actual playback.
    time.sleep(0.5)

    is_playing, now_playing = client.get_now_playing()

    assert is_playing

    now_playing_formatted = dict(now_playing)

    assert b'index' in now_playing_formatted
    assert b'path' in now_playing_formatted
    assert b'selected' in now_playing_formatted
    assert now_playing_formatted[b'index'] == 0
    assert now_playing_formatted[b'path'] == active_playlist_with_item.files[0].path.encode()
    assert now_playing_formatted[b'selected']


def test_is_playing(request, client, active_playlist_with_item):
    assert not client.is_playing()

    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    # Foobar needs time for getting the file and starting the actual playback.
    time.sleep(0.5)

    assert client.is_playing()


def test_is_paused(request, client, active_playlist_with_item):
    # Initially it is not paused
    assert not client.is_paused()

    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    # Foobar needs time for getting the file and starting the actual playback.
    time.sleep(0.5)

    client.pause(True)
    assert client.is_paused()

    client.pause(False)
    assert not client.is_paused()


def test_volume_operations(client):
    initial_volume = client.get_volume()
    assert initial_volume == 0.0

    client.volume_down()

    current_volume = client.get_volume()
    assert current_volume == -1.0

    client.volume_up()
    assert initial_volume == client.get_volume()


def test_play_or_pause(request, client, active_playlist_with_item):
    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    time.sleep(0.5)

    # Now it should be started.
    assert client.is_playing()
    client.play_or_pause()
    assert client.is_paused()

    client.play_or_pause()
    assert not client.is_paused()
    assert client.is_playing()


def test_play_or_unpause(request, client, active_playlist_with_item):
    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    time.sleep(0.5)

    # Now it should be started.
    assert client.is_playing()
    client.play_or_unpause()
    assert not client.is_paused()
    assert client.is_playing()

    client.pause(True)
    assert client.is_paused()

    client.play_or_unpause()
    assert not client.is_paused()
    assert client.is_playing()


def test_get_volume_step(client):
    step = client.get_volume_step()

    assert step == 1.0


def test_set_volume(request, client):
    original_volume = client.get_volume()
    request.addfinalizer(lambda: client.set_volume(original_volume))

    client.set_volume(-2)

    volume = client.get_volume()
    assert volume == -2.0


def test_stop_after_current(request, client):
    stop_after_current = client.get_stop_after_current()
    request.addfinalizer(lambda: client.set_stop_after_current(stop_after_current))

    assert not stop_after_current

    client.set_stop_after_current(True)
    assert client.get_stop_after_current()


def test_volume_mute_toggle(request, client):
    request.addfinalizer(lambda: client.volume_mute_toggle() if client.is_muted() else None)

    client.volume_mute_toggle()
    assert client.is_muted()

    client.volume_mute_toggle()
    assert not client.is_muted()


def test_toggle_stop_after_current(request, client):
    stop_after_current = client.get_stop_after_current()
    request.addfinalizer(lambda: client.set_stop_after_current(stop_after_current))

    client.toggle_stop_after_current()
    assert client.get_stop_after_current()

    client.toggle_stop_after_current()
    assert not client.get_stop_after_current()


def test_toggle_pause(request, client, active_playlist_with_item):
    client.start(1, False)
    request.addfinalizer(lambda: client.toggle_pause() if client.is_paused() else None)
    request.addfinalizer(lambda: client.stop())
    time.sleep(0.5)

    assert not client.is_paused()
    client.toggle_pause()
    assert client.is_paused()

    client.toggle_pause()
    assert not client.is_paused()


def test_previous_next(request, client, active_playlist_with_items):
    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    time.sleep(0.5)

    now_playing = dict(client.get_now_playing()[1])
    first = active_playlist_with_items.files[0]
    second = active_playlist_with_items.files[1]

    assert now_playing[b'path'] == first.path.encode()

    client.next()
    time.sleep(0.5)

    now_playing = dict(client.get_now_playing()[1])
    assert now_playing[b'path'] == second.path.encode()

    client.previous()
    time.sleep(0.5)

    now_playing = dict(client.get_now_playing()[1])
    assert now_playing[b'path'] == first.path.encode()


def test_previous_when_at_first(request, client, active_playlist_with_items):
    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    time.sleep(0.5)

    client.previous()
    time.sleep(0.5)

    playing, current_playing = client.get_now_playing()
    assert not playing
    assert not current_playing


def test_playlist_get_item_handle(client, new_playlist_with_items):
    handle = dict(client.playlist_get_item_handle(new_playlist_with_items.index, 0))

    first = new_playlist_with_items.files[0]

    assert handle[b'path'] == first.path.encode()
    assert handle[b'index'] == 0

    missing = client.playlist_get_item_handle(
        new_playlist_with_items.index,
        len(new_playlist_with_items.files) + 1)
    assert not missing


def test_activeplaylist_get_item_handle(client, active_playlist_with_item):
    handle = dict(client.activeplaylist_get_item_handle(0))

    first = active_playlist_with_item.files[0]

    assert handle[b'path'] == first.path.encode()
    assert handle[b'index'] == 0


def test_playlist_get_all_items(client, new_playlist_with_items):
    items = client.playlist_get_all_items(new_playlist_with_items.index)
    for index, (handle, test_file) in enumerate(zip(items, new_playlist_with_items.files)):
        song = dict(handle)
        assert song[b'path'] == test_file.path.encode()
        assert song[b'index'] == index


def test_activeplaylist_get_all_items(client, active_playlist_with_items):
    items = client.activeplaylist_get_all_items()
    for index, (handle, test_file) in enumerate(zip(items, active_playlist_with_items.files)):
        song = dict(handle)
        assert song[b'path'] == test_file.path.encode()
        assert song[b'index'] == index


def test_playlist_get_items(client, new_playlist_with_items):
    items = client.playlist_get_items(new_playlist_with_items.index, [0, 1, 4])
    files = list(itertools.compress(new_playlist_with_items.files, [1, 1, 0, 0, 1]))

    for index, (handle, test_file) in enumerate(zip(items, files)):
        song = dict(handle)
        assert song[b'path'] == test_file.path.encode()
        assert song[b'index'] == index


def test_activeplaylist_get_items(client, active_playlist_with_items):
    items = client.activeplaylist_get_items([0, 1, 4])
    files = list(itertools.compress(active_playlist_with_items.files, [1, 1, 0, 0, 1]))

    for index, (handle, test_file) in enumerate(zip(items, files)):
        song = dict(handle)
        assert song[b'path'] == test_file.path.encode()
        assert song[b'index'] == index


def test_create_playlist_autoname(request, client):
    playlist = client.create_playlist_autoname(1)
    request.addfinalizer(lambda: client.remove_playlist(playlist))

    name = client.playlist_get_name(playlist)
    assert name == b'New Playlist'


def test_cannot_find_playlist(client):
    playlist = client.find_playlist("test", 4)
    assert playlist >= 2 ** 32 -1


def test_find_playlist(client):
    playlist = client.find_playlist("Default", 7)
    assert playlist == 0


def test_find_or_create_playlist(request, client):
    playlist = client.find_or_create_playlist("test", 4)
    request.addfinalizer(lambda: client.remove_playlist(playlist))

    assert playlist == 1


def test_find_or_create_playlist_unlocked(request, client):
    playlist = client.find_or_create_playlist_unlocked("test", 4)
    request.addfinalizer(lambda: client.remove_playlist(playlist))

    assert playlist == 1


def test_find_or_create_playlist_already_exists(client):
    playlist = client.find_or_create_playlist("Default", 7)

    assert playlist == 0


def test_queue_operations(request, client, active_playlist_with_items,
                          new_playlist_with_items):

    first = active_playlist_with_items.files[0]
    second = active_playlist_with_items.files[1]
    third = active_playlist_with_items.files[2]
    fourth = new_playlist_with_items.files[1]

    initial_count = client.queue_get_count()
    assert initial_count == 0

    client.queue_add_item_playlist(0, 0)
    client.queue_add_item_playlist(0, 2)
    client.queue_add_item_playlist(0, 1)
    client.queue_add_item_playlist(0, 0)
    client.queue_add_item_playlist(new_playlist_with_items.index, 1)
    request.addfinalizer(lambda: client.queue_flush())

    count = client.queue_get_count()
    assert count == 5

    contents = client.queue_get_contents()

    expected_pairs = [(0, 0), (0, 2), (0, 1), (0, 0), (1, 1)]
    actual_pairs = [(item[b'playlist'], item[b'item']) for item in contents]

    assert expected_pairs == actual_pairs


def test_playlist_reorder_items(client, new_playlist_with_items):
    order = [4, 3, 2, 1, 0]
    client.playlist_reorder_items(new_playlist_with_items.index, order)

    items = client.playlist_get_all_items(new_playlist_with_items.index)
    for position, item in zip(order, items):
        remote_item = dict(item)
        local_item = new_playlist_with_items.files[position]

        assert local_item.path.encode() == remote_item[b'path']


def test_activeplaylist_reorder_items(client, active_playlist_with_items):
    order = [4, 2, 3, 0, 1]
    client.activeplaylist_reorder_items(order)

    items = client.activeplaylist_get_all_items()
    for position, item in zip(order, items):
        remote_item = dict(item)
        local_item = active_playlist_with_items.files[position]

        assert local_item.path.encode() == remote_item[b'path']


def test_reorder(request, client):

    playlists = [client.create_playlist(letter, 1, index)
                 for index, letter in enumerate("testing", start=1)]
    request.addfinalizer(lambda: client.remove_playlists(list(range(1, 8))))
    client.reorder([0, 2, 3, 4, 7, 5, 6, 1])

    # Now the playlists should be reordered
    order = "estgint"
    for index, letter in enumerate(order, start=1):
        assert letter.encode() == client.playlist_get_name(index)


def test_activeplaylist_replace_item(client, active_playlist_with_items,
                                     active_playlist_with_item):

    count = client.activeplaylist_get_item_count()

    original_item = dict(client.activeplaylist_get_item_handle(0))

    client.activeplaylist_replace_item(0, count - 1)

    replaced_item = dict(client.activeplaylist_get_item_handle(0))
    replaced_with_item = dict(client.activeplaylist_get_item_handle(count - 1))

    assert replaced_item[b'path'] != original_item[b'path']
    assert replaced_item[b'path'] == replaced_with_item[b'path']


def test_queue_remove_mask(request, client, active_playlist_with_items):
    client.queue_add_item_playlist(0, 0)
    client.queue_add_item_playlist(0, 2)
    client.queue_add_item_playlist(0, 1)

    request.addfinalizer(lambda: client.queue_flush())

    client.queue_remove_mask([0])

    contents = client.queue_get_contents()
    expected_items = [2, 1]
    actual_items = [item[b'item'] for item in contents]

    assert expected_items == actual_items


def test_queue_flush(request, client, active_playlist_with_items):
    client.queue_add_item_playlist(0, 0)
    client.queue_add_item_playlist(0, 2)
    client.queue_add_item_playlist(0, 1)

    request.addfinalizer(lambda: client.queue_remove_mask([0, 1, 2]))

    client.queue_flush()

    contents = client.queue_get_contents()
    assert not contents


def test_queue_is_active(request, client, active_playlist_with_item):
    request.addfinalizer(lambda: client.queue_flush())

    assert not client.queue_is_active()

    client.queue_add_item_playlist(active_playlist_with_item.index, 0)

    assert client.queue_is_active()


def test_get_playing_item_location(request, client, active_playlist_with_item):
    client.start(1, False)
    request.addfinalizer(lambda: client.stop())
    time.sleep(0.5)

    success, playlist, item = client.get_playing_item_location(
        active_playlist_with_item.index, 0)

    assert playlist == active_playlist_with_item.index
    assert item == 0


def test_playback_order_get_name(client):
    names = list(map(client.playback_order_get_name, range(7)))
    expected_names = [b'Default', b'Repeat (playlist)',
                      b'Repeat (track)', b'Random',
                      b'Shuffle (tracks)', b'Shuffle (albums)',
                      b'Shuffle (folders)']

    assert names == expected_names


def test_playback_order_get_count(client):
    assert client.playback_order_get_count() == 7


def test_playback_order(request, client):
    current_playback_order = client.playback_order_get_active()
    request.addfinalizer(lambda: client.playback_order_set_active(current_playback_order))

    client.playback_order_set_active(3)
    assert client.playback_order_get_active() == 3


def test_remove_playlist(client, created_playlist):

    assert client.find_playlist("test", 4) == created_playlist

    client.remove_playlist(created_playlist)

    assert client.find_playlist("test", 4) >= 2 ** 32 - 1


def test_remove_playlist_switch(request, client, created_playlist, new_playlist_with_items):
    request.addfinalizer(lambda: client.set_active_playlist(0))

    client.set_active_playlist(created_playlist)
    client.remove_playlist_switch(created_playlist)

    assert client.get_active_playlist() == new_playlist_with_items.index


def test_playlist_activate_ops(request, client):
    playlists = list(map(client.create_playlist_autoname, range(1, 3)))

    request.addfinalizer(lambda: client.set_active_playlist(0))
    request.addfinalizer(lambda: client.remove_playlists(playlists))

    client.playlist_activate_next()
    assert client.get_active_playlist() == 1

    client.playlist_activate_next()
    assert client.get_active_playlist() == 2

    client.playlist_activate_previous()
    assert client.get_active_playlist() == 1


def test_foobar_properties(client):
    version = client.get_version()
    version_string = client.get_version_string()
    name = client.get_name()
    version_text = client.get_version_as_text()

    # Can't make any assumption about the current version, so just verify some general things.
    assert isinstance(version_string, bytes)
    assert isinstance(version, list)
    assert name == b'foobar2000'
    assert isinstance(version_text, bytes)


def test_activeplaylist_item_format_title(client, active_playlist_with_item):
    formatted = client.activeplaylist_item_format_title(0, b"%title%")
    test_file = active_playlist_with_item.files[0].path
    base_name = os.path.basename(test_file)
    actual_name = os.path.splitext(base_name)[0]

    # We have no metadata, so it will default to the file name.
    assert formatted == actual_name.encode()


def test_playlist_item_format_title(client, new_playlist_with_items):
    formatted = client.playlist_item_format_title(new_playlist_with_items.index, 0, b"%title%")
    test_file = new_playlist_with_items.files[0].path
    base_name = os.path.basename(test_file)
    actual_name = os.path.splitext(base_name)[0]

    # We have no metadata, so it will default to the file name.
    assert formatted == actual_name.encode()


def test_playback_get_length(request, client, active_playlist_with_item):
    request.addfinalizer(lambda: client.stop())
    client.start(1, False)
    time.sleep(0.5)

    length = client.playback_get_length()
    length_ex = client.playback_get_length_ex()

    assert length == length_ex == active_playlist_with_item.files[0].length


def test_playback_seek_ops(request, client, active_playlist_with_item):
    request.addfinalizer(lambda: client.stop())
    client.start(1, False)
    time.sleep(0.5)

    assert client.playback_can_seek()

    client.playback_seek(2)
    position = client.playback_get_position()
    assert position >= 2

    client.playback_seek_delta(5)
    position = client.playback_get_position()
    assert position >= 7

    client.playback_seek_delta(-4)
    position = client.playback_get_position()
    assert 3 <= position < 7


def test_playing_playlist_ops(request, client, active_playlist_with_item,
                              new_playlist_with_items):
    request.addfinalizer(lambda: client.stop())
    request.addfinalizer(lambda: client.set_active_playlist(0))

    client.start(1, False)
    time.sleep(0.5)

    active_playlist = client.get_active_playlist()
    assert active_playlist == 0

    client.set_active_playlist(1)
    client.set_playing_playlist(1)
    client.stop()

    active_playlist = client.get_active_playlist()
    assert active_playlist == 1

    client.play_or_pause()
    time.sleep(0.5)

    _, handle = client.get_now_playing()
    formatted = dict(handle)
    assert formatted[b'path'] == new_playlist_with_items.files[0].path.encode()

    client.set_active_playlist(0)
    client.reset_playing_playlist()

    assert client.get_playing_playlist() == 0


def test_playlist_loading_saving(request, client, test_files):
    path = os.path.join(tempfile.gettempdir(), 'playlist.fpl')
    request.addfinalizer(lambda: os.remove(path))
    request.addfinalizer(lambda: client.remove_playlist(1))

    client.save_playlist(path, [test_file.path for test_file in test_files])
    assert os.path.exists(path)

    client.load_playlist(path)

    assert client.get_playlist_count() == 2

    contents = client.playlist_get_all_items(1)
    assert len(contents) == len(test_files)

    expected_paths = [test_file.path.encode() for test_file in test_files]
    actual_paths = [dict(item)[b'path'] for item in contents]

    assert expected_paths == actual_paths


def test_error_path_bad_arguments(client):
    with pytest.raises(FoobarRPCError) as error:
        client.create_playlist("a", "a", "a")

    assert str(error.value) == 'bad cast'


def test_error_path_missing_method(client):
    for meth in client._unknown_class, client._unknown_method:
        with pytest.raises(FoobarRPCError) as error:
            meth()
        assert str(error.value) == "Cannot find the given method."
