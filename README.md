[![Build Status](https://travis-ci.org/telyn/prpl-jrc.svg?branch=master)](https://travis-ci.org/telyn/prpl-jrc)

What?
=====

libpurple implementation of a [JRChat](http://creatures.wiki/JRChat) client.

Why?
====

I'm an old man in a young woman's body: I like archival and protocol preservation.

Also I know JRC pretty well at this point - I've written 2 or 3 servers and a client for it in the past.

And finally to practice writing some C.

Can I Test It?
==============

Assuming the project's got to that point, sure. There's a server at main.srv.telynz.uk:41528. If you want something to compare it to, download Greenreaper's [JRNet](http://greenreaper.co.uk/jrnet.zip) and [the update](http://greenreaper.co.uk/jrnetupdate.zip)

Does It Work On (your platform of choice here)
==============================================

If it's not linux or mac, probably not. Try building it anyway I guess and send me a pull request if you get it working.

How Do I Build It?
==================

Mac
----

```sh
git clone https://github.com/telyn/prpl-jrc.git
brew install pidgin
make
make test # to run the tests
make install # if you wanna install the plugin into your pidgin plugins directory
```

Linux
-----

Install the libpurple development headers then run
```sh
make
```

If you have pidgin installed then `sudo make install` should install it for you.

If you want to use it with a non-pidgin client, submit a PR describing how to install it.
