# -*- coding: utf-8 -*-
import pytest

from fixtures import client


@pytest.fixture
def created_playlist(request, client):
    index = client.create_playlist("test", 4, 1)

    def playlist_teardown():
        client.remove_playlist(index)
    request.addfinalizer(playlist_teardown)

    return index


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
    def playlist_finalizer():
        client.remove_playlist(playlist_index)
    request.addfinalizer(playlist_finalizer)

    # The romanian for "dirt".
    name = "țărână".encode()
    playlist_index = client.create_playlist(name, 12, 1)

    assert client.get_playlist_count() == 2

    actual_name = client.playlist_get_name(playlist_index)
    assert actual_name == name
