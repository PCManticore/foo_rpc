.. _api:

foo_rpc API methods
===================

This section contains a thorough description of each method
that **foo_rpc** accepts. The parameters of the methods are expected
to be packed with msgpack_.


.. function:: CoreVersion.get_name()

   :returns: string

   Return the name of the program, e.g. "foobar2000".

.. function:: CoreVersion.get_version_as_text()

   :returns: string

   Return the version string of the program, in the form "N.N.N.N"

.. function:: PlaylistLoader.load_playlist(string playlist)

   :returns: bool

   Attempts to load a playlist file from specified filesystem path.

.. function:: PlaylistLoader.save_playlist(string playlist_path, vector<string> files)

   :returns: bool

   Saves specified list of locations into a playlist file.

.. function:: PlaybackControl.get_now_playing()

   :returns: tuple<bool, optional<Track>>

   Retrieves now playing item handle. Returns true on success, false on failure (not playing).

.. function:: PlaybackControl.start(tuple<play_control::t_track_command, bool>)

   Starts playback. If playback is already active, existing process is stopped first.

   The first parameter specifies what track to start playback from.
   See t_track_Command enum from foobar2000's SDK for more info.

   The second parameter specifies whether playback should be started as paused.

.. function:: PlaybackControl.stop()

   Stops playback.

.. function:: PlaybackControl.is_playing()

   :returns: bool

   Returns whether playback is active.

.. function:: PlaybackControl.is_paused()

   :returns: bool

   Returns whether playback is active and in paused state.

.. function:: PlaybackControl.pause(bool p_state)

   Toggles pause state if playback is active.

   :param p_state: set to true when pausing or to false when unpausing.

.. function:: PlaybackControl.get_stop_after_current()

   :returns: bool

   Retrieves stop-after-current-track option state.

.. function:: PlaybackControl.set_stop_after_current(bool p_state)

   Alters stop-after-current-track option state.

.. function:: PlaybackControl.set_volume(float p_value)

   Alters playback volume level.
   :param p_value: volume in dB; 0 for full volume.

.. function:: PlaybackControl.get_volume()

   :returns: float

   Retrieves playback volume level. Returns current playback volume
   level, in dB; 0 for full volume.

.. function:: PlaybackControl.volume_up()

   Alters playback volume level one step up.

.. function:: PlaybackControl.volume_down()

   Alters playback volume level one step down.

.. function:: PlaybackControl.volume_mute_toggle()

   Toggles playback mute state.

.. function:: PlaybackControl.playback_seek(double p_time)

   Seeks in currenly played track to specified time.
   :param p_time: target time in seconds.

.. function:: PlaybackControl.playback_seek_delta(double p_delta)

   Seeks in currently played track by specified time forward or back.
   :param p_delta: time in seconds to seek by; can be positive to seek forward or negative to seek back.

.. function:: PlaybackControl.playback_can_seek()

   :returns: bool

   Returns whether currently played track is seekable. If it's not, playback_seek/playback_seek_delta calls will be ignored.

.. function:: PlaybackControl.playback_get_position()

   :returns: double

   Returns current playback position within currently played track, in seconds.

.. function:: PlaybackControl.playback_format_title(string script)

   :returns: string

   Renders information about currently playing item. The script represents the
   query to use for formatting the current playing item.

.. function:: PlaybackControl.playback_format_title_complete()

   :returns: string

   Similar to :func:`PlaybackControl.playback_format_title`, with the caveat that this
   function has the script hardcoded.

.. function:: PlaybackControl.playback_get_length()

   :returns: double

   Retrieves length of currently playing item.

.. function:: PlaybackControl.playback_get_length_ex()

   :returns: double

   Extended version: queries dynamic track info for the rare cases where that is different from static info.

.. function:: PlaybackControl.toggle_stop_after_current()

   Toggles stop-after-current state.

.. function:: PlaybackControl.toggle_pause()

   Toggles pause state.

.. function:: PlaybackControl.play_or_pause()

   Starts playback if playback is inactive, otherwise toggles pause.

.. function:: PlaybackControl.play_or_unpause()

.. function:: PlaybackControl.next()

.. function:: PlaybackControl.previous()

.. function:: PlaybackControl.is_muted()

   :returns: bool

   Check if the volume is muted.

.. function:: PlaybackControl.get_volume_step()

   :returns: double

   Returns user-specified the step dB value for volume decrement/increment.

.. function:: Playlist.get_playlist_count()

   :returns: t_size

   Returns number of playlists.

.. function:: Playlist.get_active_playlist()

   :returns: t_size

   Retrieves index of active playlist; infinite if no playlist is active.

.. function:: Playlist.set_active_playlist(t_size)

   Sets active playlist (infinite to set no active playlist)

.. function:: Playlist.get_playing_playlist()

   :returns: t_size

   Retrieves playlist from which items to be played are taken from.

.. function:: Playlist.set_playing_playlist(t_size)

   Sets playlist from which items to be played are taken from.

.. function:: Playlist.remove_playlists(vector<t_size>)

   Removes playlists with the ids from the given list.

.. function:: Playlist.create_playlist(string name, t_size length, t_size index)

   :returns: size_t

   Creates a playlist with the given name. Do note that the API needs the length
   of the name. For instance, this might be useful in case the name is UTF-8,
   in which case the actual length might be double than what is expected.

.. function:: Playlist.create_playlist_ex(string name, t_size length, t_size index, vector<string> files)

   :returns: size_t

   Creates a playlist with the given name. Similar notes to :func:`Playlist.create_playlist`
   applies. The **files** parameter contains the files that are added to the initial
   content of the playlist.


.. function:: Playlist.reorder(vector<int> permutations)

   :returns: bool

   Reorder the playlists according to the given permutations.

.. function:: Playlist.playlist_get_item_count(t_size playlist)

   :returns: size_t

   Get the number of items from the given playlist.

.. function:: Playlist.activeplaylist_get_item_count()

   :returns: size_t

   Get the number of items from the active playlist.

.. function:: Playlist.playlist_get_name(t_size playlist)

   :returns: string

   Get the playlist's name.

.. function:: Playlist.activeplaylist_get_name()

   :returns: string

   Get the name of the active playlist.

.. function:: Playlist.playlist_reorder_items(t_size playlist, vector<int> permutation)

   :returns: bool

   Reorder the items in the specified playlist according to the specified permutation.


.. function:: Playlist.activeplaylist_reorder_items(vector<int> permutation)

   :returns: bool

   Reorder the items in the active playlist according to the specified permutation.

.. function:: Playlist.playlist_remove_items(t_size playlist, vector<t_size> items)

   :returns: bool

   Remove the specified items from the given playlist.

.. function:: Playlist.activeplaylist_remove_items(vector<t_size> items)

   :returns: bool

   Remove the specified items from the active playlist.

.. function:: Playlist.playlist_replace_item(t_size playlist, t_size item, t_size replacee)

   :returns: bool

   Replace the given item with the one specified by the **replacee** parameter, in the
   given playlist.

.. function:: Playlist.activeplaylist_replace_item(t_size item, t_size replacee)

   :returns: bool

   Replace the given item with the one specified by the **replacee** parameter,
   in the active playlist.

.. function:: Playlist.playlist_insert_items(t_size playlist, t_size base, vector<string> files)

   :returns: t_size

   Insert the given files at the specified position, in the given playlist.

.. function:: Playlist.activeplaylist_insert_items(t_size base, vector<string> files)

   :returns: t_size

   Insert the given files at the specified position, in the active playlist.

.. function:: Playlist.playlist_rename(t_size playlist, std::string name, t_size length)

   :returns: bool

   Change the name of the given playlist.

.. function:: Playlist.activeplaylist_rename(std::string name, t_size length)

   :returns: bool

   Change the name of the active playlist.

.. function:: Playlist.playlist_activate_next()

   Activate the next playlist.

.. function:: Playlist.playlist_activate_previous()

   Activate the previous playlist.

.. function:: Playlist.playlist_add_items(t_size playlist, vector<string> files)

   :returns: t_size
   
   Add the given files in the given playlist.

.. function:: Playlist.activeplaylist_add_items(vector<string> files)

   :returns: t_size
   
   Add the given files in the active playlist.

.. function:: Playlist.playlist_item_format_title(t_size playlist, t_size item, string format)

   :returns: string

   Format the given item with the given format string. The item is retrieved from
   the given playlist.

.. function:: Playlist.activeplaylist_item_format_title(t_size item, string format)

   :returns: string

   Format the given item with the given format string. The item is retrieved from
   the active playlist.

.. function:: Playlist.get_playing_item_location()

   :returns: std::tuple<bool, t_size, t_size>

   Get the playing item location. It returns a tuple of three elements, where
   the first element is a bool representing if there is a playing item or not,
   the second item represents the playlist where the playing item is located
   and the last element is the playing item's position in the given playlist.

.. function:: Playlist.playlist_sort_by_format(t_size playlist, string format, bool sel_only)

   :returns: bool

   Sort the given playlist by the given format string. The last parameter controls if the
   sorting should be done on the selection only or not.

.. function:: Playlist.activeplaylist_sort_by_format(string format, bool sel_only)

   :returns: bool

   Sort the active playlist by the given format string. The last parameter controls if the
   sorting should be done on the selection only or not.

.. function:: Playlist.playback_order_get_count()

   :returns: t_size

   Get the number of the playback items.

.. function:: Playlist.playback_order_get_name(t_size item)

   :returns: string

   Get the name of the given playback item.

.. function:: Playlist.playback_order_get_active()

   :returns: t_size

   Get the index of the active playback order item.

.. function:: Playlist.playback_order_set_active(t_size item)

   Set the given playback order item as active.

.. function:: Playlist.queue_add_item_playlist(t_size playlist, t_size p_item)

   Add the given item, from the given playlist, into the playback queue.

.. function:: Playlist.queue_get_count()

   :returns: t_size
   
   Get the number of items in the playback queue.

.. function:: Playlist.queue_get_contents()

   :returns: std::vector<std::map<std::string, t_size>>

   Return the list of playback queue's items. A playback queue's item
   is represented as a dictionary, containing the song's position and
   its playlist.

.. function:: Playlist.queue_remove_mask(vector<t_size> items)

   Remove the given items from the playback queue.

.. function:: Playlist.queue_flush()

   Remove all the items from the playback queue.

.. function:: Playlist.queue_is_active()

   :returns: bool

   Check if the playback queue is active.

.. function:: Playlist.remove_playlist(t_size playlist)

   :returns: bool

   Remove the given playlist.

.. function:: Playlist.remove_playlist_switch(t_size playlist)

   :returns: bool

   Remove the given playlist and the switch to the next one.


.. function:: Playlist.playlist_clear(t_size playlist)

   Clear the given playlist.


.. function:: Playlist.activeplaylist_clear()

   Clear the active playlist.

.. function:: Playlist.create_playlist_autoname(t_size position.)

   :returns: t_size

   Create a playlist with a default name at the specified position.

.. function:: Playlist.reset_playlist_playlist()

.. function:: Playlist.find_playlist(string name, t_size length)

   :returns: t_size

   Find the playlist with the given name.

.. function:: Playlist.find_or_create_playlist(string name, t_size length)

   :returns: t_size

   Find the playlist with the given name. If it does not exist, it is going
   to be created with the given name.

.. function:: Playlist.get_all_items(t_size playlist)

   :returns: std::vector<Track>

   Get all the items from the given playlist. Returns a list of tracks,
   where a track is a tuple of pairs, as in::

       (
           ("index", song_index),
           ("path", song_path),
           ("selected", is_song_selected),
           ("subsong_index", subsong_index)
       )

.. function:: Playlist.activeplaylist_get_all_items()

   :returns: std::vector<Track>

   See the documentation of :func:`Playlist.get_all_items`

.. function:: Playlist.playlist_get_item_handle(t_size playlist, t_size item)

   :returns: optional<Track>

   Get the handle of the given item, from the given playlist. If the item
   does not exist, this function will return a null. Otherwise, it will
   return a Track object, which is a tuple of pairs, with the format::

       (
           ("index", song_index),
           ("path", song_path),
           ("selected", is_song_selected),
           ("subsong_index", subsong_index)
       )

.. function:: Playlist.activeplaylist_get_item_handle(t_size item)

   :returns: optional<Track>

   Get the handle of the given item, from the active playlist.

.. function:: Playlist.playlist_get_items(t_size playlist, vector<t_size> masks)

   :returns: std::vector<Track>

   Get the items specified by **masks** from the given playlist.


.. function:: Playlist.activeplaylist_get_items(vector<t_size> masks)

   :returns: std::vector<Track>

   Get the items specified by **masks** from the active playlist.


.. _msgpack: http://msgpack.org/index.html
