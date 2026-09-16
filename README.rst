gcc-python
==========

This is a plugin for GCC, which links against libpython, and (I hope) allows
you to invoke arbitrary Python scripts from inside the compiler.  The aim is to
allow you to write GCC plugins in Python.

The plugin is Free Software, licensed under the GPLv3 (or later).

It's still at the "experimental proof-of-concept stage"; expect crashes and
tracebacks (I'm new to insides of GCC, and I may have misunderstood things).

It's already possible to use this to add additional compiler errors/warnings,
e.g. domain-specific checks, or static analysis.  One of my goals for this is
to "teach" GCC about the common mistakes people make when writing extensions
for CPython, but it could be used e.g. to teach GCC about GTK's
reference-counting semantics, or about locking in the Linux kernel, or about
signal-safety in APIs.

Other ideas include visualizations of code structure.   Given a ``gcc.CFG``
instance, ``gccutils.render_to_dot(cfg)`` and ``gccutils.invoke_dot(cfg)`` will
use graphviz and eog to plot a handy visualization of a control flow graph,
showing the source code interleaved with GCC's ``GIMPLE`` internal
representation.

The documentation can be seen at:

  http://gcc-python-plugin.readthedocs.io/en/latest/index.html


Maintenance status
------------------

The plugin is being modernized and maintained at
https://github.com/mmatti-sw/gcc-python-plugin.

It was originally developed against GCC 4.6 through 8 and Python 2.7 and 3.x.
That support is historical and is no longer maintained.

GCC does not provide a stable plugin API: its internals change from one
release to the next, so compatibility has to be established, and tested,
separately for each GCC release.  Compatibility with current GCC releases is
being re-established, and no GCC release is claimed as supported until the
plugin has been built and its test suite run against it.  Verified GCC
releases will be listed here.

Python 3 is the maintenance target; Python 2 is no longer supported.

x86_64 and ppc64le are the intended test platforms.


Requirements
------------

* GCC, with the plugin development headers for that exact GCC version: usually
  available in distribution packages such as ``gcc-N-plugin-dev`` or
  ``gcc-plugin-devel``.  The plugin is compiled as C++, so the matching C++
  compiler (e.g. ``g++-N``) is needed as well.  See "Maintenance status" above
  for which GCC releases have been verified.

* Python 3, with its development headers (e.g. ``python3-dev`` or
  ``python3-devel``)


Usage
-----

I use::

    make

to build the plugin and run the tests

You can also use::

   make demo

to demonstrate the new compiler errors.

The plugin was originally developed on x86_64.  x86_64 and ppc64le are the
intended test platforms; other architectures have not been verified.

There isn't an installer yet.  In theory you should be able to add these
arguments to the gcc invocation::

    gcc -fplugin=python.so -fplugin-arg-python-script=PATH_TO_SCRIPT.py OTHER_ARGS

and have it run your script as the plugin starts up.

The plugin automatically adds the absolute path to its own directory to the
end of its `sys.path`, so that it can find support modules, such as gccutils.py
and `libcpychecker`.

The exact API is still in flux; you can currently connect to events by
registering callbacks e.g. to be called for each function in the source at
different passes.

It exposes GCC's various types as Python objects, within a "gcc" module.  You
can see the API by running::

    import gcc
    help(gcc)

from within a script.


Overview of the code
--------------------

This is currently three projects in one:

``gcc-python-*``: the plugin for GCC.  The entrypoint (``init_plugin``) is in
``gcc-python.c``.

``libcpychecker`` and ``cpychecker.py``: a Python library (and a driver script),
written for the plugin, in which I'm building new compiler warnings to
help people find bugs in CPython extension code.

``cpybuilder``: a handy module for programatically generating C source code for
CPython extensions.  I use this both to generate parts of the GCC plugin, and
also in the selftests for the cpychecker script.  (I initially attempted to use
Cython for the former, but wrapping the "tree" type hierarchy required more
programatic control)

Coding style: Python and GCC each have their own coding style guide for C.
I've chosen to follow Python's (PEP-7), as I prefer it (although my code is
admittedly a mess in places).

You'll find API documentation within the "docs" directory, written in the
reStructuredText format (as is this file, in fact).  If you have Sphinx_
installed, you can regenerate these docs using::

   make html

within the ``docs`` directory.  Sphinx is the ``python-sphinx`` package on a
Fedora/RHEL box.

.. _Sphinx: http://sphinx.pocoo.org/


More detailed documentation can be seen within ``docs/getting-involved.rst``.

Enjoy!
David Malcolm <dmalcolm@redhat.com>
