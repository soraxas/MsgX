from datetime import time

import ipywidgets
from IPython.display import display

import time

from .types import DisplayableMixin


class InfoLabelCtxMgr(DisplayableMixin):
    """A class that represent a context manager for usage during processing msg."""

    hist_msg_count: int = 0
    msg_count: int = 0

    def __init__(self):
        self.label = ipywidgets.HTML()
        self.label_format = (
            "<i class='fa fa-{icon}'></i> <b>{short_txt}</b>  |  "
            "<b>Last msg:</b> {timestamp}  |  "
            "<b><u>Stored</u> msgs:</b> {num_msgs} "
            "<b>Historic msgs:</b> {hist_num_msgs}"
        )
        self.last_msg_ts = "No msg."
        self.update_label("", "check")

    def display(self):
        display(self.label)

    def increment(self):
        self.msg_count += 1
        self.hist_msg_count += 1

    def clear(self):
        self.msg_count = 0

    def __enter__(self):
        # self.last_msg_ts = time.strftime('%d-%m_%H:%M', time.localtime())
        self.last_msg_ts = time.strftime("%H:%M", time.localtime())
        self.update_label("Processing", "retweet")
        self.increment()

    def __exit__(self, exc_type, exc_value, exc_traceback):
        icon = "check"
        short_txt = "OK!"
        if exc_type is not None:
            icon = "times"
            short_txt = "Error occurred (check captured log)"
        self.update_label(short_txt, icon)

    def update_label(self, short_txt, icon="check"):
        self.label.value = self.label_format.format(
            icon=icon,
            short_txt=short_txt,
            timestamp=self.last_msg_ts,
            num_msgs=self.msg_count,
            hist_num_msgs=self.hist_msg_count,
        )
