"""
A helper file that just imports th capnp definitions, and proceed to
remove the import hook that it brings.
"""
import capnp

from . import msgx_capnp, ndarray_capnp

capnp.remove_import_hook()
