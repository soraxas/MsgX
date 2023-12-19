from typing import List, Awaitable

import asyncio
import zmq.asyncio

# from .context_manager.info_label import InfoLabelCtxMgr
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

    __msgs: List[MsgXMessage] = []
    async_task = None

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        # self.ctx_mgr = InfoLabelCtxMgr(bounded_list=self.__msgs)

    async def initialise(self, sleep: float = 1):
        if self.current_mode is MsgXReceiverMode.Asynchronous:
            return
        self.current_mode = MsgXReceiverMode.Asynchronous
        socket = self.context.socket(zmq.SUB)
        socket.connect(self.address)
        socket.setsockopt_string(zmq.SUBSCRIBE, "")
        self.socket = socket
        await asyncio.sleep(sleep)

    def default_callback(self, msg: MsgXMessage):
        pass

    @property
    def msgs(self) -> List[MsgXMessage]:
        return self.__msgs

    @property
    def context(self):
        # noinspection PyUnresolvedReferences
        return zmq.asyncio.Context()

    async def just_get_msg(self, flag: int = 0) -> Awaitable[MsgXMessage]:
        """Return a function that process the incoming encoded msg, async-ly"""
        await self.initialise()
        return deserialise.decode_message(await self.socket.recv(flag))

    async def get_msg(self, flag: int = 0) -> Awaitable[MsgXMessage]:
        """Return a function that process the incoming encoded msg, async-ly"""
        await self.initialise()
        print("getting")
        encoded_msg = await self.socket.recv(flag)
        # with self.ctx_mgr:
        return self.__inner_get_message(encoded_msg)

    def __inner_get_message(self, encoded_msg):
        message = deserialise.decode_message(encoded_msg)
        self.__msgs.append(message)
        return message

    def can_spin(self) -> bool:
        if self.spinning_asyncly:
            print("Already spinning asyncly.")
            return False
        return True

    def stop_spin(self) -> bool:
        if not self.spinning_asyncly:
            return True
        if not self.async_task.cancel():
            print("Error happens when stopping existing async task")
            return False
        self.async_task = None
        return True

    def _spin_impl(self, callback: MsgXMessageCallBack) -> List[MsgXMessage]:
        async def _spin_async():
            await self.initialise()

            async def __process_all_pending_messages():
                processed_at_least_one_message = False
                try:
                    while True:
                        callback(
                            self.__inner_get_message(
                                await self.socket.recv(zmq.NOBLOCK)
                            )
                        )
                        processed_at_least_one_message = True
                except zmq.Again:
                    # no more pending message
                    return processed_at_least_one_message

            while True:
                # message = await self.get_msg()
                # callback(message)
                encoded_msg = await self.socket.recv(0)
                with self.ctx_mgr:
                    callback(self.__inner_get_message(encoded_msg))
                    # process all remaining and pending message all in one go.
                    while await __process_all_pending_messages():
                        # sleep for a while to see if there are more to be processed
                        # this helps to avoid exiting our context manager, which can
                        # slow down things by quite a bit
                        await asyncio.sleep(0.05)

        # noinspection PyUnresolvedReferences
        self.async_task = asyncio.create_task(_spin_async())
        self.ctx_mgr.display()
        return self.__msgs

    def clear_msgs(self):
        with self.ctx_mgr:
            self.__msgs.clear()

    @property
    def spinning_asyncly(self) -> bool:
        if self.async_task is not None and not self.async_task.cancelled():
            return True
        return False
