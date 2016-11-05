Implementation
==============

The component is based on two technologies:

 - instead of relying on COM, **foo_rpc** uses Windows named pipe
   for the communication protocol, having the address  **\\\\.\\pipe\\foobar2000**.

 - the parameters to the API methods, as well as the responses are
   serialized with msgpack_.


The APIs are exported as qualified names, where the first part is the class to which
the given method belongs and the second part is the actual method itself. For instance,
for starting the playback, one can call the method ``PlaybackControl.start``, while for
sorting the current playlist by a specific criteria, one can use ``Playlist.activeplaylist_sort_by_format``.
For more informations, see :ref:`foo_rpc API methods`.

Due to implementation limitations, we cannot know a priori what parameters a method requires when dispatching
the method. Due to this, the components needs to receive a two-packed object, containing the method
to be called and its parameters. For example, to call the **start** method, one could call the API with
this payload:
 
    .. code-block:: C++

       msgpack::sbuffer sbuf;
       msgpack::pack(sbuf, std::make_tuple(0, false));
       auto params = sbuf.data();

       msgpack::sbuffer sbuf;
       msgpack::pack(sbuf, std::make_tuple("PlaybackControl.start", params));
       auto payload = sbuf.data();

In this case, the component will know the first element of the tuple is the
method, while the second is the parameters. In case the method does not need
any parameters, the second element will not be unpacked, meaning that the first
packing is not necessary in this case.

Each API call returns a two-element tuple, where the first element designates the
status of the request, while the second one represents the result or the error, in
case the request failed. In case the request succeeds, the first element is 0.


.. _msgpack: http://msgpack.org/index.html