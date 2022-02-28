from numpy import ndarray
from typing import Optional

__version__ = ...

def print(msg: str) -> None:
    """
    prints a string using printf on the C side
    """
    ...

class Dummy:
    def __init__(self, seed: int, toss_count: Optional[int],
                 min_int: Optional[int], max_int: Optional[int],
                 min_float: Optional[float], max_float: Optional[float]):
        """
        intializes the dummy object. instance attributes can be set to
        control the randoming function.
        """
        self.randi_min = ...
        self.randi_max = ...
        self.randf_min = ...
        self.randf_max = ...

    def frand() -> float:
        """
        returns a single random float in the set range
        """
        ...
    def frand2d(height: int, width: int) -> ndarray:
        """
        returns a random float matrix in the set range
        """
        ...
    def irand() -> int:
        """
        returns a single random integer in the set range
        """
        ...
    def irand2d(height: int, width: int) -> ndarray:
        """
        returns a random integer matrix in the set range
        """
        ...

    @property
    def seed(): ...

    @property
    def toss_count(): ...