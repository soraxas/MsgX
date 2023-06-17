import time
import zmq
import enum

from . import deserialise


class MsgXReceiverMode(enum.Enum):
    Synchronised = enum.auto()
    Asynchronous = enum.auto()


class MsgXReceiver:
    """A class that listen to message from cpp"""

    def __init__(
        self, address=None, *, protocol="tcp", ip_address="127.0.0.1", port=5558
    ):
        if address is None:
            address = f"{protocol}://{ip_address}:{port}"
        self.address = address
        self.socket = None
        self.mode = None

    # noinspection PyUnresolvedReferences
    def initialise(self, mode, sleep=1):
        if self.mode == mode:
            return
        if mode == MsgXReceiverMode.Asynchronous:
            context = zmq.asyncio.Context()
        elif mode == MsgXReceiverMode.Synchronised:
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
        self.initialise(mode=MsgXReceiverMode.Synchronised)
        encoded_msg = self.socket.recv(flags=flags)
        return lambda: self._get_msg(encoded_msg)

    def get_msg(self, flags=0):
        return self.get_msg_func(flags)()

    async def get_msg_async_func(self):
        """Return a function that process the incoming encoded msg, async-ly"""
        self.initialise(mode=MsgXReceiverMode.Asynchronous)
        encoded_msg = await self.socket.recv()
        return lambda: self._get_msg(encoded_msg)

    async def get_msg_async(self):
        return (await self.get_msg_async_func())()


def spin_default():
    receiver = MsgXReceiver()
    print("===== looping printing all received message in background =====")
    try:
        while True:
            print(receiver.get_msg())
            print("----------------------------------------------------------")
    except KeyboardInterrupt:
        exit(0)


if __name__ == "__main__":
    spin_default()
