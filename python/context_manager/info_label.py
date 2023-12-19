import dataclasses
from datetime import time
from typing import List, Any

import ipywidgets
from IPython.display import display

import time

from .types import DisplayableMixin


class InfoLabelCtxMgr(DisplayableMixin):
    """A class that represent a context manager for usage during processing msg."""

    hist_msg_count: int = 0
    msg_count: int = 0
    label_format: str = (
        "<i class='fa fa-{icon}'></i> <b>{short_txt}</b>  |  "
        "<b>Last msg:</b> {timestamp}  |  "
        "<b><u>Stored</u> msgs:</b> {num_msgs} "
        "<b>Historic msgs:</b> {hist_num_msgs}"
    )
    last_msg_ts: str = "No msg."

    # tmp variable
    __tmp_msg_diff: int = 0

    @dataclasses.dataclass
    class LabelStatus:
        icon: str
        short_txt: str

    LabelEmpty: LabelStatus = LabelStatus("check", "")
    LabelOk: LabelStatus = LabelStatus("check", "OK!")
    LabelProcessing: LabelStatus = LabelStatus("retweet", "Processing")
    LabelException: LabelStatus = LabelStatus(
        "times", "Error occurred (check captured log)"
    )

    def __init__(self, bounded_list: List[Any]):
        self.label = ipywidgets.HTML()

        self.update_label(self.LabelEmpty)
        self.__bounded_list = bounded_list

    def display(self):
        display(self.label)

    def __enter__(self):
        # self.last_msg_ts = time.strftime('%d-%m_%H:%M', time.localtime())
        self.last_msg_ts = time.strftime("%H:%M", time.localtime())
        self.update_label(self.LabelProcessing)
        self.__tmp_msg_diff = len(self.__bounded_list)

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.msg_count = len(self.__bounded_list)
        self.hist_msg_count += max(0, self.msg_count - self.__tmp_msg_diff)

        status = self.LabelOk
        if exc_type is not None:
            status = self.LabelException

        self.update_label(status)

    def update_label(self, status: LabelStatus):
        self.label.value = self.label_format.format(
            icon=status.icon,
            short_txt=status.short_txt,
            timestamp=self.last_msg_ts,
            num_msgs=self.msg_count,
            hist_num_msgs=self.hist_msg_count,
        )
