import time
from abc import ABC, abstractmethod
from typing import Callable, Optional, TypeVar

import zmq
import enum

from . import deserialise

MsgXMessage = TypeVar("MsgXMessage")
MsgXMessageCallBack = Callable[[MsgXMessage], None]


class MsgXReceiverMode(enum.Enum):
    Synchronised = enum.auto()
    Asynchronous = enum.auto()


class MsgXReceiverBase(ABC):
    default_callback: MsgXMessageCallBack
    mode: MsgXReceiverMode

    def __init__(
        self,
        address: Optional[str] = None,
        *,
        protocol: str = "tcp",
        ip_address: str = "127.0.0.1",
        port: int = 5558,
    ):
        if address is None:
            address = f"{protocol}://{ip_address}:{port}"
        self.address: str = address
        self.socket = None
        self.current_mode: Optional[MsgXReceiverMode] = None

    def __repr__(self):
        return (
            f"<{self.__class__.__name__} mode={self.current_mode}, "
            f"addr={self.address}', socket={self.socket}>"
        )

    def initialise(self, sleep: float = 1):
        mode = self.mode
        if self.current_mode is mode:
            return
        self.current_mode = mode
        socket = self.context.socket(zmq.SUB)
        socket.connect(self.address)
        socket.setsockopt_string(zmq.SUBSCRIBE, "")
        self.socket = socket
        time.sleep(sleep)

    @abstractmethod
    def get_msg(self, flags: int) -> MsgXMessage:
        raise NotImplementedError()

    def can_spin(self) -> bool:
        return True

    @abstractmethod
    def _spin_impl(self, callback: MsgXMessageCallBack):
        raise NotImplementedError()

    def spin(self, callback: MsgXMessageCallBack = None):
        if not self.can_spin():
            return
        if callback is None:
            callback = self.default_callback

        return self._spin_impl(callback)

    @property
    @abstractmethod
    def context(self):
        raise NotImplementedError()


class MsgXReceiver(MsgXReceiverBase):
    """A class that listen to message from cpp"""

    mode: MsgXReceiverMode = MsgXReceiverMode.Synchronised

    def context(self):
        return zmq.Context()

    @staticmethod
    def default_callback(self, message):
        print(message)
        print("----------------------------------------------------------")

    def get_msg(self, flags: int = 0) -> MsgXMessage:
        """Return a function that process the incoming encoded msg"""
        self.initialise()
        return deserialise.decode_message(self.socket.recv(flags=flags))

    def _spin_impl(self, callback: MsgXMessageCallBack):
        while True:
            callback(self.get_msg())


def spin_default():
    receiver = MsgXReceiver()
    print("===== looping printing all received message in background =====")
    try:
        receiver.spin()
    except KeyboardInterrupt:
        exit(0)


if __name__ == "__main__":
    spin_default()
