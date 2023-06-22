from typing import List, Awaitable

import asyncio
import zmq

from .context_manager.info_label import InfoLabelCtxMgr
from . import deserialise
from .receiver import (
    MsgXReceiverMode,
    MsgXReceiverBase,
    MsgXMessageCallBack,
    MsgXMessage,
)

# from IPython.display import display

from . import context_manager


class MsgXAsyncReceiver(MsgXReceiverBase):
    """A class that listen to message from cpp"""

    mode: MsgXReceiverMode = MsgXReceiverMode.Asynchronous
    msgs: List[MsgXMessage] = []
    async_task = None

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.ctx_mgr = InfoLabelCtxMgr()

    def default_callback(self, msg: MsgXMessage):
        self.msgs.append(msg)

    @property
    def context(self):
        # noinspection PyUnresolvedReferences
        return zmq.asyncio.Context()

    async def get_msg(self, flag: int = 0) -> Awaitable[MsgXMessage]:
        """Return a function that process the incoming encoded msg, async-ly"""
        self.initialise()
        encoded_msg = await self.socket.recv(flag)
        with self.ctx_mgr:
            return deserialise.decode_message(encoded_msg)

    def can_spin(self) -> bool:
        if self.spinning_asyncly:
            print("Already spinning asyncly.")
            return False
        return True

    def _spin_impl(self, callback: MsgXMessageCallBack) -> List[MsgXMessage]:
        async def _spin_async():
            while True:
                message = await self.get_msg()
                callback(message)

        # noinspection PyUnresolvedReferences
        self.async_task = asyncio.create_task(_spin_async())
        self.ctx_mgr.display()
        return self.msgs

    def clear_msgs(self):
        self.msgs.clear()

    @property
    def spinning_asyncly(self) -> bool:
        if self.async_task is not None and not self.async_task.cancelled():
            return True
        return False
