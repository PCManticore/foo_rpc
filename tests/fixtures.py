import inspect
import multiprocessing.connection
import types

import msgpack
import pytest



def _foobar_pipe_client():
    return multiprocessing.connection.Client('\\\\.\\pipe\\foobar2000')


class BaseAPIClient(type):

    def __new__(mcs, name, bases, attrs):
        def decorator(func):
            def wrapper(*args):
                 packed_data = func(*args)
                 with _foobar_pipe_client() as client:
                     client.send_bytes(packed_data)
                     received_result = client.recv_bytes()
                     return msgpack.unpackb(received_result)

            return wrapper


        cls = type.__new__(mcs, name, bases, attrs)
        for name, method in inspect.getmembers(cls, inspect.isfunction):
            decorated = decorator(method)
            setattr(cls, name, types.MethodType(decorated, cls))
        return cls                        


class APIClient(metaclass=BaseAPIClient):

    def get_playlist_count(self):
        return msgpack.packb(["Playlist.get_playlist_count"])

    def get_active_playlist(self):
        return msgpack.packb(["Playlist.get_active_playlist"])

    def set_active_playlist(self, p_index):
        params = msgpack.packb(p_index)
        return msgpack.packb(["Playlist.set_active_playlist", params])

    def get_playing_playlist(self):
        return msgpack.packb(["Playlist.get_playing_playlist"])

    def set_playing_playlist(self, p_index):
        params = msgpack.packb(p_index)
        return msgpack.packb(["Playlist.set_playing_playlist", params])

    def remove_playlists(self, p_mask):
        params = msgpack.packb(p_mask)
        return msgpack.packb(["Playlist.remove_playlists", params])

    def create_playlist(self, p_name, p_name_length, p_index):
        params = msgpack.packb([p_name, p_name_length, p_index])
        return msgpack.packb(["Playlist.create_playlist", params])

    def create_playlist_ex(self, p_name, p_name_length, p_index, content):
        params = msgpack.packb([p_name, p_name_length, p_index, content])
        return msgpack.packb(["Playlist.create_playlist_ex", params])

    def reorder(self, p_order, p_count):
        params = msgpack.packb([p_order, p_count])
        return msgpack.packb(["Playlist.reorder", params])

    def playlist_get_item_count(self, p_playlist):
        params = msgpack.packb(p_playlist)
        return msgpack.packb(["Playlist.playlist_get_item_count", params])

    def playlist_get_focus_item(self, p_playlist):
        params = msgpack.packb(p_playlist)
        return msgpack.packb(["Playlist.playlist_get_focus_item", params])

    def playlist_get_name(self, p_playlist, p_out):
        params = msgpack.packb([p_playlist, p_out])
        return msgpack.packb(["Playlist.playlist_get_name", params])

    def playlist_reorder_items(self, p_playlist, p_order, p_count):
        params = msgpack.packb([p_playlist, p_order, p_count])
        return msgpack.packb(["Playlist.playlist_reorder_items", params])

    def activeplaylist_reorder_items(self, order, count):
        params = msgpack.packb([order, count])
        return msgpack.packb(["Playlist.activeplaylist_reorder_items", params])

    def activeplaylist_remove_items(self, mask):
        params = msgpack.packb(mask)
        return msgpack.packb(["Playlist.activeplaylist_remove_items", params])

    def playlist_remove_items(self, p_playlist, mask):
        params = msgpack.packb([p_playlist, mask])
        return msgpack.packb(["Playlist.playlist_remove_items", params])

    def activeplaylist_replace_item(self, p_item, p_new_item):
        params = msgpack.packb([p_item, p_new_item])
        return msgpack.packb(["Playlist.activeplaylist_replace_item", params])

    def playlist_replace_item(self, p_playlist, p_item, p_new_item):
        params = msgpack.packb([p_playlist, p_item, p_new_item])
        return msgpack.packb(["Playlist.playlist_replace_item", params])

    def activeplaylist_insert_items(self, p_base, data, p_selection):
        params = msgpack.packb([p_base, data, p_selection])
        return msgpack.packb(["Playlist.activeplaylist_insert_items", params])

    def activeplaylist_add_items(data, p_selection):
        params = msgpack.packb([data, p_selection])
        return msgpack.packb(["Playlist.activeplaylist_add_items", params])

    def playlist_add_items(self, playlist, data, p_selection):
        params = msgpack.packb([playlist, data, p_selection])
        return msgpack.packb(["Playlist.playlist_add_items", params])

    def playlist_insert_items(self, p_playlist, p_base, data, p_selection):
        params = msgpack.packb([p_playlist, p_base, data, p_selection])
        return msgpack.packb(["Playlist.playlist_insert_items", params])

    def activeplaylist_rename(self, p_name, p_name_len):
        params = msgpack.packb([p_name, p_name_len])
        return msgpack.packb(["Playlist.activeplaylist_rename", params])

    def playlist_rename(self, p_index, p_name, p_name_length):
        params = msgpack.packb([p_index, p_name, p_name_length])
        return msgpack.packb(["Playlist.playlist_rename", params])

    def activeplaylist_item_format_title(self, p_item, p_script):
        params = msgpack.packb([p_item, p_script])
        return msgpack.packb(["Playlist.activeplaylist_item_format_title", params])

    def playlist_item_format_title(self, p_playlist, p_item, p_script):
        params = msgpack.packb([p_playlist, p_item, p_script])
        return msgpack.packb(["Playlist.playlist_item_format_title", params])

    def get_playing_item_location(self, p_playlist, p_index):
        params = msgpack.packb([p_playlist, p_index])
        return msgpack.packb(["Playlist.get_playing_item_location", params])

    def activeplaylist_sort_by_format(self, spec, p_sel_only):
        params = msgpack.packb([spec, p_sel_only])
        return msgpack.packb(["Playlist.activeplaylist_sort_by_format", params])

    def playlist_sort_by_format(self, p_playlist, p_pattern, p_sel_only):
        params = msgpack.packb([p_playlist, p_pattern, p_sel_only])
        return msgpack.packb(["Playlist.playlist_sort_by_format", params])

    def playback_order_get_count(self):
        return msgpack.packb(["Playlist.playback_order_get_count"])

    def playback_order_get_name(self, p_index):
        params = msgpack.packb(p_index)
        return msgpack.packb(["Playlist.playback_order_get_name", params])

    def playback_order_get_active(self):
        return msgpack.packb(["Playlist.playback_order_get_active"])

    def playback_order_set_active(self, p_index):
        params = msgpack.packb(p_index)
        return msgpack.packb(["Playlist.playback_order_set_active", params])

    def queue_add_item_playlist(self, p_playlist, p_item):
        params = msgpack.packb([p_playlist, p_item])
        return msgpack.packb(["Playlist.queue_add_item_playlist", params])

    def queue_add_item(self, p_item):
        params = msgpack.packb(p_item)
        return msgpack.packb(["Playlist.queue_add_item", params])

    def queue_get_count(self):
        return msgpack.packb(["Playlist.queue_get_count"])

    def queue_get_contents(self, p_out):
        params = msgpack.packb(p_out)
        return msgpack.packb(["Playlist.queue_get_contents", params])

    def queue_remove_mask(self, p_mask):
        params = msgpack.packb(p_mask)
        return msgpack.packb(["Playlist.queue_remove_mask", params])

    def queue_flush(self):
        return msgpack.packb(["Playlist.queue_flush"])

    def queue_is_active(self):
        return msgpack.packb(["Playlist.queue_is_active"])

    def remove_playlist(self, p_playlist):
        params = msgpack.packb(p_playlist)
        return msgpack.packb(["Playlist.remove_playlist", params])

    def remove_playlist_switch(self, p_playlist):
        params = msgpack.packb(p_playlist)
        return msgpack.packb(["Playlist.remove_playlist_switch", params])

    def activeplaylist_is_item_selected(self, p_item):
        params = msgpack.packb(p_item)
        return msgpack.packb(["Playlist.activeplaylist_is_item_selected", params])

    def playlist_is_item_selected(self, p_playlist, p_item):
        params = msgpack.packb([p_playlist, p_item])
        return msgpack.packb(["Playlist.playlist_is_item_selected", params])

    def activeplaylist_clear(self):
        return msgpack.packb(["Playlist.activeplaylist_clear"])

    def playlist_clear(self, p_playlist):
        params = msgpack.packb(p_playlist)
        return msgpack.packb(["Playlist.playlist_clear", params])

    def activeplaylist_clear_selection(self):
        return msgpack.packb(["Playlist.activeplaylist_clear_selection"])

    def playlist_clear_selection(self, p_playlist):
        params = msgpack.packb(p_playlist)
        return msgpack.packb(["Playlist.playlist_clear_selection", params])

    def activeplaylist_remove_selection(self, p_crop):
        params = msgpack.packb(p_crop)
        return msgpack.packb(["Playlist.activeplaylist_remove_selection", params])

    def playlist_remove_selection(self, p_playlist, p_crop):
        params = msgpack.packb([p_playlist, p_crop])
        return msgpack.packb(["Playlist.playlist_remove_selection", params])

    def activeplaylist_get_name(self, p_out):
        params = msgpack.packb(p_out)
        return msgpack.packb(["Playlist.activeplaylist_get_name", params])

    def activeplaylist_get_item_count(self):
        return msgpack.packb(["Playlist.activeplaylist_get_item_count"])

    def activeplaylist_get_focus_item(self):
        return msgpack.packb(["Playlist.activeplaylist_get_focus_item"])

    def create_playlist_autoname(self, p_index):
        params = msgpack.packb(p_index)
        return msgpack.packb(["Playlist.create_playlist_autoname", params])

    def reset_playing_playlist(self):
        return msgpack.packb(["Playlist.reset_playing_playlist"])

    def find_playlist(self, p_name, p_name_length):
        params = msgpack.packb([p_name, p_name_length])
        return msgpack.packb(["Playlist.find_playlist", params])

    def find_or_create_playlist(self, p_name, p_name_length):
        params = msgpack.packb([p_name, p_name_length])
        return msgpack.packb(["Playlist.find_or_create_playlist", params])

    def find_or_create_playlist_unlocked(self, p_name, p_name_length):
        params = msgpack.packb([p_name, p_name_length])
        return msgpack.packb(["Playlist.find_or_create_playlist_unlocked", params])

    def active_playlist_fix(self):
        return msgpack.packb(["Playlist.active_playlist_fix"])

    def playlist_activate_delta(self, p_delta):
        params = msgpack.packb(p_delta)
        return msgpack.packb(["Playlist.playlist_activate_delta", params])

    def playlist_activate_next(self):
        return msgpack.packb(["Playlist.playlist_activate_next"])

    def playlist_activate_previous(self):
        return msgpack.packb(["Playlist.playlist_activate_previous"])

    def playlist_get_selection_count(self, p_playlist, p_max):
        params = msgpack.packb([p_playlist, p_max])
        return msgpack.packb(["Playlist.playlist_get_selection_count", params])

    def activeplaylist_get_selection_count(self, p_max):
        params = msgpack.packb(p_max)
        return msgpack.packb(["Playlist.activeplaylist_get_selection_count", params])
     
    def activeplaylist_get_all_items(self, out):
        params = msgpack.packb(out)
        return msgpack.packb(["Playlist.activeplaylist_get_all_items", params])

    def playlist_get_all_items(self, p_playlist, out):
        params = msgpack.packb([p_playlist, out])
        return msgpack.packb(["Playlist.playlist_get_all_items", params])

    def activeplaylist_get_item_handle(self, p_item):
        params = msgpack.packb(p_item)
        return msgpack.packb(["Playlist.activeplaylist_get_item_handle", params])

    def playlist_get_item_handle(self, playlist, item):
        params = msgpack.packb([playlist, item])
        return msgpack.packb(["Playlist.playlist_get_item_handle", params])

    def activeplaylist_get_items(self, out, p_mask):
        params = msgpack.packb([out, p_mask])
        return msgpack.packb(["Playlist.activeplaylist_get_items", params])

    def playlist_get_items(self, p_playlist, out, p_mask):
        params = msgpack.packb([p_playlist, out, p_mask])
        return msgpack.packb(["Playlist.playlist_get_items", params])

    def playlist_insert_items_filter(self, p_playlist, p_base, p_data, p_select):
        params = msgpack.packb([p_playlist, p_base, p_data, p_select])
        return msgpack.packb(["Playlist.playlist_insert_items_filter", params])

    def get_version_string(self):
        return msgpack.packb(["CoreVersion.get_version_string"])

    def get_name(self):
        return msgpack.packb(["CoreVersion.get_name"])

    def get_version(self):
        return msgpack.packb(["CoreVersion.get_version"])

    def get_version_as_text(self):
        return msgpack.packb(["CoreVersion.get_version_as_text"])

    def test_version(self, major, minor1, minor2, minor3):
        params = msgpack.packb([major, minor1, minor2, minor3])
        return msgpack.packb(["CoreVersion.test_version", params])

    def format_title(self, p_playlist, p_item, p_script):
        params = msgpack.packb([p_playlist, p_item, p_script])
        return msgpack.packb(["MetadbHandle.format_title", params])

    def get_now_playing(self):
        return msgpack.packb(["PlaybackControl.get_now_playing"])

    def start(self, p_command, p_paused):
        params = msgpack.packb([p_command, p_paused])
        return msgpack.packb(["PlaybackControl.start", params])

    def stop(self):
        return msgpack.packb(["PlaybackControl.stop"])

    def is_playing(self):
        return msgpack.packb(["PlaybackControl.is_playing"])

    def is_paused(self):
        return msgpack.packb(["PlaybackControl.is_paused"])

    def pause(self, p_state):
        params = msgpack.packb(p_state)
        return msgpack.packb(["PlaybackControl.pause", params])

    def get_stop_after_current(self):
        return msgpack.packb(["PlaybackControl.get_stop_after_current"])

    def set_stop_after_current(self, p_state):
        params = msgpack.packb(p_state)
        return msgpack.packb(["PlaybackControl.set_stop_after_current", params])

    def set_volume(self, p_value):
        params = msgpack.packb(p_value)
        return msgpack.packb(["PlaybackControl.set_volume", params])

    def get_volume(self):
        return msgpack.packb(["PlaybackControl.get_volume"])

    def volume_up(self):
        return msgpack.packb(["PlaybackControl.volume_up"])

    def volume_down(self):
        return msgpack.packb(["PlaybackControl.volume_down"])

    def volume_mute_toggle(self):
        return msgpack.packb(["PlaybackControl.volume_mute_toggle"])

    def playback_seek(self, p_time):
        params = msgpack.packb(p_time)
        return msgpack.packb(["PlaybackControl.playback_seek", params])

    def playback_seek_delta(self, p_delta):
        params = msgpack.packb(p_delta)
        return msgpack.packb(["PlaybackControl.playback_seek_delta", params])

    def playback_can_seek(self):
        return msgpack.packb(["PlaybackControl.playback_can_seek"])

    def playback_get_position(self):
        return msgpack.packb(["PlaybackControl.playback_get_position"])

    def playback_format_title(self, p_script):
        params = msgpack.packb(p_script)
        return msgpack.packb(["PlaybackControl.playback_format_title", params])

    def playback_get_length(self):
        return msgpack.packb(["PlaybackControl.playback_get_length"])

    def playback_get_length_ex(self):
        return msgpack.packb(["PlaybackControl.playback_get_length_ex"])

    def toggle_stop_after_current(self):
        return msgpack.packb(["PlaybackControl.toggle_stop_after_current"])

    def toggle_pause(self):
        return msgpack.packb(["PlaybackControl.toggle_pause"])

    def play_or_pause(self):
        return msgpack.packb(["PlaybackControl.play_or_pause"])

    def play_or_unpause(self):
        return msgpack.packb(["PlaybackControl.play_or_unpause"])

    def previous(self):
        return msgpack.packb(["PlaybackControl.previous"])

    def next(self):
        return msgpack.packb(["PlaybackControl.next"])

    def is_muted(self):
        return msgpack.packb(["PlaybackControl.is_muted"])

    def get_volume_step(self):
        return msgpack.packb(["PlaybackControl.get_volume_step"])


@pytest.fixture
def client():
    return APIClient()
