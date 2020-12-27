import sys
import typing as tp


def input_(prompt: tp.Optional[str] = None,
           inp: tp.Optional[tp.IO[str]] = None,
           out: tp.Optional[tp.IO[str]] = None) -> tp.Optional[str]:
    """Read a string from `inp` stream. The trailing newline is stripped.

    The `prompt` string, if given, is printed to `out` stream without a
    trailing newline before reading input.

    If the user hits EOF (*nix: Ctrl-D, Windows: Ctrl-Z+Return), return None.

    `inp` and `out` arguments are optional and should default to `sys.stdin`
    and `sys.stdout` respectively.
    """
    if out is None:
        sys.stdout.write(prompt.strip('\n') if prompt else '')
    else:
        out.write(prompt.strip('\n') if (prompt is not None) else '')
        out.flush()
    res: str = ''
    if inp is None:
        try:
            res = sys.stdin.read().strip('\n')
        except EOFError:
            return None
    else:
        try:
            res = inp.read().strip('\n')
        except EOFError:
            return None
    return res if res else None
