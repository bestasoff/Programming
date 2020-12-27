from typing import Callable, Any, TypeVar, cast
from collections import OrderedDict
import functools


Function = TypeVar('Function', bound=Callable[..., Any])


def cache(max_size: int) -> Callable[[Function], Function]:
    """
    Returns decorator, which stores result of function
    for `max_size` most recent function arguments.
    :param max_size: max amount of unique arguments to store values for
    :return: decorator, which wraps any function passed
    """
    def lru_cache(func: Function) -> Any:
        l_cache: OrderedDict[Any, Any] = OrderedDict()

        @functools.wraps(func)
        def wrapper(*args: Any, **kwargs: Any) -> Any:
            if args not in l_cache:
                result = func(*args, **kwargs)
                if len(l_cache) == max_size:
                    l_cache.popitem(last=False)
                l_cache[args] = result
            else:
                l_cache.move_to_end(args, last=True)
            return l_cache[args]

        return cast(Function, wrapper)
    return lru_cache
