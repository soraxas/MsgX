import time

import deserialise

import zmq

MSGX_ADDRESS = "tcp://127.0.0.1:5558"
MSGX_MODE_DEFAULT = "default"
MSGX_MODE_ASYNC = "async"


class MsgXReceiver:
    """A class that listen to message from cpp"""

    def __init__(self, address=MSGX_ADDRESS, ctx_mgr=None):
        self.address = address
        self.socket = None
        self.mode = None

    # noinspection PyUnresolvedReferences
    def initialise(self, sleep=1, mode=MSGX_MODE_DEFAULT):
        if self.mode == mode:
            return
        if mode == MSGX_MODE_ASYNC:
            context = zmq.asyncio.Context()
        elif mode == MSGX_MODE_DEFAULT:
            context = zmq.Context()
        else:
            raise ValueError("Unknown mode {}".format(mode))
        self.mode = mode
        socket = context.socket(zmq.SUB)
        socket.connect(self.address)
        socket.setsockopt_string(zmq.SUBSCRIBE, "")
        self.socket = socket
        time.sleep(sleep)

    @staticmethod
    def _get_msg(encoded_msg):
        return deserialise.decode_message(encoded_msg)

    def get_msg_func(self, flags=0):
        """Return a function that process the incoming encoded msg"""
        self.initialise(mode=MSGX_MODE_DEFAULT)
        encoded_msg = self.socket.recv(flags=flags)
        return lambda: self._get_msg(encoded_msg)

    def get_msg(self, flags=0):
        return self.get_msg_func(flags)()

    async def get_msg_async_func(self):
        """Return a function that process the incoming encoded msg, async-ly"""
        self.initialise(mode=MSGX_MODE_ASYNC)
        encoded_msg = await self.socket.recv()
        return lambda: self._get_msg(encoded_msg)

    async def get_msg_async(self):
        return (await self.get_msg_async_func())()


if __name__ == "__main__":
    receiver = MsgXReceiver()
    print("===== looping printing all received message in background =====")
    while True:
        print(receiver.get_msg())
        print("----------------------------------------------------------")
