foo_rpc
=======

[![Build status](https://ci.appveyor.com/api/projects/status/mn3bmdfna7bw20rt?svg=true)](https://ci.appveyor.com/project/PCManticore/foo-rpc)
[![Download](https://api.bintray.com/packages/pcmanticore/foo_rpc/foo_rpc/images/download.svg)](https://bintray.com/pcmanticore/foo_rpc/foo_rpc/_latestVersion)

Automate foobar2000 audio player through a RPC interface.

Pretty much work in progress.


Building
=========

The built DLL is going to be copied in a local directory, where it expects
a similar structure with the foobar2000's portable installation, the directory
being `portable_test\components`.

If you don't have this location already existing, you can omit the post build
step with ``/p:PostBuildEventUseInBuild=false``.