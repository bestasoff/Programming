import functools
import datetime


def profiler(func):  # type: ignore
    """
    Returns profiling decorator, which counts calls of function
    and measure last function execution time.
    Results are stored as function attributes: `calls`, `last_time_taken`
    :param func: function to decorate
    :return: decorator, which wraps any function passed
    """

    @functools.wraps(func)
    def wrapper(*args, **kwargs):  # type: ignore
        if wrapper.exits == wrapper.calls:
            wrapper.exits = 0
            wrapper.calls = 0
        wrapper.begin = datetime.datetime.now()
        wrapper.calls += 1
        resulted_func = func(*args, **kwargs)
        wrapper.exits += 1
        wrapper.last_time_taken = (datetime.datetime.now() - wrapper.begin).total_seconds()
        return resulted_func

    wrapper.calls = 0
    wrapper.exits = 0
    return wrapper
