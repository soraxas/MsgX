from abc import ABC, abstractmethod


class DisplayableMixin(ABC):
    @abstractmethod
    def display(self):
        raise NotImplementedError()
