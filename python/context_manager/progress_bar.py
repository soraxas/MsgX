import ipywidgets


class ProgressBarCtxMgr:
    """A class that represent a context manager for
    usage during processing msg."""

    def __init__(self):
        self.w_progress_bar = ipywidgets.IntProgress(
            value=0, min=0, max=1, description="Progress:", bar_style="info"
        )
        self.w_progress_bar.layout.display = "none"

    def add(self):
        self.w_progress_bar.value += 1
        if self.w_progress_bar.value == self.w_progress_bar.max:
            self.w_progress_bar.bar_style = "success"

    def start(self, num: int):
        self.w_progress_bar.layout.display = ""
        self.w_progress_bar.value = 0
        self.w_progress_bar.max = num
