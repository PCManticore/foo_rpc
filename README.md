foo_rpc
=======

[![Build status](https://ci.appveyor.com/api/projects/status/mn3bmdfna7bw20rt?svg=true)](https://ci.appveyor.com/project/PCManticore/foo-rpc)
[![Download](https://api.bintray.com/packages/pcmanticore/foo_rpc/foo_rpc/images/download.svg)](https://bintray.com/pcmanticore/foo_rpc/foo_rpc/_latestVersion)
[![Documentation Status](https://readthedocs.org/projects/foo-rpc/badge/?version=latest)](http://foo-rpc.readthedocs.io/en/latest/?badge=latest)

This project is a [foobar2000](http://www.foobar2000.org/) component,
which exposes a RPC interface for a part of foobar2000's internal API.
This is similar in nature to [foo_com_automation](http://foosion.foobar2000.org/0.9/#comserver),
although based on different technologies.


Building
=========

The built DLL is going to be copied in a local directory, where it expects
a similar structure with the foobar2000's portable installation, the directory
being `portable_test\components`.

If you don't have this location already existing, you can omit the post build
step with ``/p:PostBuildEventUseInBuild=false``.
