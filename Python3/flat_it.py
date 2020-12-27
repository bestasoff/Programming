from typing import Iterable, Generator, Any


def flat_it(sequence: Iterable[Any]) -> Generator[Any, None, None]:
    """
    :param sequence: sequence with arbitrary level of nested iterables
    :return: generator producing flatten sequence
    """
    try:
        if isinstance(sequence, str):
            for char in sequence:
                yield char
        else:
            for seq in sequence:
                yield from flat_it(seq)
    except TypeError:
        yield sequence
